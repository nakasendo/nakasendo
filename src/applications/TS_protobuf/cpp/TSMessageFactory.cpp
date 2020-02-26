
#include <cstdlib>
#include <TSMessageFactory.h>
#include <orchestrator.h>
#include <GroupMetaData.h>
#include <player.h>
#include <TSProtoBufHelper.h>
#include <ts_messages.pb.h>



std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

void createRegisterRequest(const player& p, std::ostream& os){
    
    thresholdsignature::Player* tsplayer = new thresholdsignature::Player();
    tsplayer->set_name(p.userID());
    tsplayer->set_uri(p.uri()); 
    tsplayer->set_addr(p.addr()); 
    tsplayer->set_port(p.port()); 

    thresholdsignature::RegisterRequest registerreq;
    registerreq.set_allocated_playerid(tsplayer);

    
    os << TSMessageDefs::TS_REGISTER_PLAYER_REQUEST;
    if (!registerreq.SerializeToOstream(&os)){
        throw std::runtime_error ("Could not serialize register message");
    }
    
    return ; 
}

void createRegisterReply(const player& p, std::ostream& os){
    thresholdsignature::RegisterReply resp;
    resp.set_success(true);
    os << TSMessageDefs::TS_REGISTER_PLAYER_RESPONSE; 
    if (!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize register player response");
    }  
    return ; 
}

void createListPlayersRequest(const player& p, std::ostream& os){
    thresholdsignature::GetPlayerListRequest playerlistreq;
    playerlistreq.set_name(p.userID());
    os << TSMessageDefs::TS_LIST_PLAYER_REQUEST;
    if (!playerlistreq.SerializeToOstream(&os)){
        throw std::runtime_error ("Could not serialize player list message");
    }
    return ;  
}

void createListPlayersResponse(const player& p, std::ostream& os){
    thresholdsignature::GetPlayerListResponse resp ;
    std::vector<player> players = getPlayerList(); 
    for(std::vector<player>::const_iterator iter = players.begin(); iter != players.end(); ++ iter){
        thresholdsignature::Player * tsp = resp.add_playerid();
        tsp->set_name(iter->userID());
        tsp->set_uri(iter->uri());
        tsp->set_addr(iter->addr());
        tsp->set_port(iter->port());
    }
    os << TSMessageDefs::TS_LIST_PLAYER_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error( "Unable to serialize get user list response");
    }
    return;
}

void createGroupRequest(const player& p, const int& m, const int& n, std::ostream& os){
    thresholdsignature::CreateGroupRequest grpreq;
    grpreq.set_userid(p.userID());
    grpreq.set_m(m);
    grpreq.set_n(n);
    os << TSMessageDefs::TS_CREATE_GROUP_REQUEST;
    if(!grpreq.SerializeToOstream(&os)){
        throw std::runtime_error ("Could not serialize create threshold group message" );
    }
    return ;
}

void createGroupReply(const player& p, const std::string& grpid, const bool& grpset, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::CreateGroupReply grpRsp;     
    id->set_userid( p.userID() );
    id->set_groupid( grpid );
    grpRsp.set_allocated_id( id ) ;
    os << TSMessageDefs::TS_CREATE_GROUP_RESPONSE;
    if(!grpRsp.SerializeToOstream(&os)){
        throw std::runtime_error( "Unabe to serialize a create threshold group response" );
    }
    return;
}

void createInviteRequest( const std::string& grpid, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::InviteRequest inviteReq;
    id->set_groupid( grpid );
    inviteReq.set_allocated_id( id ) ;

    os << TSMessageDefs::TS_INVITE_TO_GROUP_REQUEST;
    if (!inviteReq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize invite to threshold group message");
    }
    return ; 
}
 
