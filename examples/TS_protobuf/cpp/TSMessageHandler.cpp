#include <TSMessageHandler.h>
#include <player.h>
#include <GroupMetaData.h>
#include <orchestrator.h>
#include <random>
#include <MessageRequestResponse.h>

std::string enum2string( thresholdsignature::CalcType calculation ) 
{
    if( calculation == thresholdsignature::PRIVATEKEYSHARE )
        return "PRIVATEKEYSHARE" ;
    else if( calculation == thresholdsignature::LITTLEK )
        return  "LITTLEK" ;
    else if( calculation == thresholdsignature::ALPHA )
        return "ALPHA" ;
    else
    {
        std::cout << "ERROR: unknown calculation: " << calculation << std::endl ;
        throw ;
    }
}

bool HandleMessage(MessageDescriptor& msgdesc, std::istream& is){
    
    std::cout << "HandleRequestMessage for message id  " << msgdesc.m_EnumMSGID << std::endl;

        
    if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_REGISTER_PLAYER_REQUEST){
        thresholdsignature::RegisterRequest registerreq;
        if(!registerreq.ParseFromIstream(&is)){
            std::cout << "Unable to read a resiger player request message from the input stream" << std::endl;
            return false; 
        }
        thresholdsignature::Player tsplayer = registerreq.playerid();
        
        player p (tsplayer.name(), tsplayer.uri(), tsplayer.addr(), tsplayer.port()); 
        addPlayer(p); 
    }else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_REGISTER_PLAYER_RESPONSE){
        thresholdsignature::RegisterReply resp;
        if(!resp.ParseFromIstream(&is)){
         std::cout << "Unable to load jack to response" << std::endl;
        }
        if (!resp.success()){
            std::cout << "Unable to join the group" << std::endl; 
        }
    }else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_LIST_PLAYER_REQUEST){
        thresholdsignature::GetPlayerListRequest req; 
        if(!req.ParseFromIstream(&is)){
            std::cout << "unable to read playerlist request from the input stream" << std::endl;
            return false; 
        }
        std::cout << "Received player list request from " << req.name() << std::endl;
        return true;
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_LIST_PLAYER_RESPONSE){
        thresholdsignature::GetPlayerListResponse resp;
        if(!resp.ParseFromIstream(&is)){
            std::cout << "unable to read playerlist response from the input stream" << std::endl;
            return false; 
        }
        for (int j = 0; j < resp.playerid_size(); j++) {
            const thresholdsignature::Player& tsplayer = resp.playerid(j);
            player p(tsplayer.name(), tsplayer.uri(), tsplayer.addr(), tsplayer.port());
            std::cout << p << std::endl;
        }

    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_REQUEST){
        thresholdsignature::CreateGroupRequest grpreq;
        if(!grpreq.ParseFromIstream(&is)){
            std::cout << "Unable to read create threshold group request from the input stream" << std::endl;
            return false;
        }
        std::string proposer = grpreq.userid();
        int m = grpreq.m();
        int n = grpreq.n();
        std::string grpid; 
        try{
            grpid = CreateGroup(proposer, m,n);
        }
        catch(std::exception& e){
            std::cout << "Unable to create threshold group " << e.what() << std::endl;
            return false;
        }
        // send group invites to the users, set up the group as the responses come back
        SendGroupInvites(proposer,grpid);
        
        // BroadGroupDetails
        SendGroupDetails(grpid);
        
        // return from this method to a createGroupResponse can be sent back to the proposer
        msgdesc.m_grpID = grpid;
        
        return true; 
        
        
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_INVITE_TO_GROUP_REQUEST){
        // handle an invite request
        thresholdsignature::InviteRequest req; 
        if(!req.ParseFromIstream(&is)){
            std::cout << "unable to read playerlist request from the input stream" << std::endl;
            return false; 
        }
        //
        std::cout << "Received invite to grp id" <<  req.id().groupid() << std::endl;
        msgdesc.m_grpID = req.id().groupid();
        return true;
        
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_INVITE_TO_GROUP_RESPONSE){
        // handle a group response
        thresholdsignature::InviteReply resp; 
        if(!resp.ParseFromIstream(&is)){
            std::cout << "unable to read inviteToGroupResponse from the input stream" << std::endl;
            return false;
        }
        std::cout << "Received invite to grp response from: " << resp.id().userid() << " for group " << resp.id().groupid() << std::endl;
        if(resp.acceptance())
            addUserToGroup(resp.id().groupid(),resp.id().userid() );
        
        msgdesc.m_grpID = resp.id().groupid();
        
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_REQUEST){
 
        thresholdsignature::GroupIsSetRequest tsreq ;
        if(!tsreq.ParseFromIstream(&is)){
            std::cout << "Unable to read GroupIsSetRequest message from input stream" << std::endl;
        }
        // hard code for now
        BigNumber mod;
        mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
        playerGroupMetaData playerGrp (tsreq.degree(), mod);
        
        playerGrp.m_id      = tsreq.groupid();

        for (int j = 0; j < tsreq.participants_size(); j++) {
            const thresholdsignature::GroupIsSetRequest::Participant& tsplayer = tsreq.participants(j);
            if(tsplayer.playerid().name() ==  SinglePlayer::Instance()->UserID()){
                playerGrp.m_ordinal = tsplayer.ordinal();
            }else
            {
                player p 
                    (
                        tsplayer.playerid().name(),
                        tsplayer.playerid().uri(),
                        tsplayer.playerid().addr(),
                        tsplayer.playerid().port()
                    ); 
                playerGrp.m_ordinalList.push_back( tsplayer.ordinal() ) ;            
                playerGrp.m_ordinalAndPlayerList.push_back(std::make_pair(tsplayer.ordinal(),p));
            }
        }

                    
        std::cout << "TSMessageFactory..Handling broadcast group details message\n"
                    << "\n\tGroup ID : " << playerGrp.m_id
                    << "\n\tPlayer ordinal : " << playerGrp.m_ordinal
                    << "\n\t player ordinal list: "
                    ;
        for( auto iter : playerGrp.m_ordinalList ) 
        {
            std::cout << iter << " " ;
        }
        
        
        std::cout << std::endl; 
        playerGrp.polynomialPreCalculation(playerGrp.m_privateKeyPolynomial);
        
        SinglePlayer::Instance()->addPlayerGroup(playerGrp);
        msgdesc.m_grpID  = tsreq.groupid();
        msgdesc.m_userID = SinglePlayer::Instance()->UserID();
        std::cout << "Finished with TS_BROADCAST_GROUP_DETAILS_REQUEST" << std::endl ;
       
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_RESPONSE){
        thresholdsignature::GroupIsSetReply resp;
        if(!resp.ParseFromIstream(&is)){
            std::cout << "unable to read groupIsSet response from the input stream" << std::endl;
        }
        std::cout << "received groupIsSet response for group " << resp.id().groupid()  << " from " << resp.id().userid() << std::endl;
    }else if(msgdesc.m_EnumMSGID  == TSMessageDefs::TS_CREATE_GROUP_RESPONSE){
        thresholdsignature::CreateGroupReply tsgrpResp; 
        if(!tsgrpResp.ParseFromIstream(&is)){
            std::cout << "unable to read threshold group response from the input stream" << std::endl;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_DELETE_GROUP_REQUEST){
        thresholdsignature::DeleteGroupRequest delgrpreq;
        if(!delgrpreq.ParseFromIstream(&is)){
            std::cout << "unable to read delete group request from the input stream" << std::endl;
        }
        // send delete grp messages to all players
        if(!SendGroupDelete(delgrpreq.userid(), delgrpreq.groupid())){
            std::cout << "Failed to send group delete request to all players" << std::endl;
        }
        
        // delete the group from the server & response back to the requestor that the group has been deleted. 
        if(!deleteGroup(delgrpreq.groupid())){
            std::cout << "Unable to delete group " << delgrpreq.groupid() << std::endl;
        }
        msgdesc.m_userID = delgrpreq.userid();
        msgdesc.m_grpID = delgrpreq.groupid(); 
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_DELETE_GROUP_RESPONSE){
        thresholdsignature::DeleteGroupResonse delgrpResp;
        if(!delgrpResp.ParseFromIstream(&is)){
            std::cout << "failed to read delete group response from the input stream:" << std::endl;
            return false;
        }
        try{
            if(delgrpResp.groupdeleted())
                deletePlayerGroup(delgrpResp.userid(), delgrpResp.groupid()); 
        }
        catch(std::exception& e){
            std::cout << "Cannot delete local group " << delgrpResp.groupid() << "for player " << delgrpResp.userid()  
                       <<  e.what() << std::endl;
            return false;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_DELETE_PLAYER_GROUP_REQUEST){
        thresholdsignature::DeleteGroupPlayerRequest delgrpPlayerReq; 
        if(!delgrpPlayerReq.ParseFromIstream(&is)){
            std::cout << "Unable to stream deleteGroupPlayerRequest from the input stream" << std::endl;
            return false; 
        }
        msgdesc.m_userID = delgrpPlayerReq.userid();
        msgdesc.m_grpID = delgrpPlayerReq.groupid();
        
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_DELETE_PLAYER_GROUP_RESPONSE){
        thresholdsignature::DeleteGroupPlayerResponse delgrpPlayerResp; 
        // remove the player who responsed from the grp
        if(!delgrpPlayerResp.ParseFromIstream(&is)){
            std::cout << "Unable to stream deleteGroupPlayerRequest from the input stream" << std::endl;
            return false; 
        }
        if(delgrpPlayerResp.deleted())
        {
            std::cout << " player " << msgdesc.m_userID << " has delete the group " << std::endl;
        }
    } else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SECRET_REQUEST){
        thresholdsignature::ShareSecretDataRequest secretSharingReq; 
        if(!secretSharingReq.ParseFromIstream(&is)){
            std::cout << "Unable to stream SecretSharingRequest from the input stream" << std::endl;
            return false;
        }
        // for a list of players in the group
        // send a ShareSecretDataRequest
        // lock the group
        lockGrp(secretSharingReq.id().groupid());

        std::string calcType = enum2string( secretSharingReq.calculation() ) ;
        // ask each player to share the evals with the relevant players
        if(!ShareEvalsPlayers
            (
                secretSharingReq.id().userid(),
                secretSharingReq.id().groupid(),
                calcType
            ))
        {
            std::cout << "Player sharing of evals failed" << std::endl;
            return false;
        }

        if(!SecretSharingPlayerDataRequest
            (
                secretSharingReq.id().userid(),
                secretSharingReq.id().groupid(),
                calcType
            ))
        {
            std::cout << "Unable to send Player Data request all group members" << std::endl;
            return false;
        }
        
        // build the collated data
        // send SecretSharingCollatedPlayerRequest to each player
        if(!SecretSharingCollatedDataRequest
            (
                secretSharingReq.id().userid(),
                secretSharingReq.id().groupid(),
                calcType
            ))
        {
            std::cout << "Unable to send collated player data request to all group members" << std::endl;
            return false;
        }
        // return so the system can send a SecretSharingResponse to the original requestor
        msgdesc.m_userID    = secretSharingReq.id().userid();
        msgdesc.m_grpID     = secretSharingReq.id().groupid();
        msgdesc.m_SSType    = calcType ;
        //clear the data sharing containers in GroupMetaData
        if(!DeleteGrpSharingContainers(secretSharingReq.id().groupid())){
            std::cout   << "Could not clear up JVRSS Public shared data for group id " 
                        << secretSharingReq.id().groupid() << std::endl;
            return false;
        }
        // unlock the groupx
        unlockGrp(secretSharingReq.id().groupid());

    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SECRET_RESPONSE){
        // acknowledge that the secret has been shared
        thresholdsignature::ShareSecretReply secretSharingResp;
        if(!secretSharingResp.ParseFromIstream(&is)){
            std::cout << "Unable to parse a secret sharing response from the input stream" << std::endl;
            return false;
        }

        std::string calcType = enum2string( secretSharingResp.calculation() ) ;
        
        msgdesc.m_userID    = secretSharingResp.id().userid();
        msgdesc.m_grpID     = secretSharingResp.id().groupid();
        msgdesc.m_SSType    = calcType;
        
        if(secretSharingResp.success()){
            std::cout << "Secret of type " << calcType << " has been shared by the group " << msgdesc.m_grpID << std::endl;
        }
    } else if ( msgdesc.m_EnumMSGID == TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_REQUEST){
        // 
        thresholdsignature::InitShareEvalsRequest initPrivateEvalReq; 
        if(!initPrivateEvalReq.ParseFromIstream(&is)){
            std::cout << "Unable to parse a secret sharing init private eval exchange from the input stream" << std::endl;
            return false;
        }
        std::cout << "Initial private eval sharing request" << std::endl;
        if  ( !SharePrivateEvalsRequest
                (
                    initPrivateEvalReq.id().userid(),
                    initPrivateEvalReq.id().groupid(),
                    enum2string( initPrivateEvalReq.calculation() )
                )
            )
        {
            std::cout << "Unable to send Player Data request all group members" << std::endl;
            return false;
        }
        msgdesc.m_userID    = initPrivateEvalReq.id().userid();
        msgdesc.m_grpID     = initPrivateEvalReq.id().groupid();
        msgdesc.m_SSType    = enum2string( initPrivateEvalReq.calculation() );
        // 
    } else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_RESPONSE){
        thresholdsignature::InitShareEvalsReply initPrivateEvalResp; 
        if(!initPrivateEvalResp.ParseFromIstream(&is)){
            std::cout << "Unable to parse a secret sharing init private eval exchange from the input stream" << std::endl;
            return false;
        }
        if(initPrivateEvalResp.success()){
            std::cout << "All evals successfully shared " << std::endl;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIVATE_EVAL_REQUEST){
        thresholdsignature::DistributeEvalsRequest evalsharingReq; 
        if(!evalsharingReq.ParseFromIstream(&is)){
            std::cout << "Unable to parse a secret sharing share private eval message from the input stream" << std::endl;
            return false;
        }
        msgdesc.m_userID = getPublicPlayerInfo().userID();
        msgdesc.m_grpID = evalsharingReq.id().groupid();
        playerGroupMetaData& grp = PlayerGrpData (evalsharingReq.id().groupid());
        grp.addPublicEvalsToJVRSS
            (
                std::to_string(evalsharingReq.fromordinal()), 
                evalsharingReq.f_x()
            ); 

    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIVATE_EVAL_RESPONSE){
        thresholdsignature::DistributeEvalsReply evalsharingResp; 
        if(!evalsharingResp.ParseFromIstream(&is)){
            std::cout << "Unable to parse a secret sharing share private eval response from the input stream" << std::endl;
            return false;
        }
        if(evalsharingResp.success()){
            std::cout   << "Eval successfully received by " 
                        << evalsharingResp.id().userid() 
                        << " from the group " 
                        << evalsharingResp.id().groupid() 
                        << std::endl;
        }
        msgdesc.m_userID = evalsharingResp.id().userid();
        msgdesc.m_grpID = evalsharingResp.id().groupid();
    }else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_SECRET_REQUEST){
        thresholdsignature::ShareSecretDataRequest req;
        if(!req.ParseFromIstream(&is)){
            std::cout << "Unable to read secret sharing player data request from input stream" << std::endl;
            return false;
        }
        msgdesc.m_userID    = req.id().userid();
        msgdesc.m_grpID     = req.id().groupid();
        msgdesc.m_SSType    = enum2string( req.calculation() ); 
    } else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_SECRET_RESPONSE){
        thresholdsignature::ShareSecretDataReply ssPlayerDataResponse;
        if(!ssPlayerDataResponse.ParseFromIstream(&is)){
            std::cout << "Unable to read Secret Sharing Player Data Response from the input stream" << std::endl;
               return false;
        }
        //  parse & load the GroupMetaData components
        GroupMetadata& grp = GetGroup(ssPlayerDataResponse.id().groupid());
        grp.calculationType () = enum2string( ssPlayerDataResponse.calculation()) ;
        std::string ord = std::to_string(ssPlayerDataResponse.ordinal());
        
        
        // decode Hidden Polynomials
        std::vector<std::string> poly;
        for( int j = 0; j<ssPlayerDataResponse.hiddenpoly_size(); ++j )
        {
            poly.push_back(ssPlayerDataResponse.hiddenpoly(j));
        }
        grp.addCollatedHiddenPolys(ord,poly);

        // decode Hidden Evals 
        for( int j=0; j<ssPlayerDataResponse.hiddenevals_size(); ++j )
        {
            grp.addCollatedHiddenEvals
            (
                ord,
                std::to_string( ssPlayerDataResponse.hiddenevals(j).ordinal() ),
                ssPlayerDataResponse.hiddenevals(j).f_x()
            ) ;
        }
        
        msgdesc.m_userID    = ssPlayerDataResponse.id().userid();
        msgdesc.m_grpID     = ssPlayerDataResponse.id().groupid();
        msgdesc.m_SSType    = enum2string( ssPlayerDataResponse.calculation() ) ;
        
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_REQUEST){
        thresholdsignature::CollatedSecretRequest req ;
        if(!req.ParseFromIstream(&is)){
            std::cout << "Unable to read Collated Secret request" << std::endl;
            return false;
        }     

        // hiddenPolys   
        playerGroupMetaData& playergrp = PlayerGrpData(req.id().groupid());
        for ( int j=0; j<req.hiddenpolys_size(); ++j )
        {
            std::string ordinal = std::to_string( req.hiddenpolys( j ).ordinal( ) ) ;
            //std::cout << "ordinal = " << ordinal << std::endl ;
            std::vector<std::string> polynomial;

            for( int k=0; k<req.hiddenpolys( j ).coefficients_size(); ++k )
            {
                polynomial.push_back( req.hiddenpolys( j ).coefficients( k ) ) ;
                //std::cout << "\tcoefficients = " << req.hiddenpolys( j ).coefficients( k )  << std::endl ;
            }
            playergrp.addHiddenPolynomialToJVRSS(ordinal, polynomial);
        }


        
        // hiddenEvals
        for( int j=0; j<req.hiddenevals_size(); ++j )
        {
            std::string ordinal = std::to_string( req.hiddenevals( j ).ordinal( ) ) ;
            std::cout << "ordinal = " << ordinal << std::endl ;
            std::vector<std::pair<std::string,std::string> > hiddenevals;

            for( int k=0; k<req.hiddenevals( j ).ep_size(); ++k )
            {
                hiddenevals.push_back
                ( 
                    std::make_pair
                    (
                        std::to_string( req.hiddenevals( j ).ep( k ).ordinal() ), 
                        req.hiddenevals( j ).ep( k ).f_x()
                    )
                );
                std::cout << "\tordinal = " << req.hiddenevals( j ).ep( k ).ordinal() << std::endl ;
                std::cout << "\f_x = " << req.hiddenevals( j ).ep( k ).f_x() << std::endl ;
            }
            playergrp.addHiddenEvalsToJVRSS(ordinal,hiddenevals);
        }
        
        // call the verification routines in PlayerGroupMetaData
        
        std::cout << "JVRSS Data after collating data: \n" << playergrp.m_transientData << std::endl;
        try{
            playergrp.verifyCorrectness();
            playergrp.verifyHonesty(); 
        }catch(std::exception& e){
            std::cout << "Unable to share a secret due to verification of honesty or verification of correctness failure: " << e.what() << std::endl;
            return false;
        }
        // 
        
        BigNumber secret = playergrp.createSecret();
        std::string calcType = enum2string( req.calculation( ) ) ;
        std::cout << "Secret Validated & calculated " << secret.ToHex() << std::endl; 
        if( calcType  == "PRIVATEKEYSHARE"){
            playergrp.m_privateKeyShare = secret; 
            playergrp.m_GroupPublicKey = playergrp.createGroupPublicKey();
            std::cout << "Calculated the group public key " << playergrp.m_GroupPublicKey.ToHex() << std::endl;
        }else if( calcType == "LITTLEK"){
            playergrp.m_littleK = secret;
            
        }else if( calcType == "ALPHA"){
            playergrp.m_alpha = secret;
        }

        // delete the jvrss data at this point
        std::cout << "Deleting the JVRSS data set" << std::endl;
        playergrp.m_transientData.reset();
        msgdesc.m_userID    = getPublicPlayerInfo().userID();
        msgdesc.m_grpID     = req.id().groupid();
        msgdesc.m_SSType    = calcType ;

        
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_RESPONSE){
        thresholdsignature::CollatedSecretReply ssCollatedPlayerResp;
        if(!ssCollatedPlayerResp.ParseFromIstream(&is)){
            std::cout << "Unable to read Secret Sharing Collated Player Response from the the stream" << std::endl;
            return false;
        }
        std::string calcType = enum2string( ssCollatedPlayerResp.calculation() ) ;
        if(ssCollatedPlayerResp.success()){
            std::cout 
                << "Player " << ssCollatedPlayerResp.id().userid() 
                << " has validated the data for secret of type " << calcType
                << " with group id " << ssCollatedPlayerResp.id().groupid()
                << std::endl;
        }
        msgdesc.m_userID    = ssCollatedPlayerResp.id().userid();
        msgdesc.m_grpID     = ssCollatedPlayerResp.id().groupid();
        msgdesc.m_SSType    = calcType ;
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_REQUEST){
        // ephemeral key data requeest
        thresholdsignature::InitPresignRequest ekeyDataReq;
        if(!ekeyDataReq.ParseFromIstream(&is)){
            std::cout << "Unable to read ephemeral key data request from the stream" << std::endl;
            return false;
        }
        
         // for a list of players in the group
        // send a EphemeralKeyPlayerDataRequest
        if (!lockGrp(ekeyDataReq.id().groupid())){
            std::cout << "Unable to lock the group with id: " << ekeyDataReq.id().groupid() << std::endl;
            return false;
        }
        if(!EphemeralKeyPlayerDataRequest(ekeyDataReq.id().userid(),ekeyDataReq.id().groupid())){
            std::cout << "Unable to send Player Data request all group members" << std::endl;
            return false;
        }
        // build the collated data
        // send EphemeralKeyollatedPlayerRequest to each player
        if(!EphemeralKeyCollatedPlayerDataRequest(ekeyDataReq.id().userid(),ekeyDataReq.id().groupid())){
            std::cout << "Unable to send collated player data request to all group members" << std::endl;
            return false;
        }
        // return so the system can send a SecretSharingResponse to the original requestor
        msgdesc.m_userID = ekeyDataReq.id().userid();
        msgdesc.m_grpID = ekeyDataReq.id().groupid();
        //clear the data sharing containers in GroupMetaData
        if(!unlockGrp(ekeyDataReq.id().groupid())){
            std::cout << "Unable to unlock the group with id: " << ekeyDataReq.id().groupid() << std::endl;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_RESPONSE){
        // ephermal key data response (last one to do)
        thresholdsignature::InitPresignReply ekeyDataResp;
        if(!ekeyDataResp.ParseFromIstream(&is)){
            std::cout << "Unable to read InitPresignReply from the stream " << std::endl;
            return false;
        }
        if(ekeyDataResp.success()){
            std::cout   << "Ephemeral Key data set for all players in the group " 
                        << ekeyDataResp.id().groupid() << std::endl;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_REQUEST){
        // 
        thresholdsignature::ShareVWDataMessageRequest ekeyPlayerDataReq;
        if(!ekeyPlayerDataReq.ParseFromIstream(&is)){
            std::cout << "Unable to read ephemeral key player data request from the stream" << std::endl;
            return false;
        }
     
        // set the user & grp id
        msgdesc.m_userID = getPublicPlayerInfo().userID();
        msgdesc.m_grpID = ekeyPlayerDataReq.id().groupid();
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_RESPONSE){
        thresholdsignature::ShareVWDataMessage resp;
        if(!resp.ParseFromIstream(&is)){
            std::cout << "Unable to read ephemeral key player data response from the stream" << std::endl;
            return false;
        }
        //  parse & load the GroupMetaData components
        GroupMetadata& grp = GetGroup(resp.id().groupid());
        grp.addCollatedVW
            (
                std::to_string(resp.data().ordinal()), 
                resp.data().v(), 
                resp.data().w()
            );
        msgdesc.m_userID    = resp.id().userid();
        msgdesc.m_grpID     = resp.id().groupid();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_REQUEST){
        thresholdsignature::CollatedVWShareRequest req; 
        if(!req.ParseFromIstream(&is)){
            std::cout << "Unable to read AllEphemeralKeyPlayerRequest from the stream" << std::endl;
            return false; 
        }
        playerGroupMetaData& playergrp = PlayerGrpData(req.id().groupid());
        for(int j = 0; j < req.data_size(); ++j){
            std::string ord = std::to_string( req.data(j).ordinal());
            std::string v = req.data(j).v();
            std::string w = req.data(j).w();
            playergrp.addPreSignDataToJVRSS(ord,v,w);
        }
        // each player interpolates the v curve, w curve 
        playergrp.CalculateEphemeralKey();
        msgdesc.m_userID = getPublicPlayerInfo().userID();
        msgdesc.m_grpID = req.id().groupid();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_RESPONSE){
        thresholdsignature::CollatedVWShareReply resp; 
        if(!resp.ParseFromIstream(&is)){
            std::cout << "Unable to read AllEphemeralKeyPlayerResponse from the stream" << std::endl;
            return false; 
        }
        if(resp.success()){
            std::cout   << "AllEphemeralKeyPlayerResponse success from player " 
                        << resp.id().userid() 
                        << " from group " << resp.id().groupid() << std::endl;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SIG_REQUEST){
        thresholdsignature::InitSignatureRequest sigreq;
        if(!sigreq.ParseFromIstream(&is)){
            std::cout << "Unable to read InitSignatureRequest from the stream" << std::endl;
            return false;
        }

        //lock the group
        if(!lockGrp(sigreq.id().groupid())){
            std::cout   << "Unable to lock the group with id for signature: " 
                        << sigreq.id().groupid() << std::endl;
            return false;
        }
        // send the request to all the players
        if(!SignaturePlayerDataRequest
            (
                sigreq.id().userid(),
                sigreq.id().groupid(),
                sigreq.message(), 
                sigreq.keyindex())
            )
        {
            std::cout << "Unable to send Player Data request all group members" << std::endl;
            return false;
        }
        //
        msgdesc.m_userID    = sigreq.id().userid();
        msgdesc.m_grpID     = sigreq.id().groupid();
        msgdesc.m_Message   = sigreq.message();
        msgdesc.m_eKeyIndex = sigreq.keyindex();
        //lock the group
        if(!unlockGrp(sigreq.id().groupid())){
            std::cout   << "Unable to lock the group with id for signature: " 
                        << sigreq.id().groupid() << std::endl;
            return false;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SIG_RESPONSE){
        thresholdsignature::SignDataMessage sigresp;
        if(!sigresp.ParseFromIstream(&is)){
            std::cout << "Unable to read GenerateSignatureResponse from the stream" << std::endl;
            return false;
        }
        playerGroupMetaData& grp = PlayerGrpData (sigresp.id().groupid() );
        std::vector<std::pair<std::string, std::string> > partialsigs;
        for(int j = 0; j<sigresp.signatures_size(); ++j){
            partialsigs.push_back(std::make_pair
                (
                    std::to_string( sigresp.signatures(j).ordinal() ), 
                    sigresp.signatures(j).signature()
                ) );
        }
        //GenerateSignatureResponse
        std::pair<BigNumber,BigNumber> sig = grp.CalculateGroupSignature
            (sigresp.message(), sigresp.keyindex(), partialsigs);

        std::cout << "Calculated Signature\t[" << sig.first.ToHex() << "," << sig.second.ToHex() << "]" << std::endl;
        
        
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_SIG_PLAYER_REQUEST){
        thresholdsignature::ShareOfSigRequest sigplayerreq;
        if(!sigplayerreq.ParseFromIstream(&is)){
            std::cout   << "unable to read SignaturePlayerDataRequest from the stream" 
                        << std::endl;
            return false;
        }
        msgdesc.m_userID =  getPublicPlayerInfo().userID();
        msgdesc.m_grpID = sigplayerreq.groupid();
        msgdesc.m_Message = sigplayerreq.message();
        msgdesc.m_eKeyIndex = sigplayerreq.keyindex();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_SIG_PLAYER_RESPONSE){
        thresholdsignature::ShareOfSigReply sigplayerresp;
        if(!sigplayerresp.ParseFromIstream(&is)){
            std::cout   << "unable to read SignaturePlayerDataResponse from the stream" 
                        << std::endl;
            return false;
        }
        // add to the GroupMetaData Collated partial signatures
        
        GroupMetadata& grp = GetGroup(sigplayerresp.id().groupid());
        grp.addCollatedPartialSignautre
            (
                std::to_string( sigplayerresp.ordinal() ),
                sigplayerresp.signature()
            );
        msgdesc.m_userID = getPublicPlayerInfo().userID();
        msgdesc.m_grpID = sigplayerresp.id().groupid();
    } else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIKEY_REQUEST){
        thresholdsignature::GenerateGroupPrivateKeyRequest groupPriKeyReq; 
        if(!groupPriKeyReq.ParseFromIstream(&is)){
            std::cout << "unable to read GroupPrivateKeyRequest from the stream" << std::endl;
            return false;
        }
        /////////////////
        // for a list of players in the request
        // send a PlayerPrivateShareExchangeRequest
        // lock the group
        lockGrp(groupPriKeyReq.id().groupid());

        std::vector<std::string> users;
        for(int i=0;i<groupPriKeyReq.players_size();++i){
            users.push_back(groupPriKeyReq.players(i));
        }
        // ask each player to share the evals with the relevant player
        if(!InitPrivateKeyShareExchange(groupPriKeyReq.id().userid(),groupPriKeyReq.id().groupid(),users)){
            std::cout << "Player sharing of evals failed" << std::endl;
            return false;
        }
        msgdesc.m_userID    = groupPriKeyReq.id().userid();
        msgdesc.m_grpID     = groupPriKeyReq.id().groupid();
        //clear the data sharing containers in GroupMetaData
 
        // unlock the group
        unlockGrp(groupPriKeyReq.id().groupid());


        //////////////////
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIKEY_RESPONSE){
        thresholdsignature::GenerateGroupPrivateKeyResponse groupPriKeyResp;
        if(!groupPriKeyResp.ParseFromIstream(&is)){
            std::cout << "unable to read GroupPrivateKeyResponse from the stream" << std::endl;
            return false;
        }
        playerGroupMetaData& grp = PlayerGrpData (groupPriKeyResp.id().groupid() );
        BigNumber priKey = grp.CalculateGroupPrivateKey();
        if(grp.ValidateGroupPrivateKey(priKey)){
            std::cout << "Group private Key validated and the key is:" << priKey.ToHex() << std::endl;
        }
        msgdesc.m_userID    = getPublicPlayerInfo().userID();
        msgdesc.m_grpID     = groupPriKeyResp.id().groupid();

    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_REQUEST){
        thresholdsignature::PlayerPrivateShareExchangeRequest req; 
        if(!req.ParseFromIstream(&is)){
            std::cout << "unable to read PlayerPrivateShareExchangeRequest from the stream" << std::endl;
            return false;
        }
        //  player to send his private key share to the player in req.m_userid
        if(!SharePrivateKeyShare(req.id().userid(), req.id().groupid())){
            std::cout   << "Unable to share private key share with player: " 
                        << req.id().userid()<< " for the group " << req.id().groupid() << std::endl;
        }
        msgdesc.m_userID    = req.id().userid();
        msgdesc.m_grpID     = req.id().groupid();
    } else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_RESPONSE){
        // the player has send his private share to the request user
        // move onto th next player
        thresholdsignature::PlayerPrivateShareExchangeResponse resp;
        if(!resp.ParseFromIstream(&is)){
            std::cout   << "unable to read PlayerPrivateShareExchangeResponse from the stream" 
                        << std::endl;
            return false;
        }
        if(resp.privatedatashared())
            std::cout   << "Player " << resp.id().userid() 
                        << " shared private share " << std::endl;
        msgdesc.m_userID    = resp.id().userid();
        msgdesc.m_grpID     = resp.id().groupid();

    } else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_REQUEST){
        // received a private key share from a player ... add to the list
        //
        thresholdsignature::PlayerPrivateKeyShareRequest req; 
        if(!req.ParseFromIstream(&is)){
            std::cout << "unable to read PlayerPrivateKeyShareRequest from the stream" << std::endl;
            return false;
        }
        // 
        std::cout   << "Received a private key share from " << req.id().userid() 
                    << " for group id " << req.id().groupid() << std::endl;
        playerGroupMetaData& grpinfo = PlayerGrpData (req.id().groupid());
        grpinfo.addPrivateKeyInfo(std::to_string(req.ordinal()),req.privatekeyshare());
        msgdesc.m_userID = req.id().userid();
        msgdesc.m_grpID = req.id().groupid();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_RESPONSE){
        thresholdsignature::PlayerPrivateKeyShareResponse resp;
        if(!resp.ParseFromIstream(&is)){
            std::cout   << "unable to read PlayerPrivateKeyShareResponse from the stream" 
                        << std::endl;
            return false;
        }
        if(resp.sharedprivatekeyshare())
            std::cout   << "Received acknowledge of private key share from " 
                        << resp.id().userid() << " and group " 
                        << resp.id().groupid();

        msgdesc.m_userID    = resp.id().userid();
        msgdesc.m_grpID     = resp.id().groupid();
    }
    
    return true; 
}

