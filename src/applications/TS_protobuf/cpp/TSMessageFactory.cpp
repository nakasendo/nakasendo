
#include <cstdlib>
#include <TSMessageFactory.h>
#include <orchestrator.h>
#include <GroupMetaData.h>
#include <player.h>
#include <ts.pb.h>



std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

void createTimeRequest(const player& p, std::ostream& os)//, const std::string& addr, const std::string& port){
{
    thresholdsignature::TimeRequest timeReq;
    timeReq.set_name (p.userID());
    timeReq.set_uuid (p.uri());  
    
    os << TSMessageDefs::TS_TIME_REQUEST; 
    if (!timeReq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize time request" );
    }

}

void createTimeResponse(const player& p, std::ostream& os)//, const std::string& addr, const std::string& port){
{
    thresholdsignature::TimeResponse timeResp;
    timeResp.set_timestring(make_daytime_string());
    std::cout << "Sending TimeResponse: " << timeResp.timestring() << " with id " << TSMessageDefs::TS_TIME_RESPONSE << std::endl;
    os << TSMessageDefs::TS_TIME_RESPONSE;
    if(!timeResp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize time response"); 
    }
    return;
}

void createRegisterPlayerRequest(const player& p, std::ostream& os){
    thresholdsignature::RegisterPlayerRequest registerreq;
    thresholdsignature::player* tsplayer = registerreq.add_playerid();
    tsplayer->set_name(p.userID());
    tsplayer->set_uri(p.uri()); 
    tsplayer->set_addr(p.addr()); 
    tsplayer->set_port(p.port()); 
    
    os << TSMessageDefs::TS_REGISTER_PLAYER_REQUEST;
    if (!registerreq.SerializeToOstream(&os)){
        throw std::runtime_error ("Could not serialize register message");
    }
    
    return ; 
}

void createRegisterPlayerResponse(const player& p, std::ostream& os){  
    thresholdsignature::RegisterPlayerResponse resp;
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
        thresholdsignature::player * tsp = resp.add_playerid();
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

void createThresholdGroupRequest(const player& p, const int& m, const int& n, std::ostream& os){
    thresholdsignature::CreateThresholdGroup grpreq;
    grpreq.set_proposerid(p.userID());
    grpreq.set_recombinationthreshold(m);
    grpreq.set_maxnumberofplayers(n);
    os << TSMessageDefs::TS_CREATE_GROUP_REQUEST;
    if(!grpreq.SerializeToOstream(&os)){
        throw std::runtime_error ("Could not serialize create threshold group message" );
    }
    return ;
}

void createThresholdGroupResponse(const player&, const std::string& grpid, const bool& grpset, std::ostream& os){
    thresholdsignature::CreateThresholdGroupResponse grpRsp; 
    grpRsp.set_groupset(grpset);
    grpRsp.set_grpid(grpid);
    os << TSMessageDefs::TS_CREATE_GROUP_RESPONSE;
    if(!grpRsp.SerializeToOstream(&os)){
        throw std::runtime_error( "Unabe to serialize a create threshold group response" );
    }
    return;
}
void createInviteToGroupMessage(const std::string& proposer, const std::string& grpid, std::ostream& os){
    thresholdsignature::InviteToGroupRequest inviteReq;
    inviteReq.set_proposeruserid(proposer);
    inviteReq.set_groupid(grpid);
    os << TSMessageDefs::TS_INVITE_TO_GROUP_REQUEST;
    if (!inviteReq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize invite to threshold group message");
    }
    return ; 
}
 
void createInviteToGroupResponse(const std::string& grpid, const std::string& playerid, const std::string& uri, const std::string& addr, const std::string& port, const bool& accepted, std::ostream& os){
    thresholdsignature::InviteToGroupResponse inviteResp;
    inviteResp.set_userid(playerid);
    inviteResp.set_groupid(grpid);
    inviteResp.set_useruri(uri);
    inviteResp.set_userip(addr);
    inviteResp.set_userport(port);
    inviteResp.set_acceptinvite(accepted); 
    os << TSMessageDefs::TS_INVITE_TO_GROUP_RESPONSE;
    if (!inviteResp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize invite to threshold group message");
    }
    return ; 
}

void createBroadCastGroupDetails(const GroupMetadata& grp, std::ostream& os){
    thresholdsignature::ThresholdGroup tsgrp;
    tsgrp.set_grpid(grp.groupid());
    tsgrp.set_proposer(grp.proposer());
    tsgrp.set_m(grp.m());
    tsgrp.set_n(grp.n());
    tsgrp.set_t(grp.t());
    
    
    int ordinalChoice(1);
    for(std::vector<playerCommsInfo>::const_iterator iter = grp.participantList().begin(); iter != grp.participantList().end(); ++iter){
        thresholdsignature::ThresholdGroup::GroupMember* tsgrpmem = tsgrp.add_member();
        tsgrpmem->set_userid(iter->m_userid);
        tsgrpmem->set_useruri(iter->m_uri);
        tsgrpmem->set_userip(iter->m_ip);
        tsgrpmem->set_userport(iter->m_port);
        tsgrpmem->set_ordinal(ordinalChoice++);
    }
    os << TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_REQUEST ; 
    if(!tsgrp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize broadcast group details");
    }
    return;
}

void createBroadCastGroupDetailsResponse(const std::string& grpid, const std::string& playerid, const bool& userset, std::ostream& os){
    thresholdsignature::ThresholdGroupResponse resp;
    resp.set_groupid (grpid); 
    resp.set_userid (playerid);
    resp.set_userset(userset);
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
    thresholdsignature::SecretSharingRequest req;
    req.set_userid(p.userID());
    req.set_groupid(grpid);
    req.set_calctype(calctype);
    os << TSMessageDefs::TS_CREATE_GROUP_SECRET_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingRequest message for grp id grpid" + grpid); 
    }
    return ; 
}