void createInviteReply(const std::string& grpid, const std::string& playerid, const bool& accepted, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::InviteReply inviteResp;

    id->set_userid  ( playerid );
    id->set_groupid ( grpid );
    inviteResp.set_allocated_id( id ) ;    
    inviteResp.set_acceptance( accepted ); 

    os << TSMessageDefs::TS_INVITE_TO_GROUP_RESPONSE;
    if (!inviteResp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize invite to threshold group message");
    }
    return ; 
}

void createGroupIsSetRequest
    (const std::string& userId, const GroupMetadata& grp, std::ostream& os){

    std::cout << "In createGroupIsSetRequest" << std::endl ;

    thresholdsignature::GroupIsSetRequest tsreq;
    tsreq.set_groupid( grp.groupid() ) ;
    tsreq.set_degree( grp.t() ) ;

    int ordinal( 1 ) ;
    for ( auto iter : grp.participantList() ) 
    {        
        thresholdsignature::Player* id = new thresholdsignature::Player( ) ; 
        id->set_name( iter.m_userid ) ;
        id->set_uri ( iter.m_uri ) ;
        id->set_addr( iter.m_ip) ;
        id->set_port( iter.m_port ) ;
 
        thresholdsignature::GroupIsSetRequest::Participant* ptcpnt = tsreq.add_participants() ; 
        
        ptcpnt->set_ordinal  ( ordinal++ ) ;
        ptcpnt->set_allocated_playerid( id ) ;
    }


    os << TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_REQUEST ; 
    if(!tsreq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize broadcast group details");
    }
    return;
}

void createGroupIsSetReply(const std::string& grpid, const std::string& playerid, const bool& userset, std::ostream& os){

    
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::GroupIsSetReply resp;
    id->set_userid( playerid );
    id->set_groupid( grpid );
    resp.set_allocated_id( id ) ;


    resp.set_success(userset);
    os << TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("Unable to serialize broadcast group response");
    }
    return ; 
}

void createDeleteTSGroupRequest(const player& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::DeleteGroupRequest req; 
    req.set_userid(p.userID());
    req.set_groupid(grpid);
    os << TSMessageDefs::TS_DELETE_GROUP_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize deleteGroup message for grp id " + grpid); 
    }
    return; 
}

void createDeleteTSGroupResponse(const player& p, const std::string& grpid, const bool& deleted, std::ostream& os){
    thresholdsignature::DeleteGroupResonse resp;
    resp.set_userid(p.userID());
    resp.set_groupid(grpid); 
    resp.set_groupdeleted(deleted);
    os << TSMessageDefs::TS_DELETE_GROUP_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize deleteGroupResponse message for grp id " + grpid); 
    }
    return ; 
}

void createDeleteTSPlayerGroupRequest(const std::string& userid, const std::string& grpid, std::ostream& os){
    thresholdsignature::DeleteGroupPlayerRequest req;
    req.set_userid(userid);
    req.set_groupid(grpid);
    os << TSMessageDefs::TS_DELETE_PLAYER_GROUP_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize deleteTSPlayerGroupRequest message for grp id " + grpid);
    }
    return;
}

void createDeleteTSPlayerGroupResponse(const std::string& userid, const std::string& grpid, const bool& deleted, std::ostream& os){
    thresholdsignature::DeleteGroupPlayerResponse resp;
    resp.set_userid(userid);
    resp.set_groupid(grpid);
    resp.set_deleted(deleted);
    os << TSMessageDefs::TS_DELETE_PLAYER_GROUP_RESPONSE; 
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize deleteTSPlayerGroupResponse message for grp id " + grpid);
    }
    return ;
}

void createSecretSharingRequest(const player& p, const std::string& grpid, const std::string& calctype, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::ShareSecretRequest req;

    id->set_userid          ( p.userID() );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;
    req.set_calculation     ( string2enum( calctype ) ) ;

    os << TSMessageDefs::TS_CREATE_GROUP_SECRET_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingRequest message for grp id grpid" + grpid); 
    }
    return ; 
}