bool CreateResponseMessage(MessageDescriptor& respMesDesc,std::ostream& os){
    // depemnding on the message ID of the request then construnct the apropriate respnse

    if (respMesDesc.m_EnumMSGID == TSMessageDefs::TS_REGISTER_PLAYER_REQUEST){
        std::cout << "creating a register player response" << std::endl; 
        createRegisterReply(getPublicPlayerInfo(), os);
    }else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_LIST_PLAYER_REQUEST){
        std::cout << "Creating a playerlist response " << std::endl;
        createListPlayersResponse(getPublicPlayerInfo(), os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_REQUEST){
        std::cout << "creating a createGroupReply" << std::endl;
        createGroupReply(getPublicPlayerInfo(),respMesDesc.m_grpID, true,os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_INVITE_TO_GROUP_REQUEST){
        std::cout << "Creating a GroupInvite Response " << std::endl;        
        if(respMesDesc.m_userID.empty() && respMesDesc.m_grpID.empty()){
            std::cout << "No group details to build invite response from" << std::endl;
            return false;
        }
        std::pair<std::string, bool> inviteInfo = acceptInvitationToGroup(respMesDesc.m_grpID); 
        player p = getPublicPlayerInfo();
        createInviteReply(respMesDesc.m_grpID, inviteInfo.first, inviteInfo.second,os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_REQUEST){
        std::cout << "Create a groupIsSet response" << std::endl;
        createGroupIsSetReply(respMesDesc.m_grpID,respMesDesc.m_userID,true,os);
        //createGroupIsSetReply(respMesDesc.m_grpID,SinglePlayer::Instance()->UserID(),true,os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_DELETE_GROUP_REQUEST){
        // all users have responsed to say they have deleted the group & the orchestrator has deleted the group
        createDeleteTSGroupResponse(getPublicPlayerInfo(),respMesDesc.m_grpID, true, os); 
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_DELETE_PLAYER_GROUP_REQUEST){
        bool deleted(true);
        try{
            deletePlayerGroup(respMesDesc.m_userID, respMesDesc.m_grpID);
        }catch (std::exception& e){
            deleted = !deleted;
            std::cout << "Unable to delete the group : " << respMesDesc.m_grpID << std::endl;
        }
        createDeleteTSPlayerGroupResponse(respMesDesc.m_userID,respMesDesc.m_grpID, deleted, os); 
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SECRET_REQUEST){
        std::cout << "Creating Secret sharing response ... the group/key/secret should now be set" << std::endl;
        createSecretSharingResponse(getPublicPlayerInfo(),respMesDesc.m_grpID,respMesDesc.m_SSType, true, os);
    } else if (respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIVATE_EVAL_REQUEST){
        createPrivateDataEvalResponse(respMesDesc.m_userID,respMesDesc.m_grpID, true ,os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_INITIATE_PRIVATE_EVAL_REQUEST){
        std::cout << "All players have shared their evals...response back to proposer" << std::endl;
        createSecretSharingInitEvalResponse(respMesDesc.m_userID,respMesDesc.m_grpID, respMesDesc.m_SSType,true,os );
    }else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_SECRET_REQUEST){        
        const int& myOrdinal = ordinalForGroup(respMesDesc.m_grpID);
        const jvrss& jvrssobj = PlayerGrpData(respMesDesc.m_grpID).m_transientData;
        createSecretSharingPlayerDataResponse(getPublicPlayerInfo().userID(),respMesDesc.m_grpID,respMesDesc.m_SSType,myOrdinal, jvrssobj, os);
    } else if (respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_REQUEST){
        createSecretSharingCollatedPlayerResponse(respMesDesc.m_userID,respMesDesc.m_grpID, respMesDesc.m_SSType,os);
    } else if (respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_REQUEST){
        createEphemeralKeyDataResponse(respMesDesc.m_userID,respMesDesc.m_grpID,os);
        if(!deleteGrpPreSignatureContainers(respMesDesc.m_grpID)){
            std::cout << "Unable to clear the containers for a pre-signature request" << std::endl;
        }
        
    }else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_REQUEST){
        // each player does k*alpha & alpha * G
        const int& myOrdinal = ordinalForGroup(respMesDesc.m_grpID);
        std::pair<BigNumber,ECPoint> eKeyData = EphemeralKeyPlayerData(respMesDesc.m_grpID);
        createEphemeralKeyPlayerDataResponse(respMesDesc.m_userID, respMesDesc.m_grpID,myOrdinal,eKeyData, os); 
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_REQUEST){
        const int& myOrdinal = ordinalForGroup(respMesDesc.m_grpID);
        createAEphemeralKeyCollatedDataResponse(respMesDesc.m_userID,respMesDesc.m_grpID,std::to_string(myOrdinal),os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SIG_REQUEST){
        const GroupMetadata& grp = GetGroup(respMesDesc.m_grpID);
        createSignatureResponse(respMesDesc.m_userID, respMesDesc.m_grpID, grp, respMesDesc.m_Message,respMesDesc.m_eKeyIndex, os); 
        if(!deleteGrpSignatureSharingContainsers(respMesDesc.m_grpID)){
            std::cout << "Unable to clear the containers for a signature request" << std::endl;
        }
        
    }else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_SIG_PLAYER_REQUEST){
        playerGroupMetaData& playergrp = PlayerGrpData(respMesDesc.m_grpID);
        std::pair<std::string, BigNumber> sigInfo = playergrp.CalcPartialSignature(respMesDesc.m_Message,respMesDesc.m_eKeyIndex);
        if(sigInfo.first == std::to_string(-1)){
            // condition of no ephermal keys ... tell the signature requestor
            return false;
        }
        // create a response message
        createSignaturePlayerDataResponse(respMesDesc.m_userID,respMesDesc.m_grpID,sigInfo,respMesDesc.m_eKeyIndex,os);
    } else if (respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIKEY_REQUEST){
        // all players have shared private key with the requesting player ... return a success
        // specific applications will have to handle this as they see fit.
        createPrivateKeyResponse(respMesDesc.m_userID,respMesDesc.m_grpID,true, os);
    }else if (respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRIKEYSHARE_EXCHANGE_REQUEST){
        // key share send by player (ack received)...response back to orchestrator
        createPlayerPrivateShareExchangeResponse(getPublicPlayerInfo().userID(),respMesDesc.m_grpID, true, os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_PRIKEY_REQUEST){
        // hard-coded to true ... applications must decide on how to validate a share
        createPlayerPrivateKeyShareResponse(respMesDesc.m_userID, respMesDesc.m_grpID,true,os);
    }
    return true; 
}

bool ShareEvalsPlayers(const std::string& proposer, const std::string& grpid, const std::string& calctype){
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grpid);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grpid; 
        return false;
    }
    const GroupMetadata& grp = iter->second;
    for(std::vector<playerCommsInfo>::const_iterator grpMemIter = grp.participantList().begin(); grpMemIter != grp.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(grpMemIter->m_userid);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        boost::asio::streambuf b;
        std::ostream os(&b);
        std::cout << "Sending an initiate private eval sharing to player " << playersIter->second.addr() << " " << playersIter->second.port() << std::endl;
        createSecretSharingInitEvalRequest(proposer,grpid,calctype,os); 
        genericRequestResponse(os,b,playersIter->second.addr(), playersIter->second.port());
        
    } 
    return true;   
}

