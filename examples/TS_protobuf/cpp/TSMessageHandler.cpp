#include <TSMessageHandler.h>
#include <player.h>
#include <GroupMetaData.h>
#include <orchestrator.h>
#include <random>
#include <MessageRequestResponse.h>

bool HandleMessage(MessageDescriptor& msgdesc, std::istream& is){
    
    std::cout << "HandleRequestMessage for message id  " << msgdesc.m_EnumMSGID << std::endl;

    if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_TIME_REQUEST){
        thresholdsignature::TimeRequest timeReq;
        if(!timeReq.ParseFromIstream(&is)){
            std::cout << "Unable to read message from socket stream" << std::endl;
            //send an error message
            return false;
        }
        std::cout << "Received a timereqest from " << timeReq.name() << " with UUID " << timeReq.uuid() << std::endl;
        std::cout << "Sending reply" << std::endl;
        
    }else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_TIME_RESPONSE){
        thresholdsignature::TimeResponse timeResp;
        if (!timeResp.ParseFromIstream(&is)){
            std::cout << "Unable to read time response message from socket stream" << std::endl;
            return false;
        }
        std::cout << timeResp.timestring();
        
    }else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_REGISTER_PLAYER_REQUEST){
        thresholdsignature::RegisterPlayerRequest registerreq;
        if(!registerreq.ParseFromIstream(&is)){
            std::cout << "Unable to read a resiger player request message from the input stream" << std::endl;
            return false; 
        }
        thresholdsignature::player tsplayer = registerreq.playerid(0);
        
        player p (tsplayer.name(), tsplayer.uri(), tsplayer.addr(), tsplayer.port()); 
        addPlayer(p); 
    }else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_REGISTER_PLAYER_RESPONSE){
        thresholdsignature::RegisterPlayerResponse resp;
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
            const thresholdsignature::player& tsplayer = resp.playerid(j);
            player p(tsplayer.name(), tsplayer.uri(), tsplayer.addr(), tsplayer.port());
            std::cout << p << std::endl;
        }

    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_REQUEST){
        thresholdsignature::CreateThresholdGroup grpreq;
        if(!grpreq.ParseFromIstream(&is)){
            std::cout << "Unable to read create threshold group request from the input stream" << std::endl;
            return false;
        }
        std::string proposer = grpreq.proposerid();
        int m = grpreq.recombinationthreshold();
        int n = grpreq.maxnumberofplayers();
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
        thresholdsignature::InviteToGroupRequest req; 
        if(!req.ParseFromIstream(&is)){
            std::cout << "unable to read playerlist request from the input stream" << std::endl;
            return false; 
        }
        //
        std::cout << "Received invite to grp id" <<  req.groupid() << " from " << req.proposeruserid() << std::endl;
        msgdesc.m_grpID = req.groupid();
        return true;
        
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_INVITE_TO_GROUP_RESPONSE){
        // handle a group response
        thresholdsignature::InviteToGroupResponse resp; 
        if(!resp.ParseFromIstream(&is)){
            std::cout << "unable to read inviteToGroupResonse from the input stream" << std::endl;
            return false;
        }
        std::cout << "Received invite to grp response from: " << resp.userid() << " for group " << resp.groupid() << std::endl;
        if(resp.acceptinvite())
            addUserToGroup(resp.groupid(),resp.userid());
        
        msgdesc.m_grpID = resp.groupid();
        
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_REQUEST){
        // broadcast group message.
        thresholdsignature::ThresholdGroup reqTSGroup; 
        if(!reqTSGroup.ParseFromIstream(&is)){
            std::cout << "Unable to read ThresholdGroup message from input stream" << std::endl;
        }
        // hard code for now
        BigNumber mod;
        mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
        playerGroupMetaData playerGrp (reqTSGroup.t(),mod);
        playerGrp.m_id = reqTSGroup.grpid();

                    
        for (int j = 0; j < reqTSGroup.member_size(); j++) {
            const thresholdsignature::ThresholdGroup::GroupMember& tsplayer = reqTSGroup.member(j);
            if(tsplayer.userid() ==  SinglePlayer::Instance()->UserID())
                playerGrp.m_ordinal = tsplayer.ordinal();
            else
                playerGrp.m_ordinalList.push_back(tsplayer.ordinal());
        }
        
        std::cout << "TSMessageFactory..Handling broadcast group details message\n"
                    << "\n\tGroup ID : " << playerGrp.m_id
                    << "\n\tPlayer ordinal : " << playerGrp.m_ordinal
                    << "\n\t player ordinal list: "
                    ;
        for(std::vector<int>::const_iterator iter = playerGrp.m_ordinalList.begin(); iter != playerGrp.m_ordinalList.end(); ++iter){
            std::cout << *iter << " " ;
        }
        
        
        std::cout << std::endl;
        playerGrp.polynomialPreCalculation(playerGrp.m_privateKeyPolynomial);
        
        SinglePlayer::Instance()->addPlayerGroup(playerGrp);
        msgdesc.m_grpID = reqTSGroup.grpid();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_RESPONSE){
        thresholdsignature::ThresholdGroupResponse resp;
        if(!resp.ParseFromIstream(&is)){
            std::cout << "unable to read thresholdgroupresponse from the input stream" << std::endl;
        }
        std::cout << "received thresholdgroup response for group " << resp.groupid()  << " from " << resp.userid() << std::endl;
    }else if(msgdesc.m_EnumMSGID  == TSMessageDefs::TS_CREATE_GROUP_RESPONSE){
        thresholdsignature::CreateThresholdGroupResponse tsgrpResp; 
        if(!tsgrpResp.ParseFromIstream(&is)){
            std::cout << "unable to read threshold group response from the input stream" << std::endl;
        }
        //if we may it here, then all invitees to the group have responded
        if(tsgrpResp.groupset())
            std::cout << "Group: " << tsgrpResp.grpid() << " is set " << std::endl;
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
        thresholdsignature::SecretSharingRequest secretSharingReq; 
        if(!secretSharingReq.ParseFromIstream(&is)){
            std::cout << "Unable to stream SecretSharingRequest from the input stream" << std::endl;
            return false;
        }
        // for a list of players in the group
        // send a SecretSharingPlayerDataRequest
        // lock the group
        lockGrp(secretSharingReq.groupid());
        if(!SecretSharingPlayerDataRequest(secretSharingReq.userid(),secretSharingReq.groupid(),secretSharingReq.calctype())){
            std::cout << "Unable to send Player Data request all group members" << std::endl;
            return false;
        }
        
        // build the collated data
        // send SecretSharingCollatedPlayerRequest to each player
        if(!SecretSharingCollatedDataRequest(secretSharingReq.userid(),secretSharingReq.groupid(),secretSharingReq.calctype())){
            std::cout << "Unable to send collated player data request to all group members" << std::endl;
            return false;
        }
        // return so the system can send a SecretSharingResponse to the original requestor
        msgdesc.m_userID = secretSharingReq.userid();
        msgdesc.m_grpID = secretSharingReq.groupid();
        msgdesc.m_SSType = secretSharingReq.calctype();
        //clear the data sharing containers in GroupMetaData
        if(!DeleteGrpSharingContainers(secretSharingReq.groupid())){
            std::cout << "Could not clear up JVRSS Public shared data for group id " << secretSharingReq.groupid() << std::endl;
            return false;
        }
        // unlock the group
        unlockGrp(secretSharingReq.groupid());

    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SECRET_RESPONSE){
        // acknowledge that the secret has been shared
        thresholdsignature::SecretSharingResponse secretSharingResp;
        if(!secretSharingResp.ParseFromIstream(&is)){
            std::cout << "Unable to send parse a secret sharing response from the input stream" << std::endl;
            return false;
        }
        
        msgdesc.m_userID = secretSharingResp.userid();
        msgdesc.m_grpID = secretSharingResp.groupid();
        msgdesc.m_SSType = secretSharingResp.calctype();
        
        if(secretSharingResp.sharedsecret()){
            std::cout << "Secret of type " << secretSharingResp.calctype() << " has been shared by the group " << msgdesc.m_grpID << std::endl;
        }
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_SECRET_REQUEST){
        thresholdsignature::SecretSharingPlayerDataRequest req;
        if(!req.ParseFromIstream(&is)){
            std::cout << "Unable to read secret sharing player data request from input stream" << std::endl;
            return false;
        }
        msgdesc.m_userID = req.userid();
        msgdesc.m_grpID = req.groupid();
        msgdesc.m_SSType = req.calctype(); 
    } else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_SECRET_RESPONSE){
        thresholdsignature::SecretSharingPlayerDataResponse ssPlayerDataResponse;
        if(!ssPlayerDataResponse.ParseFromIstream(&is)){
            std::cout << "Unable to read Secret Sharing Player Data Response from the input stream" << std::endl;
               return false;
        }
        //  parse & load the GroupMetaData components
        GroupMetadata& grp = GetGroup(ssPlayerDataResponse.groupid());
        grp.calculationType () = ssPlayerDataResponse.calctype();
        std::string ord = std::to_string(ssPlayerDataResponse.ordinal());
        
        for(int j = 0; j<ssPlayerDataResponse.evals_size(); ++j){
            grp.addCollatedEvals(ord, ssPlayerDataResponse.evals(j).ordinal(), ssPlayerDataResponse.evals(j).eval());
        }
        
        std::vector<std::string> poly;
        for(int j = 0; j<ssPlayerDataResponse.hiddenpoly_size(); ++j){
            thresholdsignature::HiddenPoly hp = ssPlayerDataResponse.hiddenpoly(j);
            for(int k = 0; k <hp.val_size(); ++k){
                poly.push_back(hp.val(k).val());
            }
        }
        grp.addCollatedHiddenPolys(ord,poly);
        
        //verify that ord == ssPlayerDataResponse.hiddenevals(j).key()
        for(int j=0; j<ssPlayerDataResponse.hiddenevals_size(); ++j){
            for(int k=0;k<ssPlayerDataResponse.hiddenevals(j).value_size();++k){
                grp.addCollatedHiddenEvals(ord,ssPlayerDataResponse.hiddenevals(j).value(k).ordinal(),ssPlayerDataResponse.hiddenevals(j).value(k).eval());
            }
        }
        
        msgdesc.m_userID = ssPlayerDataResponse.userid();
        msgdesc.m_grpID = ssPlayerDataResponse.groupid();
        msgdesc.m_SSType = ssPlayerDataResponse.calctype();
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_REQUEST){
        thresholdsignature::SecretSharingCollatedPlayerRequest ssCollatedPlayerDataRequest;
        if(!ssCollatedPlayerDataRequest.ParseFromIstream(&is)){
            std::cout << "Unable to read Secret Sharing Collated Data request" << std::endl;
            return false;
        }
        playerGroupMetaData& playergrp = PlayerGrpData(ssCollatedPlayerDataRequest.groupid());
        // evals
        std::cout << "Size of collated evals .. " << ssCollatedPlayerDataRequest.collatedevals_size() << std::endl;
        for(int j=0;j<ssCollatedPlayerDataRequest.collatedevals_size(); ++j){
            std::string ordinal = ssCollatedPlayerDataRequest.collatedevals(j).key();
            std::vector<std::pair<std::string,std::string> > evals;
            for(int k=0;k<ssCollatedPlayerDataRequest.collatedevals(j).value_size();++k){
                evals.push_back(std::make_pair(ssCollatedPlayerDataRequest.collatedevals(j).value(k).ordinal(), ssCollatedPlayerDataRequest.collatedevals(j).value(k).eval()));
            }
            playergrp.addPublicEvalsToJVRSS(ordinal,evals);
        }
        // hidden polys
        for (int j=0;j<ssCollatedPlayerDataRequest.collatedhiddenpolys_size(); ++j){
            std::string ordinal = ssCollatedPlayerDataRequest.collatedhiddenpolys(j).key();
            std::vector<std::string> polynomial;
            for(int k=0;k<ssCollatedPlayerDataRequest.collatedhiddenpolys(j).value_size(); ++k){
                polynomial.push_back(ssCollatedPlayerDataRequest.collatedhiddenpolys(j).value(k).val());
            }
            playergrp.addHiddenPolynomialToJVRSS(ordinal, polynomial);
        }
        
        // hidden evals
        for(int j=0; j<ssCollatedPlayerDataRequest.collatedhiddenevals_size();++j){
            std::string ordinal = ssCollatedPlayerDataRequest.collatedhiddenevals(j).key();
            std::vector<std::pair<std::string,std::string> > hiddenevals;
            for(int k=0;k<ssCollatedPlayerDataRequest.collatedhiddenevals(j).value_size();++k){
                hiddenevals.push_back(std::make_pair(ssCollatedPlayerDataRequest.collatedhiddenevals(j).value(k).ordinal(), ssCollatedPlayerDataRequest.collatedhiddenevals(j).value(k).eval()));
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
        std::cout << "Secret Validated & calculated " << secret.ToHex() << std::endl; 
        if( ssCollatedPlayerDataRequest.calctype() == "PRIVATEKEYSHARE"){
            playergrp.m_privateKeyShare = secret; 
            playergrp.m_GroupPublicKey = playergrp.createGroupPublicKey();
            std::cout << "Calculated the group public key " << playergrp.m_GroupPublicKey.ToHex() << std::endl;
        }else if(ssCollatedPlayerDataRequest.calctype() == "LITTLEK"){
            playergrp.m_littleK = secret;
            
        }else if(ssCollatedPlayerDataRequest.calctype() == "ALPHA"){
            playergrp.m_alpha = secret;
        }
        msgdesc.m_userID = getPublicPlayerInfo().userID();
        msgdesc.m_grpID = ssCollatedPlayerDataRequest.groupid();
        msgdesc.m_SSType = ssCollatedPlayerDataRequest.calctype();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_SECRET_PLAYER_RESPONSE){
        thresholdsignature::SecretSharingCollatedPlayerResponse ssCollatedPlayerResp;
        if(!ssCollatedPlayerResp.ParseFromIstream(&is)){
            std::cout << "Unable to read Secret Sharing Collated Player Response from the the stream" << std::endl;
            return false;
        }
        if(ssCollatedPlayerResp.collatedplayerdatavalidated()){
            std::cout 
                << "Player " << ssCollatedPlayerResp.userid() << " has validated the data for secret of type " << ssCollatedPlayerResp.calctype()
                << " with group id " << ssCollatedPlayerResp.groupid()
                << std::endl;
        }
        msgdesc.m_userID = ssCollatedPlayerResp.userid();
        msgdesc.m_grpID = ssCollatedPlayerResp.groupid();
        msgdesc.m_SSType = ssCollatedPlayerResp.calctype();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_REQUEST){
        // ephemeral key data requeest
        thresholdsignature::EphemeralKeyDataRequest ekeyDataReq;
        if(!ekeyDataReq.ParseFromIstream(&is)){
            std::cout << "Unable to read ephemeral key data request from the stream" << std::endl;
            return false;
        }
        
         // for a list of players in the group
        // send a EphemeralKeyPlayerDataRequest
        if (!lockGrp(ekeyDataReq.groupid())){
            std::cout << "Unable to lock the group with id: " << ekeyDataReq.groupid() << std::endl;
            return false;
        }
        if(!EphemeralKeyPlayerDataRequest(ekeyDataReq.userid(),ekeyDataReq.groupid())){
            std::cout << "Unable to send Player Data request all group members" << std::endl;
            return false;
        }
        // build the collated data
        // send EphemeralKeyollatedPlayerRequest to each player
        if(!EphemeralKeyCollatedPlayerDataRequest(ekeyDataReq.userid(),ekeyDataReq.groupid())){
            std::cout << "Unable to send collated player data request to all group members" << std::endl;
            return false;
        }
        // return so the system can send a SecretSharingResponse to the original requestor
        msgdesc.m_userID = ekeyDataReq.userid();
        msgdesc.m_grpID = ekeyDataReq.groupid();
        //clear the data sharing containers in GroupMetaData
        if(!unlockGrp(ekeyDataReq.groupid())){
            std::cout << "Unable to unlock the group with id: " << ekeyDataReq.groupid() << std::endl;
        }
        
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_RESPONSE){
        // ephermal key data response (last one to do)
        thresholdsignature::EphemeralKeyDataResponse ekeyDataResp;
        if(!ekeyDataResp.ParseFromIstream(&is)){
            std::cout << "Unable to read EphemeralKeyDataResponse from the stream " << std::endl;
            return false;
        }
        if(ekeyDataResp.emphemeralkeydataset()){
            std::cout << "Ephemeral Key data set for all players in the group " << ekeyDataResp.groupid() << std::endl;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_REQUEST){
        // 
        thresholdsignature::EphemeralKeyPlayerDataRequest ekeyPlayerDataReq;
        if(!ekeyPlayerDataReq.ParseFromIstream(&is)){
            std::cout << "Unable to read ephemeral key player data request from the stream" << std::endl;
            return false;
        }
     
        // set the user & grp id
        msgdesc.m_userID = getPublicPlayerInfo().userID();
        msgdesc.m_grpID = ekeyPlayerDataReq.groupid();
    } else if (msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PRE_SIG_PLAYER_RESPONSE){
        thresholdsignature::EphemeralKeyPlayerDataResponse ekeyplayerDataResp;
        if(!ekeyplayerDataResp.ParseFromIstream(&is)){
            std::cout << "Unable to read ephemeral key player data response from the stream" << std::endl;
            return false;
        }
        //  parse & load the GroupMetaData components
        GroupMetadata& grp = GetGroup(ekeyplayerDataResp.groupid());
        grp.addCollatedVW(std::to_string(ekeyplayerDataResp.ordinal()), ekeyplayerDataResp.kalpha(), ekeyplayerDataResp.alphag());
        msgdesc.m_userID = ekeyplayerDataResp.userid();
        msgdesc.m_grpID = ekeyplayerDataResp.groupid();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_REQUEST){
        thresholdsignature::AllEphemeralKeyPlayerRequest ekeyCollatedVWReq; 
        if(!ekeyCollatedVWReq.ParseFromIstream(&is)){
            std::cout << "Unable to read AllEphemeralKeyPlayerRequest from the stream" << std::endl;
            return false; 
        }
        playerGroupMetaData& playergrp = PlayerGrpData(ekeyCollatedVWReq.groupid());
        for(int j = 0; j < ekeyCollatedVWReq.collatedvws_size(); ++j){
            std::string ord = ekeyCollatedVWReq.collatedvws(j).ordinal();
            std::string v = ekeyCollatedVWReq.collatedvws(j).v();
            std::string w = ekeyCollatedVWReq.collatedvws(j).w();
            playergrp.addPreSignDataToJVRSS(ord,v,w);
        }
        // each player interpolates the v curve, w curve 
        playergrp.CalculateEphemeralKey();
        msgdesc.m_userID = getPublicPlayerInfo().userID();
        msgdesc.m_grpID = ekeyCollatedVWReq.groupid();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_COLLATED_PRE_SIG_RESPONSE){
        thresholdsignature::AllEphemeralKeyPlayerResponse ekeyCollatedVWResp; 
        if(!ekeyCollatedVWResp.ParseFromIstream(&is)){
            std::cout << "Unable to read AllEphemeralKeyPlayerResponse from the stream" << std::endl;
            return false; 
        }
        if(ekeyCollatedVWResp.ephemeralkeyplayerset()){
            std::cout << "AllEphemeralKeyPlayerResponse success from player " << ekeyCollatedVWResp.userid() << " from group " << ekeyCollatedVWResp.groupid() << std::endl;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SIG_REQUEST){
        thresholdsignature::GenerateSignatureRequest sigreq;
        if(!sigreq.ParseFromIstream(&is)){
            std::cout << "Unable to read GenerateSignatureRequest from the stream" << std::endl;
            return false;
        }

        //lock the group
        if(!lockGrp(sigreq.groupid())){
            std::cout << "Unable to lock the group with id for signature: " << sigreq.groupid() << std::endl;
            return false;
        }
        // send the request to all the players
        if(!SignaturePlayerDataRequest(sigreq.userid(),sigreq.groupid(),sigreq.msg(), sigreq.emphemeralkeyindex())){
            std::cout << "Unable to send Player Data request all group members" << std::endl;
            return false;
        }
        //
        msgdesc.m_userID = sigreq.userid();
        msgdesc.m_grpID = sigreq.groupid();
        msgdesc.m_Message = sigreq.msg();
        msgdesc.m_eKeyIndex = sigreq.emphemeralkeyindex();
        //lock the group
        if(!unlockGrp(sigreq.groupid())){
            std::cout << "Unable to lock the group with id for signature: " << sigreq.groupid() << std::endl;
            return false;
        }
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_SIG_RESPONSE){
        thresholdsignature::GenerateSignatureResponse sigresp;
        if(!sigresp.ParseFromIstream(&is)){
            std::cout << "Unable to read GenerateSignatureResponse from the stream" << std::endl;
            return false;
        }
        playerGroupMetaData& grp = PlayerGrpData (sigresp.groupid() );
        std::vector<std::pair<std::string, std::string> > partialsigs;
        for(int j = 0; j<sigresp.partialsignatures_size(); ++j){
            partialsigs.push_back(std::make_pair(sigresp.partialsignatures(j).ordinal(), sigresp.partialsignatures(j).eval()));
        }
        //GenerateSignatureResponse
        std::pair<BigNumber,BigNumber> sig = grp.CalculateGroupSignature(sigresp.msg(), sigresp.emphemeralkeyindex(), partialsigs);

        std::cout << "Calculated Signature\t[" << sig.first.ToHex() << "," << sig.second.ToHex() << "]" << std::endl;
        
        
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_SIG_PLAYER_REQUEST){
        thresholdsignature::SignaturePlayerDataRequest sigplayerreq;
        if(!sigplayerreq.ParseFromIstream(&is)){
            std::cout << "unable to read SignaturePlayerDataRequest from the stream" << std::endl;
            return false;
        }
        msgdesc.m_userID =  getPublicPlayerInfo().userID();
        msgdesc.m_grpID = sigplayerreq.groupid();
        msgdesc.m_Message = sigplayerreq.msg();
        msgdesc.m_eKeyIndex = sigplayerreq.emphemeralkeyindex();
    }else if(msgdesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_SIG_PLAYER_RESPONSE){
        thresholdsignature::SignaturePlayerDataResponse sigplayerresp;
        if(!sigplayerresp.ParseFromIstream(&is)){
            std::cout << "unable to read SignaturePlayerDataResponse from the stream" << std::endl;
            return false;
        }
        // add to the GroupMetaData Collated partial signatures
        
        GroupMetadata& grp = GetGroup(sigplayerresp.groupid());
        grp.addCollatedPartialSignautre(sigplayerresp.ordinal(),sigplayerresp.signatureshare());
        msgdesc.m_userID = getPublicPlayerInfo().userID();
        msgdesc.m_grpID = sigplayerresp.groupid();
    }
    
    return true; 
}

bool CreateResponseMessage(MessageDescriptor& respMesDesc,std::ostream& os){
    // depemnding on the message ID of the request then construnct the apropriate respnse
    //if (respMesDesc.m_msgID == TSMessageDefs::TS_TIME_REQUEST){
    if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_TIME_REQUEST){
        createTimeResponse(getPublicPlayerInfo(),os);
    }else if (respMesDesc.m_EnumMSGID == TSMessageDefs::TS_REGISTER_PLAYER_REQUEST){
        std::cout << "creating a register player response" << std::endl; 
        createRegisterPlayerResponse(getPublicPlayerInfo(), os);
    }else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_LIST_PLAYER_REQUEST){
        std::cout << "Creating a playerlist response " << std::endl;
        createListPlayersResponse(getPublicPlayerInfo(), os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_GROUP_REQUEST){
        std::cout << "creating a createGroupResponse" << std::endl;
        createThresholdGroupResponse(getPublicPlayerInfo(),respMesDesc.m_grpID, true,os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_INVITE_TO_GROUP_REQUEST){
        std::cout << "Creating a GroupInvite Response " << std::endl;        
        if(respMesDesc.m_userID.empty() && respMesDesc.m_grpID.empty()){
            std::cout << "No group details to build invite response from" << std::endl;
            return false;
        }
        std::pair<std::string, bool> inviteInfo = acceptInvitationToGroup(respMesDesc.m_grpID); 
        createInviteToGroupResponse(respMesDesc.m_grpID,inviteInfo.first,inviteInfo.second,os);
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_BROADCAST_GROUP_DETAILS_REQUEST){
        std::cout << "Create a broadcast group response" << std::endl;
        createBroadCastGroupDetailsResponse(respMesDesc.m_grpID,respMesDesc.m_userID,true,os);
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
        
    } else if(respMesDesc.m_EnumMSGID == TSMessageDefs::TS_CREATE_PLAYER_SECRET_REQUEST){        
        const int& myOrdinal = ordinalForGroup(respMesDesc.m_grpID);
        const jvrss& jvrssobj = PlayerSecretDataForGrp(respMesDesc.m_grpID, respMesDesc.m_SSType);
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
    for(std::vector<std::string>::const_iterator grpMemIter = grp.participantList().begin(); grpMemIter != grp.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(*grpMemIter);
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
            createInviteToGroupMessage(proposer,grpID, os);
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
    for (std::vector<std::string>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(*grpMemIter);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        boost::asio::streambuf b;
        std::ostream os(&b);
        std::cout << "SendGroupDetails .. calling CreateBroadCastGroupDetails" << std::endl;
        createBroadCastGroupDetails(iter->second,os); 
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
    //std::cout << "Group Details in SendGroupDetails\n" << grpinfo << std::endl;
    for (std::vector<std::string>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(*grpMemIter);
        if(playersIter == GlobalUsers::Instance()->users().end()){
            std::cout << "No contact details for player " << *grpMemIter << " registered in group " << grp << std::endl;
            return false;
        }
        if(userID != playersIter->second.userID()){
            boost::asio::streambuf b;
            std::ostream os(&b);
            std::cout << "SendGroupDelete" << std::endl;
            createDeleteTSPlayerGroupRequest(userID,grp,os); 
            std::cout << "Sending SendGroupDelete details to " << playersIter->second.addr() << "," << playersIter->second.port() << std::endl;
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
    for (std::vector<std::string>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(*grpMemIter);
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
    for (std::vector<std::string>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(*grpMemIter);
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
    for (std::vector<std::string>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(*grpMemIter);
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
    for (std::vector<std::string>::const_iterator grpMemIter = iter->second.participantList().begin(); grpMemIter != iter->second.participantList().end(); ++ grpMemIter){
        players::const_iterator playersIter = GlobalUsers::Instance()->users().find(*grpMemIter);
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