void createSecretSharingResponse(const player& p, const std::string& grpid, const std::string& calctype, const bool& sharedsecret, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::ShareSecretReply resp;

    id->set_userid          ( p.userID() );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;
    resp.set_calculation    ( string2enum( calctype ) ) ;
    resp.set_success        ( sharedsecret ) ;

    os << TSMessageDefs::TS_CREATE_GROUP_SECRET_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingResponse message for group id" + grpid ); 
    }
    return;
}

void createSecretSharingInitEvalRequest(const std::string& proposer, const std::string& grpid, const std::string& calctype, std::ostream& os){
    
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::InitShareEvalsRequest req;

    id->set_userid          ( proposer );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;

    req.set_calculation     ( string2enum( calctype) );
    os << TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SecretSharingInitEvalRequest for group id" + grpid);
    }
    return;
}

void createSecretSharingInitEvalResponse(const std::string& userid, const std::string& grpid, const std::string& calctype, const bool& evalShareComplete, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::InitShareEvalsReply resp;

    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;

    resp.set_calculation    ( string2enum(calctype) ) ;
    resp.set_success        ( evalShareComplete );
    os << TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SecretSharingInitEvalRequest for group id" + grpid);
    }
    return;
}

void createPrivateDataEvalRequest(const std::string& userid, const std::string& grpid, const int& ord, const int& playerord, const jvrss& secrets, std::ostream& os){
    // look up the player ordinal in the vector of evals

    std::map<std::string, BigNumber>::const_iterator evalIter =  secrets.m_evals.find(std::to_string(playerord));
    if(evalIter == secrets.m_evals.end()){
        throw std::runtime_error("No evalution for player ordinal " + std::to_string(playerord) + " in the list of evaluated players");
    } 

    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::DistributeEvalsRequest req;

    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;

    req.set_fromordinal     (ord);
    req.set_f_x(evalIter->second.ToHex());
    os << TSMessageDefs::TS_CREATE_PRIVATE_EVAL_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PrivateDataEvalRequest for player " + std::to_string(ord) );
    }
    return;
}

void createPrivateDataEvalResponse(const std::string& userid, const std::string& grpid, const bool& evaladded, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::DistributeEvalsReply resp;

    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    resp.set_allocated_id    ( id ) ;

    resp.set_success        ( evaladded );
    os << TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PrivateDataEvalResponse for player " + userid );
    }
    return;
}
void createSecretSharingPlayerDataRequest(const std::string& p, const std::string& grpid, const std::string& calctype, std::ostream& os){

    thresholdsignature::ShareSecretDataRequest req;
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 

    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;
    req.set_calculation     ( string2enum( calctype ) );


    os << TSMessageDefs::TS_CREATE_PLAYER_SECRET_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingPlayerDataRequest for player p " + p + "and group id: " + grpid);
    }
    return;
}
void createSecretSharingPlayerDataResponse(const std::string& p, const std::string& grpid, const std::string& calctype , const int& ord, const jvrss& secrets, std::ostream& os){

    
    thresholdsignature::ShareSecretDataReply resp;
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;

    resp.set_ordinal        ( ord ) ;
    resp.set_calculation    ( string2enum( calctype ) );

    for(
            std::vector<ECPoint>::const_iterator hiddenPolyIter =  secrets.m_hiddenPolynomial.begin();
            hiddenPolyIter != secrets.m_hiddenPolynomial.end(); 
            ++hiddenPolyIter
        )
    {
        resp.add_hiddenpoly( hiddenPolyIter->ToHex() ) ; 
    }

    std::map<std::string, std::vector<std::pair<std::string,ECPoint> > >::const_iterator 
        hiddenEvalIter = secrets.m_hiddenEvals.find(std::to_string(ord) ) ;
    if ( hiddenEvalIter != secrets.m_hiddenEvals.end() ) 
    {

        // Create Hidden Evals
        for(
                std::vector<std::pair<std::string, ECPoint> >::const_iterator ecptIter = hiddenEvalIter->second.begin(); 
                ecptIter != hiddenEvalIter->second.end(); 
                ++ ecptIter
            )
        {
            thresholdsignature::evaluatedPoly* ep = resp.add_hiddenevals() ;
            ep->set_ordinal ( std::stoi( ecptIter->first ) ) ;
            ep->set_f_x     ( ecptIter->second.ToHex() ) ;
        }
    }
    
    os << TSMessageDefs::TS_CREATE_PLAYER_SECRET_RESPONSE ; 
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingPlayerDataResponse for player " + p + " and group " + grpid);
    }
    
    
    return;
}