bool SharePrivateEvalsRequest(const std::string& userid, const std::string& grpid, const std::string& calctype){
    playerGroupMetaData& grpInfo = PlayerGrpData (grpid); 
    // calculates all the data required for secret sharing at this point.
    const jvrss& jvrssobj = PlayerSecretDataForGrp(grpid, calctype);
    for (std::vector<std::pair<int,player> >::const_iterator iter = grpInfo.m_ordinalAndPlayerList.begin(); iter != grpInfo.m_ordinalAndPlayerList.end(); ++iter){
        boost::asio::streambuf b;
        std::ostream os(&b);
        createPrivateDataEvalRequest(userid,grpid, grpInfo.m_ordinal, iter->first,jvrssobj, os);
        genericRequestResponse(os,b, iter->second.addr(), iter->second.port());
    }
    return true;   
}
bool SecretSharingPlayerDataRequest(const std::string& proposer, const std::string& grpid, const std::string& calctype){
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grpid);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grpid; 
        return false;
    }
    const GroupMetadata& grp = iter->second;
    for(std::vector<playerCommsInfo>::const_iterator grpMemIter = grp.participantList().begin(); grpMemIter != grp.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(grpMemIter->m_userid);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        boost::asio::streambuf b;
        std::ostream os(&b);
        createSecretSharingPlayerDataRequest(proposer,grpid,calctype,os); 
        genericRequestResponse(os,b,playersIter->second.addr(), playersIter->second.port());
    } 
    return true;
}