void createSecretSharingResponse(const player& p, const std::string& grpid, const std::string& calctype, const bool& sharedsecret, std::ostream& os){

    thresholdsignature::SecretSharingResponse resp;
    resp.set_userid(p.userID());
    resp.set_groupid(grpid);
    resp.set_calctype(calctype);
    // figure out a way to pass success around
    resp.set_sharedsecret(sharedsecret);
    os << TSMessageDefs::TS_CREATE_GROUP_SECRET_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingResponse message for group id" + grpid ); 
    }
    return;
}

void createSecretSharingInitEvalRequest(const std::string& proposer, const std::string& grpid, const std::string& calctype, std::ostream& os){
    thresholdsignature::InitEvalSharingRequest req;
    req.set_userid(proposer);
    req.set_groupid(grpid);
    req.set_calctype(calctype);
    os << TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SecretSharingInitEvalRequest for group id" + grpid);
    }
    return;
}

void createSecretSharingInitEvalResponse(const std::string& userid, const std::string& grpid, const std::string& calctype, const bool& evalShareComplete, std::ostream& os){
    thresholdsignature::InitEvalSharingResponse resp;
    resp.set_userid(userid);
    resp.set_groupid(grpid);
    resp.set_calctype(calctype);
    resp.set_evalsshared(evalShareComplete);
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
    thresholdsignature::EvalSharingRequest req;
    req.set_userid(userid);
    req.set_groupid(grpid);
    req.set_fromord(std::to_string(ord));
    req.set_eval(evalIter->second.ToHex());
    os << TSMessageDefs::TS_CREATE_PRIVATE_EVAL_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PrivateDataEvalRequest for player " + std::to_string(ord) );
    }
    return;
}