void createSecretSharingCollatedDataRequest(const std::string& p, const std::string& grpid, const std::string& calctype, const GroupMetadata& grp, std::ostream& os){

    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::CollatedSecretRequest req;    
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;

    req.set_calculation( string2enum(calctype) ) ;
    

    // hiddenPolys
    for ( auto const& iter : grp.CollatedHiddenPolys() ) 
    {
        thresholdsignature::hiddenPolynomial* hp = req.add_hiddenpolys( ) ;
        hp->set_ordinal( std::stoi( iter.first ) ) ;

        for ( auto const& innerIter : iter.second ) 
        {
            hp->add_coefficients( innerIter.ToHex() ) ;
        }
    }

    // hiddenEvals
    for ( auto const& iter : grp.CollatedHiddenEvals() )
    {
        thresholdsignature::listOfPolynomials* lp = req.add_hiddenevals( ) ;        
        lp->set_ordinal( std::stoi( iter.first ) ) ;


        for ( auto const& innerIter : iter.second ) 
        {
            thresholdsignature::evaluatedPoly* ep = lp->add_ep( ) ;
            ep->set_ordinal( std::stoi( innerIter.first) ) ;
            ep->set_f_x( innerIter.second.ToHex() ) ;
        }
    }
    
    os << TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SecretSharingCollatedDataRequest for player " + p + " and group " + grpid);
    }
    return ; 
}

void createSecretSharingCollatedPlayerResponse(const std::string& p, const std::string& grpid, const std::string& calctype,std::ostream& os){

    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::CollatedSecretReply resp; 
 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;

    resp.set_calculation( string2enum( calctype ) ) ;
    resp.set_success( true ) ;
    os << TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize Collated Secret Reply for player " + p + " and group " + grpid);
    }
    
    return ; 
}

void createEphemeralKeyDataRequest(const std::string& p, const std::string& grpid, std::ostream& os){

    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::InitPresignRequest req;
 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id   ( id ) ;

    os << TSMessageDefs::TS_CREATE_PRE_SIG_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize InitPresignRequest for player " + p + " and group " + grpid);
    }
    return;
}

void createEphemeralKeyDataResponse(const std::string& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::InitPresignReply resp;
 
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;

    resp.set_success(true);
    
    os << TSMessageDefs::TS_CREATE_PRE_SIG_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize InitPresignReply for player " + p + " and group " + grpid);
    }
    return ;
}

void createEphermalKeyPlayerDataRequest(const std::string& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::ShareVWDataMessageRequest resp;
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;

    os << TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_REQUEST;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize ShareVWDataMessageRequest for player " + p + " and group " + grpid);
    }
    return ; 
}



void createEphemeralKeyPlayerDataResponse(const std::string& p, const std::string & grpid, const int& ordinal, const std::pair<BigNumber, ECPoint>& ekey, std::ostream& os){
    
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::ShareVWDataMessage resp;
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;  

    thresholdsignature::VWData* vw = new thresholdsignature::VWData( ) ;
    vw->set_ordinal( ordinal ) ;
    vw->set_v( ekey.first.ToHex() ) ;
    vw->set_w( ekey.second.ToHex() ) ;
    resp.set_allocated_data ( vw ) ;

    os << TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize EphemeralKeyPlayerDataResponse for player " + p + " and group " + grpid);
    }
    return ; 
}