bool SendGroupInvites(const std::string& proposer, const std::string& grpID){
    std::vector<player> players =  GlobalUsers::Instance()->getPlayerList();
    // ask each player ... gather up the responses ... randomise and us the first n (n is the group size
    // random at this point.
    std::shuffle(players.begin(), players.end(),std::default_random_engine {}); 
    for (std::vector<player>::const_iterator iter = players.begin(); iter != players.end(); ++iter){
        //
       // create an invite message
        boost::asio::streambuf b;
        std::ostream os(&b);
       // don't send a message to oneself
        if(proposer != iter->userID()){
            createInviteRequest( grpID, os);
            genericRequestResponse(os,b,iter->addr(), iter->port());
       }  
    }
    return false;
}

bool SendGroupDetails(const std::string& grp){
    // find the group
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    
    const GroupMetadata& grpinfo = iter->second;
    //std::cout << "Group Details in SendGroupDetails\n" << grpinfo << std::endl;
    for (   std::vector<playerCommsInfo>::const_iterator grpMemIter = iter->second.participantList().begin(); 
            grpMemIter != iter->second.participantList().end(); 
            ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(grpMemIter->m_userid);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        boost::asio::streambuf b;
        std::ostream os(&b);
        std::cout << "SendGroupDetails .. calling CreateGroupIsSet" << std::endl;
        createGroupIsSetRequest(grpMemIter->m_userid, iter->second,os); 
        std::cout << "Sending group details to " << playersIter->second.addr() << "," << playersIter->second.port() << std::endl;
        genericRequestResponse(os,b,playersIter->second.addr(), playersIter->second.port());
    }
    return true; 
}