void createPrivateDataEvalResponse(const std::string& userid, const std::string& grpid, const bool& evaladded, std::ostream& os){
    thresholdsignature::EvalSharingResponse resp;
    resp.set_userid(userid);
    resp.set_groupid(grpid);
    resp.set_evalreceived(evaladded);
    os << TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PrivateDataEvalResponse for player " + userid );
    }
    return;
}
void createSecretSharingPlayerDataRequest(const std::string& p, const std::string& grpid, const std::string& calctype, std::ostream& os){
    thresholdsignature::SecretSharingPlayerDataRequest req;
    req.set_userid(p);
    req.set_groupid(grpid);
    req.set_calctype(calctype);
    os << TSMessageDefs::TS_CREATE_PLAYER_SECRET_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingPlayerDataRequest for player p " + p + "and group id: " + grpid);
    }
    return;
}
void createSecretSharingPlayerDataResponse(const std::string& p, const std::string& grpid, const std::string& calctype , const int& ord, const jvrss& secrets, std::ostream& os){

    
    thresholdsignature::SecretSharingPlayerDataResponse resp;
    resp.set_userid(p);
    resp.set_groupid(grpid);
    resp.set_calctype(calctype);
    resp.set_ordinal(ord);
    
    // if the calculation type != PRIVATEKEYSHARE, regenerate the polynomial,evals, etc.
    
    //for(std::map<std::string, BigNumber>::const_iterator evalIter = secrets.m_evals.begin(); evalIter != secrets.m_evals.end(); ++evalIter){
    //    thresholdsignature::stringPair* evals = resp.add_evals();
    //    evals->set_ordinal(evalIter->first);
    //    evals->set_eval(evalIter->second.ToHex());
    //}
    
         
    for(std::map<std::string, std::vector<std::pair<std::string,ECPoint> > >::const_iterator hiddenEvalIter = secrets.m_hiddenEvals.begin();
            hiddenEvalIter != secrets.m_hiddenEvals.end(); ++ hiddenEvalIter){
        thresholdsignature::KeyStringAndValueListStringPair* ptrHiddenEvals = resp.add_hiddenevals();
        ptrHiddenEvals->set_key(hiddenEvalIter->first);
        for(std::vector<std::pair<std::string, ECPoint> >::const_iterator ecptIter = hiddenEvalIter->second.begin(); ecptIter != hiddenEvalIter->second.end(); ++ ecptIter){
            thresholdsignature::stringPair* ecpt = ptrHiddenEvals->add_value();
            ecpt->set_ordinal(ecptIter->first);
            ecpt->set_eval(ecptIter->second.ToHex());
        }
    }
    
    
    thresholdsignature::HiddenPoly* ptrPoly = resp.add_hiddenpoly();
    for(std::vector<ECPoint>::const_iterator hiddenPolyIter =  secrets.m_hiddenPolynomial.begin();
            hiddenPolyIter != secrets.m_hiddenPolynomial.end(); ++hiddenPolyIter){
        thresholdsignature::singleString* ptrToElement = ptrPoly->add_val();
        ptrToElement->set_val(hiddenPolyIter->ToHex());
    }
    
    os << TSMessageDefs::TS_CREATE_PLAYER_SECRET_RESPONSE ; 
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise SecretSharingPlayerDataResponse for player " + p + " and group " + grpid);
    }
    
    
    return;
}

void createSecretSharingCollatedDataRequest(const std::string& p, const std::string& grpid, const std::string& calctype, const GroupMetadata& grp, std::ostream& os){
    thresholdsignature::SecretSharingCollatedPlayerRequest ssCollatedDataRequest;
    ssCollatedDataRequest.set_userid(p);
    ssCollatedDataRequest.set_groupid(grpid);
    ssCollatedDataRequest.set_calctype(calctype);
    
    //thresholdsignature::KeyStringAndValueListStringPair* evalsPtr = ssCollatedDataRequest.add_collatedevals();
    //for(std::map<std::string, std::vector<std::pair<std::string, BigNumber> > >::const_iterator iter = grp.collatedEvals().begin(); iter != grp.collatedEvals().end(); ++iter){
    //    thresholdsignature::KeyStringAndValueListStringPair* evalsPtr = ssCollatedDataRequest.add_collatedevals();
    //    evalsPtr->set_key(iter->first);
    //    for(std::vector<std::pair<std::string,BigNumber> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++ innerIter){
    //        thresholdsignature::stringPair* ptrValue = evalsPtr->add_value();
    //        ptrValue->set_ordinal(innerIter->first);
    //        ptrValue->set_eval(innerIter->second.ToHex());
    //    }
    //}
    
    //thresholdsignature::KeyStringAndValueListRepeatedString* hiddenPolyPtr = ssCollatedDataRequest.add_collatedhiddenpolys();
    for(std::map<std::string, std::vector<ECPoint> >::const_iterator iter = grp.CollatedHiddenPolys().begin(); iter != grp.CollatedHiddenPolys().end(); ++ iter){
        thresholdsignature::KeyStringAndValueListRepeatedString* hiddenPolyPtr = ssCollatedDataRequest.add_collatedhiddenpolys();
        hiddenPolyPtr->set_key(iter->first);
        for(std::vector<ECPoint>::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++innerIter){
            thresholdsignature::singleString* ptrValue = hiddenPolyPtr->add_value();
            ptrValue->set_val(innerIter->ToHex());
        }
    }
    //thresholdsignature::KeyStringAndValueListStringPair* collatedHiddenEvalsPtr = ssCollatedDataRequest.add_collatedhiddenevals();
    for(std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::const_iterator iter = grp.CollatedHiddenEvals().begin(); iter != grp.CollatedHiddenEvals().end(); ++ iter){
        thresholdsignature::KeyStringAndValueListStringPair* collatedHiddenEvalsPtr = ssCollatedDataRequest.add_collatedhiddenevals();
        collatedHiddenEvalsPtr->set_key(iter->first); 
        for(std::vector<std::pair<std::string,ECPoint> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++ innerIter){
            thresholdsignature::stringPair* ptrValue = collatedHiddenEvalsPtr->add_value();
            ptrValue->set_ordinal(innerIter->first);
            ptrValue->set_eval(innerIter->second.ToHex());
        }
    }
    
    os << TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_REQUEST;
    if(!ssCollatedDataRequest.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SecretSharingCollatedDataRequest for player " + p + " and group " + grpid);
    }
    return ; 
}