void createEphemeralKeyCollatedDataRequest(const std::string& p, const std::string& grpid, const GroupMetadata& grp, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::CollatedVWShareRequest req;
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    req.set_allocated_id    ( id ) ;

    for(
            std::map<std::string, std::pair<BigNumber, ECPoint> >::const_iterator iter = grp.CollatedVW().begin(); 
            iter != grp.CollatedVW().end(); 
            ++ iter
        )
    {
        thresholdsignature::VWData* ptrOrdVW = req.add_data();
        ptrOrdVW->set_ordinal(std::stoi( iter->first));
        ptrOrdVW->set_v(iter->second.first.ToHex());
        ptrOrdVW->set_w(iter->second.second.ToHex());
    }
    os << TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_REQUEST;
    if(!req.SerializeToOstream(&os)){
       throw std::runtime_error("could not serialize AllEphemeralKeyPlayerRequest for player " + p + " and group " + grpid);
    }
    return ; 
}

void createAEphemeralKeyCollatedDataResponse(const std::string& p, const std::string& grpid, const std::string& ordinal, std::ostream& os){
   
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::CollatedVWShareReply resp;
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    resp.set_allocated_id   ( id ) ;    


    resp.set_ordinal( std::stoi( ordinal ) ) ;

    // need a way to track & report failures
    resp.set_success(true);
    os << TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize AllEphemeralKeyPlayerResponse for player " + p + " and group " + grpid); 
    }
    return ;
}

void createSignatureRequest(const std::string& p, const std::string& grpid, const std::string& msg, const int& eKeyIndex, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::InitSignatureRequest sigreq;
    id->set_userid          ( p );
    id->set_groupid         ( grpid );
    sigreq.set_allocated_id ( id ) ;  

    sigreq.set_message(msg);
    sigreq.set_keyindex(eKeyIndex);
    os << TSMessageDefs::TS_CREATE_GROUP_SIG_REQUEST;
    if(!sigreq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize InitSignatureRequest for player " + p + " and group " + grpid);
    }
    return ;
}

void createSignatureResponse(const std::string& p, const std::string& grpid, const GroupMetadata& grp , const std::string& msg, const int& ekeyindex,std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::SignDataMessage sigresp;
    id->set_userid              ( p );
    id->set_groupid             ( grpid );
    sigresp.set_allocated_id    ( id ) ;  

    sigresp.set_message(msg);
    sigresp.set_keyindex(ekeyindex);
    for(std::vector<std::pair<std::string, BigNumber> >::const_iterator iter = grp.CollatedPartialSignatures().begin(); iter != grp.CollatedPartialSignatures().end(); ++iter){
        thresholdsignature::SigData* siginfo = sigresp.add_signatures();
        siginfo->set_ordinal(std::stoi( iter->first) );
        siginfo->set_signature(iter->second.ToHex());
    }
    os << TSMessageDefs::TS_CREATE_GROUP_SIG_RESPONSE;
    if(!sigresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise GenerateSignatureResponse for player " + p + " and group " + grpid);
    }
    return;
}

void createSignaturePlayerDataRequest(const std::string& p, const std::string& grpid, const std::string& themessage, const int& ekeyIndex, std::ostream& os){
    thresholdsignature::ShareOfSigRequest playersigreq;
    playersigreq.set_groupid ( grpid ) ;  

    playersigreq.set_message(themessage);
    playersigreq.set_keyindex(ekeyIndex);
    os << TSMessageDefs::TS_CREATE_SIG_PLAYER_REQUEST;
    if(!playersigreq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SignaturePlayerDataRequest for player " + p + " and group " + grpid);
    }
    return ; 
}