bool SendGroupDelete(const std::string& userID, const std::string& grp){
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    
    const GroupMetadata& grpinfo = iter->second;
    for (std::vector<playerCommsInfo>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(grpMemIter->m_userid);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        if(userID != playersIter->second.userID()){
            boost::asio::streambuf b;
            std::ostream os(&b);
            createDeleteTSPlayerGroupRequest(userID,grp,os); 
            genericRequestResponse(os,b,playersIter->second.addr(), playersIter->second.port());
       }
    }
    return true; 
}

bool SecretSharingCollatedDataRequest(const std::string& userid, const std::string& grp, const std::string& calctype){
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    const GroupMetadata& grpinfo = iter->second;
    for (std::vector<playerCommsInfo>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(grpMemIter->m_userid);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        boost::asio::streambuf b;
        std::ostream os(&b);
        createSecretSharingCollatedDataRequest(userid,grp,calctype,grpinfo,os); 
        genericRequestResponse(os,b,playersIter->second.addr(), playersIter->second.port());
    }
    
    return true;
}


bool EphemeralKeyPlayerDataRequest(const std::string& userid, const std::string& grp){
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    const GroupMetadata& grpinfo = iter->second;
    for (std::vector<playerCommsInfo>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(grpMemIter->m_userid);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        boost::asio::streambuf b;
        std::ostream os(&b);
        createEphermalKeyPlayerDataRequest(userid,grp,os); 
        genericRequestResponse(os,b,playersIter->second.addr(), playersIter->second.port());
    }
    std::cout << grpinfo << std::endl;
    return true;
}