void createSecretSharingCollatedPlayerResponse(const std::string& p, const std::string& grpid, const std::string& calctype,std::ostream& os){
    thresholdsignature::SecretSharingCollatedPlayerResponse ssCollatedDataResp; 
    ssCollatedDataResp.set_userid(p);
    ssCollatedDataResp.set_groupid(grpid);
    ssCollatedDataResp.set_calctype(calctype);
    ssCollatedDataResp.set_collatedplayerdatavalidated(true);
    os << TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_RESPONSE;
    if(!ssCollatedDataResp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SecretSharingCollatedPlayerResponse for player " + p + " and group " + grpid);
    }
    
    return ; 
}

void createEphemeralKeyDataRequest(const std::string& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::EphemeralKeyDataRequest emphemeraldatareq;
    emphemeraldatareq.set_userid(p);
    emphemeraldatareq.set_groupid(grpid);
    os << TSMessageDefs::TS_CREATE_PRE_SIG_REQUEST;
    if(!emphemeraldatareq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize EphemeralKeyDataRequest for player " + p + " and group " + grpid);
    }
    return;
}

void createEphemeralKeyDataResponse(const std::string& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::EphemeralKeyDataResponse ekeyplayerdataresp;
    ekeyplayerdataresp.set_userid(p);
    ekeyplayerdataresp.set_groupid(grpid);
    ekeyplayerdataresp.set_emphemeralkeydataset(true);
    
    os << TSMessageDefs::TS_CREATE_PRE_SIG_RESPONSE;
    if(!ekeyplayerdataresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize EphemeralKeyDataResponse for player " + p + " and group " + grpid);
    }
    return ;
}

void createEphermalKeyPlayerDataRequest(const std::string& p, const std::string& grpid, std::ostream& os){
    thresholdsignature::EphemeralKeyPlayerDataRequest ekeyplayerdatareq;
    ekeyplayerdatareq.set_userid(p);
    ekeyplayerdatareq.set_groupid(grpid);
    os << TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_REQUEST;
    if(!ekeyplayerdatareq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize EphemeralKeyPlayerDataRequest for player " + p + " and group " + grpid);
    }
    return ; 
}



void createEphemeralKeyPlayerDataResponse(const std::string& p, const std::string & grpid, const int& ordinal, const std::pair<BigNumber, ECPoint>& ekey, std::ostream& os){
    thresholdsignature::EphemeralKeyPlayerDataResponse ekyplayerdataresp;
    ekyplayerdataresp.set_userid(p);
    ekyplayerdataresp.set_groupid(grpid);
    ekyplayerdataresp.set_ordinal(ordinal);
    ekyplayerdataresp.set_kalpha(ekey.first.ToHex());
    ekyplayerdataresp.set_alphag(ekey.second.ToHex());
    os << TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_RESPONSE;
    if(!ekyplayerdataresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize EphemeralKeyPlayerDataResponse for player " + p + " and group " + grpid);
    }
    return ; 
}