void createSignaturePlayerDataResponse(const std::string& p, const std::string& grpid, const std::pair<std::string, BigNumber>& siginfo, const int& ekeyindex,std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::ShareOfSigReply playersigresp;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    playersigresp.set_allocated_id ( id ) ;  

    playersigresp.set_ordinal(std::stoi( siginfo.first ) );
    playersigresp.set_keyindex(ekeyindex);
    playersigresp.set_signature(siginfo.second.ToHex());
    os << TSMessageDefs::TS_CREATE_SIG_PLAYER_RESPONSE;
    if(!playersigresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SignaturePlayerDataResponse for player " + p + " and group " + grpid);
    }
    return ;
}

void createPrivateKeyRequest(const std::string& p,  const std::string& grpid, const std::vector<std::string>& users, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::GenerateGroupPrivateKeyRequest priKeyReq;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    priKeyReq.set_allocated_id ( id ) ;  
    
    for(std::vector<std::string>::const_iterator iter = users.begin(); iter != users.end(); ++ iter){
        priKeyReq.add_players( *iter ) ;
    }
    os << TSMessageDefs::TS_CREATE_PRIKEY_REQUEST;
    if(!priKeyReq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GenerateGroupPrivateKeyRequest for player " + p + " and group " + grpid);
    }
    return;
}

void createPrivateKeyResponse(const std::string& p, const std::string& grpid, const bool& keyshared, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::GenerateGroupPrivateKeyResponse prikeyresp;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    prikeyresp.set_allocated_id ( id ) ;  
    
    prikeyresp.set_privatedatashared(keyshared);
    os << TSMessageDefs::TS_CREATE_PRIKEY_RESPONSE;
    if(!prikeyresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GenerateGroupPrivateKeyResponse for player " + p + " and group " + grpid);
    }
    return;
}
 
 void createPlayerPrivateShareExchangeRequest(const std::string& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::PlayerPrivateShareExchangeRequest req;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    req.set_allocated_id ( id ) ;  
    
    os << TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_REQUEST;
    if(!req.SerializeToOstream(&os)){
       throw std::runtime_error("could not serialize PlayerPrivateShareExchangeRequest for player " + p + " and group " + grpid);
    }
    return;
 }

 void createPlayerPrivateShareExchangeResponse(const std::string& p, const std::string& grpid, const bool& keyshared, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::PlayerPrivateShareExchangeResponse resp;
    id->set_userid          ( p ) ;
    id->set_groupid         ( grpid ) ;
    resp.set_allocated_id ( id ) ;  

     resp.set_privatedatashared(keyshared);
     os << TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_RESPONSE ;
     if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateShareExchangeResponse for player " + p + " and group " + grpid);
    }
    return;

 }

 void createPlayerPrivateKeyShareRequest(const playerGroupMetaData& grpinfo,std::ostream& os){

    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::PlayerPrivateKeyShareRequest req;
    id->set_userid          ( getPublicPlayerInfo().userID());
    id->set_groupid         ( grpinfo.m_id );
    req.set_allocated_id ( id ) ;  


    req.set_ordinal(grpinfo.m_ordinal);
    req.set_privatekeyshare(grpinfo.privateKeyShare().ToHex());
    os << TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateKeyShareRequest for player " + getPublicPlayerInfo().userID() + " and group " + grpinfo.m_id);
    }
    return;
 }

void createPlayerPrivateKeyShareResponse(const std::string& userid, const std::string& grpid, const bool& sharerec, std::ostream& os){
    thresholdsignature::IdentityMessage* id = new thresholdsignature::IdentityMessage( ) ; 
    thresholdsignature::PlayerPrivateKeyShareResponse resp;
    id->set_userid          ( userid );
    id->set_groupid         ( grpid );
    resp.set_allocated_id ( id ) ;  

    resp.set_sharedprivatekeyshare(sharerec);
    os << TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateKeyShareResponse for player " + userid + " and group " + grpid);
    }
    return;
}