bool EphemeralKeyCollatedPlayerDataRequest(const std::string& userid, const std::string& grp){
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    const GroupMetadata& grpinfo = iter->second;
    for (std::vector<playerCommsInfo>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(grpMemIter->m_userid);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        boost::asio::streambuf b;
        std::ostream os(&b);
        createEphemeralKeyCollatedDataRequest(userid,grp,grpinfo,os); 
        genericRequestResponse(os,b,playersIter->second.addr(), playersIter->second.port());
        std::cout << grpinfo << std::endl;
    }    
    return true;
}

bool SignaturePlayerDataRequest(const std::string& userid, const std::string& grp, const std::string& themessage, const int& ekeyindex){
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    const GroupMetadata& grpinfo = iter->second;
    for (std::vector<playerCommsInfo>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(grpMemIter->m_userid);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        if(userid != playersIter->second.userID()){
            boost::asio::streambuf b;
            std::ostream os(&b);
            createSignaturePlayerDataRequest(userid,grp,themessage,ekeyindex,os); 
            genericRequestResponse(os,b,playersIter->second.addr(), playersIter->second.port());
        }
    } 
    std::cout << grpinfo << std::endl;
    return true;
}

bool InitPrivateKeyShareExchange(const std::string& userid,const std::string& grpid,const std::vector<std::string>& players){
    groupsMap::const_iterator iter = GlobalGroups::Instance()->groups().find(grpid);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grpid; 
        return false;
    }

    const GroupMetadata& grp = iter->second;
    // verify that enough users have been supplied to allow for the key to reconstructed correctly.
    if(players.size() != (grp.m() - 1)){
        std::cout << "Not enough players supplied to meet the key reconstruction criteria for this group\nGroup threshold is " 
                        << grp.m() <<"\nSize of players list supplied " << players.size() << std::endl;
        return false;
    }
    for(std::vector<std::string>::const_iterator iter = players.begin(); iter != players.end(); ++ iter){
        const std::string& p = *iter;
        std::vector<playerCommsInfo>::const_iterator grpPlayersIter
                = std::find_if(grp.participantList().begin(), grp.participantList().end(), [&p](playerCommsInfo const& elem) {return elem.m_userid == p;});
        if(grpPlayersIter == grp.participantList().end()){
            std::cout << "No contact details for player " << p << " in group " << grpid << " in InitPrivateKeyExchange" << std::endl;
            return false;
        }
        boost::asio::streambuf b;
        std::ostream os(&b);
        std::cout << "sending an init private key share exchange to player (needs to know who to send it back to)" << std::endl;
        createPlayerPrivateShareExchangeRequest(userid, grpid,os);
        genericRequestResponse(os,b,grpPlayersIter->m_ip,grpPlayersIter->m_port);

    }
    return true; 
}

bool SharePrivateKeyShare(const std::string& userid,const std::string& grpid){
    playerGroupMetaData& grpInfo = PlayerGrpData (grpid); 
    // calculates all the data required for secret sharing at this point.
    std::vector<std::pair<int, player> >::const_iterator iter 
        = std::find_if(grpInfo.m_ordinalAndPlayerList.begin(), grpInfo.m_ordinalAndPlayerList.end(),[&userid](std::pair<int, player> const& elem) { return elem.second.userID() == userid;});
    
    if(iter == grpInfo.m_ordinalAndPlayerList.end()){
        std::cout << "No connection details for player with user id " << userid << " in the group " << grpid << std::endl;
        return false;
    }
    boost::asio::streambuf b;
    std::ostream os(&b);
    createPlayerPrivateKeyShareRequest(grpInfo,os);
    genericRequestResponse(os,b, iter->second.addr(), iter->second.port());
    return true;   
}