void createEphemeralKeyCollatedDataRequest(const std::string& p, const std::string& grpid, const GroupMetadata& grp, std::ostream& os){
    thresholdsignature::AllEphemeralKeyPlayerRequest ekeyCollatedReq;
    ekeyCollatedReq.set_groupid(grpid);
    for(std::map<std::string, std::pair<BigNumber, ECPoint> >::const_iterator iter = grp.CollatedVW().begin(); iter != grp.CollatedVW().end(); ++ iter){
        thresholdsignature::OrdinalVW* ptrOrdVW = ekeyCollatedReq.add_collatedvws();

        ptrOrdVW->set_ordinal(iter->first);
        ptrOrdVW->set_v(iter->second.first.ToHex());
        ptrOrdVW->set_w(iter->second.second.ToHex());
    }
    os << TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_REQUEST;
    if(!ekeyCollatedReq.SerializeToOstream(&os)){
       throw std::runtime_error("could not serialize AllEphemeralKeyPlayerRequest for player " + p + " and group " + grpid);
    }
    
    return ; 
}

void createAEphemeralKeyCollatedDataResponse(const std::string& p, const std::string& grpid, const std::string& ordinal, std::ostream& os){
    thresholdsignature::AllEphemeralKeyPlayerResponse ekeyCollatedResp;
    ekeyCollatedResp.set_userid(p);
    ekeyCollatedResp.set_groupid(grpid);
    ekeyCollatedResp.set_ordinal(ordinal);
    // need a way to track & report failures
    ekeyCollatedResp.set_ephemeralkeyplayerset(true);
    os << TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_RESPONSE;
    if(!ekeyCollatedResp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize AllEphemeralKeyPlayerResponse for player " + p + " and group " + grpid); 
    }
    return ;
}

void createSignatureRequest(const std::string& p, const std::string& grpid, const std::string& msg, const int& eKeyIndex, std::ostream& os){
    thresholdsignature::GenerateSignatureRequest sigreq;
    sigreq.set_userid(p);
    sigreq.set_groupid(grpid);
    sigreq.set_msg(msg);
    sigreq.set_emphemeralkeyindex(eKeyIndex);
    os << TSMessageDefs::TS_CREATE_GROUP_SIG_REQUEST;
    if(!sigreq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GenerateSignatureRequest for player " + p + " and group " + grpid);
    }
    return ;
}

void createSignatureResponse(const std::string& p, const std::string& grpid, const GroupMetadata& grp , const std::string& msg, const int& ekeyindex,std::ostream& os){
    thresholdsignature::GenerateSignatureResponse sigresp;  
    sigresp.set_userid(p);
    sigresp.set_groupid(grpid);
    sigresp.set_msg(msg);
    sigresp.set_emphemeralkeyindex(ekeyindex);
    for(std::vector<std::pair<std::string, BigNumber> >::const_iterator iter = grp.CollatedPartialSignatures().begin(); iter != grp.CollatedPartialSignatures().end(); ++iter){
        thresholdsignature::stringPair* siginfo = sigresp.add_partialsignatures();
        siginfo->set_ordinal(iter->first);
        siginfo->set_eval(iter->second.ToHex());
    }
    os << TSMessageDefs::TS_CREATE_GROUP_SIG_RESPONSE;
    if(!sigresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialise GenerateSignatureResponse for player " + p + " and group " + grpid);
    }
    return;
}

void createSignaturePlayerDataRequest(const std::string& p, const std::string& grpid, const std::string& themessage, const int& ekeyIndex, std::ostream& os){
    thresholdsignature::SignaturePlayerDataRequest playersigreq;
    playersigreq.set_userid(p);
    playersigreq.set_groupid(grpid);
    playersigreq.set_msg(themessage);
    playersigreq.set_emphemeralkeyindex(ekeyIndex);
    os << TSMessageDefs::TS_CREATE_SIG_PLAYER_REQUEST;
    if(!playersigreq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SignaturePlayerDataRequest for player " + p + " and group " + grpid);
    }
    return ; 
}

void createSignaturePlayerDataResponse(const std::string& p, const std::string& grpid, const std::pair<std::string, BigNumber>& siginfo, const int& ekeyindex,std::ostream& os){
    thresholdsignature::SignaturePlayerDataResponse playersigresp;
    playersigresp.set_userid(p);
    playersigresp.set_groupid(grpid);
    playersigresp.set_ordinal(siginfo.first);
    playersigresp.set_emphemeralkeyindex(ekeyindex);
    playersigresp.set_signatureshare(siginfo.second.ToHex());
    os << TSMessageDefs::TS_CREATE_SIG_PLAYER_RESPONSE;
    if(!playersigresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize SignaturePlayerDataResponse for player " + p + " and group " + grpid);
    }
    return ;
}

void createPrivateKeyRequest(const std::string& p,  const std::string& grpid, const std::vector<std::string>& users, std::ostream& os){
    thresholdsignature::GenerateGroupPrivateKeyRequest priKeyReq; 
    priKeyReq.set_userid(p);
    priKeyReq.set_groupid(grpid);
    for(std::vector<std::string>::const_iterator iter = users.begin(); iter != users.end(); ++ iter){
        thresholdsignature::singleString* pinfo = priKeyReq.add_players();
        pinfo->set_val(*iter);
    }
    os << TSMessageDefs::TS_CREATE_PRIKEY_REQUEST;
    if(!priKeyReq.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GenerateGroupPrivateKeyRequest for player " + p + " and group " + grpid);
    }
    return;
}

void createPrivateKeyResponse(const std::string& p, const std::string& grpid, const bool& keyshared, std::ostream& os){
    thresholdsignature::GenerateGroupPrivateKeyResponse prikeyresp;
    prikeyresp.set_userid(p);
    prikeyresp.set_groupid(grpid);
    prikeyresp.set_privatedatashared(keyshared);
    os << TSMessageDefs::TS_CREATE_PRIKEY_RESPONSE;
    if(!prikeyresp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize GenerateGroupPrivateKeyResponse for player " + p + " and group " + grpid);
    }
    return;
}
 
 void createPlayerPrivateShareExchangeRequest(const std::string& p, const std::string& grpid, std::ostream& os){
     thresholdsignature::PlayerPrivateShareExchangeRequest req;
     req.set_userid(p);
     req.set_groupid(grpid);
     os << TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_REQUEST;
     if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateShareExchangeRequest for player " + p + " and group " + grpid);
    }
    return;
 }

 void createPlayerPrivateShareExchangeResponse(const std::string& p, const std::string& grpid, const bool& keyshared, std::ostream& os){
     thresholdsignature::PlayerPrivateShareExchangeResponse resp;
     resp.set_userid(p);
     resp.set_groupid(grpid);
     resp.set_privatedatashared(keyshared);
     os << TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_RESPONSE ;
     if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateShareExchangeResponse for player " + p + " and group " + grpid);
    }
    return;

 }

 void createPlayerPrivateKeyShareRequest(const playerGroupMetaData& grpinfo,std::ostream& os){
    thresholdsignature::PlayerPrivateKeyShareRequest req;

    req.set_userid(getPublicPlayerInfo().userID());
    req.set_groupid(grpinfo.m_id);
    req.set_ordinal(std::to_string(grpinfo.m_ordinal));
    req.set_privatekeyshare(grpinfo.privateKeyShare().ToHex());
    os << TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_REQUEST;
    if(!req.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateKeyShareRequest for player " + getPublicPlayerInfo().userID() + " and group " + grpinfo.m_id);
    }
    return;
 }

void createPlayerPrivateKeyShareResponse(const std::string& userid, const std::string& grpid, const bool& sharerec, std::ostream& os){
    thresholdsignature::PlayerPrivateKeyShareResponse resp;
    resp.set_userid(userid);
    resp.set_groupid(grpid);
    resp.set_sharedprivatekeyshare(sharerec);
    os << TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_RESPONSE;
    if(!resp.SerializeToOstream(&os)){
        throw std::runtime_error("could not serialize PlayerPrivateKeyShareResponse for player " + userid + " and group " + grpid);
    }
    return;
}

