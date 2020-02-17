/*g++ -o TSExample -I. -I /Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/src/core -I /Users/j.murphy/nChain/SDK-CLEAN/build/generated/hpp -I /Users/j.murphy/nChain/SDK-CLEAN/build/generated/protobuf -I /Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/src/applications/TS_protobuf/cpp TSExample.cpp -L/Users/j.murphy/nChain/SDK-CLEAN/build/x64/release -lTSCore -lBigNumbers -lECPoint -lPolynomial -lAsymKey -lprotobuf -ldl -pthread -std=c++17*/

#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <orchestrator.h>
#include <GroupMetaData.h>
#include <TSState.h>
#include <ECPoint/ECPoint.h>
#include <BigNumbers/BigNumbers.h>
#include <AsymKey/AsymKey.h>

std::string random_string( std::size_t length )
{
    static const std::string alphabet = "abcdefghijklmnopqrstuvwxyz" ;
    static std::default_random_engine rng( std::time(nullptr) ) ;
    static std::uniform_int_distribution<std::size_t> distribution( 0, alphabet.size() - 1 ) ;

    std::string str ;
    while( str.size() < length ) str += alphabet[ distribution(rng) ] ;
    return str ;
}

using playerGroups = std::map<std::string, playerGroupMetaData>; 

class ExtendedPlayer : public player
{
    public:
        ExtendedPlayer(const std::string& pid, const std::string& uri, const std::string& addr, const std::string& port)
            : player(pid,uri,addr,port)
        { return ;}
        
        playerGroups& PlayerGroup() { return m_PlayerGroup;}
        void addPlayerGroup(playerGroupMetaData& obj){
            m_PlayerGroup[obj.m_id] = obj;
        }
        
    private:
        playerGroups m_PlayerGroup;
};

int GenerateRandomRange (const int& lowerVal, const int& upperVal){
    BigNumber lower;
    BigNumber upper;
    
    lower.FromDec(std::to_string(lowerVal));
    upper.FromDec(std::to_string(upperVal));
    BigNumber res = GenerateRandRange(lower,upper);
    return std::stoi(res.ToDec());
} 

void ShareSecret(GroupMetadata& glbgrp, std::vector<ExtendedPlayer>& players, const std::string& calctype, const int& t, const BigNumber& mod){
    if(calctype == "LITTLEK" || calctype == "ALPHA"){
        for(std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++ iter){
            Polynomial poly(t, mod);
            playerGroupMetaData& pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
            pgrp.polynomialPreCalculation(poly);
        }
    }
    // for each player, add the hidden polynomial & ordinal to the global group
    // add the ordinal + a list of the poly evals to the global group
    
    for(std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++ iter){
        playerGroupMetaData& pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        std::vector<std::string> hpcoeffs;
        for(std::vector<ECPoint>::const_iterator hpIter = pgrp.m_transientData.m_hiddenPolynomial.begin(); hpIter != pgrp.m_transientData.m_hiddenPolynomial.end(); ++ hpIter){
            hpcoeffs.push_back(hpIter->ToHex());
        }
        glbgrp.addCollatedHiddenPolys(std::to_string(pgrp.m_ordinal),hpcoeffs);
        
        
        for(std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::iterator heIter = pgrp.m_transientData.m_hiddenEvals.begin(); heIter != pgrp.m_transientData.m_hiddenEvals.end(); ++ heIter){
            for(std::vector<std::pair<std::string,ECPoint> >::iterator innerHeIter = heIter->second.begin();
                    innerHeIter != heIter->second.end(); ++ innerHeIter){
                        glbgrp.addCollatedHiddenEvals(std::to_string(pgrp.m_ordinal),innerHeIter->first,innerHeIter->second.ToHex());
            }
        }
    }
    
    // share the evals between the players
    // please note these values DO NOT go to the global group
    for(int pi=0;pi < players.size(); ++ pi){
        playerGroupMetaData& pipgrp = players[pi].PlayerGroup().find(glbgrp.groupid())->second;
        int piOrd = pipgrp.m_ordinal;
        for(int pj = 0; pj<players.size(); ++pj){
            if(pi != pj){
                playerGroupMetaData& pjgrp = players[pj].PlayerGroup().find(glbgrp.groupid())->second;
                int pjOrd = pjgrp.m_ordinal;
                std::map<std::string, BigNumber>::const_iterator iter = pipgrp.m_transientData.m_evals.find(std::to_string(pjOrd));
                if(iter != pipgrp.m_transientData.m_evals.end())
                    pjgrp.addPublicEvalsToJVRSS(std::to_string(piOrd),iter->second.ToHex());
            }
        }
    }
    
    // grab the hidden poly information & hidden eval info from the global group
    // in a real application this is achieved via a messaging layer
    for(std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++ iter){
        playerGroupMetaData& pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        for(std::map<std::string, std::vector<ECPoint> >::const_iterator hpIter = glbgrp.CollatedHiddenPolys().begin();
                hpIter != glbgrp.CollatedHiddenPolys().end(); ++ hpIter){
                std::vector<std::string> hpStrs;
                for(std::vector<ECPoint>::const_iterator ecPtiter = hpIter->second.begin(); ecPtiter != hpIter->second.end(); ++ecPtiter){
                    hpStrs.push_back(ecPtiter->ToHex());
                }
                pgrp.addHiddenPolynomialToJVRSS(hpIter->first,hpStrs);
        }
        
        for(std::map<std::string, std::vector<std::pair<std::string, ECPoint> > >::const_iterator iter = glbgrp.CollatedHiddenEvals ().begin();
                iter != glbgrp.CollatedHiddenEvals ().end(); ++ iter){
                
            std::vector<std::pair<std::string, std::string> > he;
            for(std::vector<std::pair<std::string,ECPoint> >::const_iterator innerIter = iter->second.begin(); innerIter != iter->second.end(); ++innerIter){
                he.push_back(std::make_pair(innerIter->first,innerIter->second.ToHex()));   
            }
            pgrp.addHiddenEvalsToJVRSS(iter->first, he);
        }
    }
    
    
    
    
    for(std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++ iter){
        playerGroupMetaData& pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        pgrp.verifyCorrectness();
        pgrp.verifyHonesty();
    }
    
    for(std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++ iter){
        playerGroupMetaData& pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        
        BigNumber secretShare = pgrp.createSecret();
        if(calctype == "PRIVATEKEYSHARE"){
            pgrp.m_privateKeyShare = secretShare;
            pgrp.m_GroupPublicKey = pgrp.createGroupPublicKey();
        }else if(calctype == "LITTLEK"){
            pgrp.m_littleK = secretShare;
        }else if(calctype == "ALPHA"){
            pgrp.m_alpha = secretShare;
        }
    }
    
    for(std::vector<ExtendedPlayer>::iterator iter = players.begin(); iter != players.end(); ++ iter){
        playerGroupMetaData& pgrp = iter->PlayerGroup().find(glbgrp.groupid())->second;
        pgrp.m_transientData.reset();
    }
    glbgrp.clearSharedDataContainers();
    return;
}

int main(int argc, char * argv[]){
    //GOOGLE_PROTOBUF_VERIFY_VERSION;
    std::cout << "Starting TSExample test" << std::endl;
    BigNumber mod; 
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"); 
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1,6);
    
    int tt =  GenerateRandomRange(1,6);// generates number in the range 1..6
    int margin = GenerateRandomRange(1,5);
    int nn = 2*tt+1 + margin;   
    
    
    // Register players with the global list
    for (int i=0;i<(2*nn);++i){
        player p(random_string(10),"","127.0.0.1","808" + std::to_string(i));
        addPlayer(p);
    }
    
    //print a list of players
    const std::vector<player> players = getPlayerList();
    for(std::vector<player>::const_iterator iter = players.begin(); iter != players.end(); ++ iter){
        std::cout << *iter << std::endl;
    }
    
    //pick a random player to propose a group
    
    
    int propserIndex = GenerateRandomRange(0,players.size()-1);
    std::cout << "propser " << players[propserIndex] << std::endl;
    
    std::cout << "Creating a group with parameters m = " << tt+1 << " nn = " << nn << std::endl;
    std::string grpID = CreateGroup(players[propserIndex].userID(), tt+1, nn);
    std::cout << grpID << std::endl;
    
    
    // At this point, invite a subset of players to the group , or send an invite to all of the registered players
    // and add them if they respond in a positive way
    // This piece of code is purely for illustrative purposes only but the concept is the same as communicating 
    // with a distributed client
    
    std::vector<int> indexOfPlayers;
    indexOfPlayers.push_back(propserIndex);
    while (indexOfPlayers.size() < nn){
        int index = GenerateRandomRange(0,players.size()-1);
        if(std::find(indexOfPlayers.begin(),indexOfPlayers.end(),index) == indexOfPlayers.end())
            indexOfPlayers.push_back(index);
    }

    // add the players to the global group
    for(std::vector<int>::const_iterator iter = indexOfPlayers.begin(); iter != indexOfPlayers.end(); ++ iter){
        if(*iter != propserIndex)
            addUserToGroup(grpID, players[*iter].userID());
    }
    
    std::cout << GetGroup(grpID) << std::endl;
    
    // this point, distributed users would be online and in a position to participate in rounds of MPC
    // the data structure below mimics that. 
    // I've created a class called ExtendedPlayer to allow for  multiple SinglePlayer objects to be created within a single process for explanation purporse. 
    
    std::vector<ExtendedPlayer> distributedplayers; 
    for(std::vector<int>::const_iterator iter = indexOfPlayers.begin(); iter != indexOfPlayers.end(); ++ iter){
        ExtendedPlayer p (players[*iter].userID(), players[*iter].uri(),players[*iter].addr(), players[*iter].port());
        distributedplayers.push_back(p);
    }
    
    // each player in the group receives a copy of the global group. The class that models this is called
    // PlayerGroupMetadata
    
    const GroupMetadata& grp = GetGroup(grpID);
    
    for(std::vector<ExtendedPlayer>::iterator iter = distributedplayers.begin(); 
            iter != distributedplayers.end(); ++ iter){
            
         playerGroupMetaData pgrp (tt,mod);
         pgrp.m_id = grpID;
         int ordinal=1;
         const std::vector<playerCommsInfo>& playerinfo = grp.participantList();
         for(std::vector<playerCommsInfo>::const_iterator grpPlayersIter = playerinfo.begin(); 
                grpPlayersIter != playerinfo.end(); ++ grpPlayersIter){
         
             if(iter->userID() == grpPlayersIter->m_userid){
                pgrp.m_ordinal = ordinal++;
             }else{
                player p (grpPlayersIter->m_userid,grpPlayersIter->m_uri,grpPlayersIter->m_ip,grpPlayersIter->m_port);
                pgrp.m_ordinalAndPlayerList.push_back(std::make_pair(ordinal,p));
                pgrp.m_ordinalList.push_back(ordinal);
                ++ ordinal; 
             }
        }
        pgrp.polynomialPreCalculation(pgrp.m_privateKeyPolynomial);
        iter->addPlayerGroup(pgrp);
    }
    
    // to view the list of groups a player is in from the point of view of the player
    
    
    ShareSecret(GetGroup(grpID),distributedplayers,"PRIVATEKEYSHARE",tt,mod);
    // check the public key for each player
    for(std::vector<ExtendedPlayer>::iterator iter = distributedplayers.begin(); iter != distributedplayers.end(); ++ iter){
        playerGroupMetaData& pgrp = iter->PlayerGroup().find(grpID)->second;
        std::cout << pgrp.m_ordinal << " " << pgrp.m_GroupPublicKey.ToHex() << std::endl;
    }
    
    
    
    // calculate a bag of ephemeral keys
    int numEKeys(1);
    for(int i=0;i<numEKeys;++i){
        ShareSecret(GetGroup(grpID),distributedplayers,"LITTLEK",tt,mod);
        ShareSecret(GetGroup(grpID),distributedplayers,"ALPHA",tt,mod);
        
        // push the VW calculation to the global group for each player
        for(std::vector<ExtendedPlayer>::iterator iter = distributedplayers.begin(); iter != distributedplayers.end(); ++ iter){
            playerGroupMetaData& pgrp = iter->PlayerGroup().find(grpID)->second;
            BigNumber kalpha = (pgrp.m_littleK * pgrp.m_alpha);
            ECPoint alphaG = MultiplyByGeneratorPt(pgrp.m_alpha);
            GetGroup(grpID).addCollatedVW(std::to_string(pgrp.m_ordinal),kalpha.ToHex(),alphaG.ToHex());
        }
        
        for(std::vector<ExtendedPlayer>::iterator iter = distributedplayers.begin(); iter != distributedplayers.end(); ++ iter){
            playerGroupMetaData& pgrp = iter->PlayerGroup().find(grpID)->second;
            for(std::map<std::string, std::pair<BigNumber, ECPoint> >::const_iterator iter = GetGroup(grpID).CollatedVW().begin(); 
                        iter != GetGroup(grpID).CollatedVW().end(); ++ iter){
               pgrp.addPreSignDataToJVRSS(iter->first, iter->second.first.ToHex(), iter->second.second.ToHex());
            }       
            if(!pgrp.CalculateEphemeralKey()){
                std::cout << "Failed to calculate an emphemeral key" << std::endl;
            }
        }
         GetGroup(grpID).clearPreSignatureContainers();
    }
    
    //for(std::vector<ExtendedPlayer>::iterator iter = distributedplayers.begin(); iter != distributedplayers.end(); ++ iter){
    //    std::cout << iter->PlayerGroup().find(grpID)->second << std::endl;
    //}
    
   
    // pick a subset of players to form a signature
    std::string msg ("I love deadlines. I love the whooshing noise they make as they go by.");
    std::vector<int> playerSigIndex;
    while(playerSigIndex.size () < (2*tt)){
        int index = GenerateRandomRange (0,distributedplayers.size()-1);
        if(std::find(playerSigIndex.begin(),playerSigIndex.end(),index) == playerSigIndex.end()){
            playerSigIndex.push_back(index);
        }
    }
    
    for(int i=0;i<playerSigIndex.size();++i){
        std::pair<std::string, BigNumber> sig = distributedplayers[playerSigIndex[i]].PlayerGroup().find(grpID)->second.CalcPartialSignature(msg,0);
        GetGroup(grpID).addCollatedPartialSignautre(sig.first,sig.second.ToHex());
    }
    
    // pick one more player who will finish the signature
    int playerCompSigIndex = 1;
    while(std::find(playerSigIndex.begin(),playerSigIndex.end(),playerCompSigIndex) != playerSigIndex.end()){
        playerCompSigIndex = GenerateRandomRange (0,distributedplayers.size()-1);
    }
    
    std::cout << "Player at index " << playerCompSigIndex << " will complete the signature" << std::endl;
    
    
    const std::vector<std::pair<std::string,BigNumber> >& partialSigs =   GetGroup(grpID).CollatedPartialSignatures();
    std::vector<std::pair<std::string, std::string> > partSig;
    for(std::vector<std::pair<std::string,BigNumber> >::const_iterator iter = partialSigs.begin();iter != partialSigs.end(); ++ iter){
        partSig.push_back(std::make_pair(iter->first,iter->second.ToHex()));
    }
    
    std::pair<BigNumber, BigNumber> signature = distributedplayers[playerCompSigIndex].PlayerGroup().find(grpID)->second.CalculateGroupSignature(msg,0,partSig);
    
    
    GetGroup(grpID).clearSignatureContainers();
    std::cout << signature.first.ToHex() << " " << signature.second.ToHex() << std::endl; 
   
    std::pair<std::string, std::string> pubkeypoints = distributedplayers[playerCompSigIndex].PlayerGroup().find(grpID)->second.m_GroupPublicKey.GetAffineCoords_GFp();
    
    std::pair<std::string, std::string> sigAsStr (signature.first.ToHex(),signature.second.ToHex());
    std::string pubkeypem = pubkey_coordinates2pem(pubkeypoints.first,pubkeypoints.second);
    if(verify(msg,pubkeypem,sigAsStr))
        std::cout << "Message verified" << std::endl;
        
    // Should you wish to create the private key from the group.
    std::vector<int> playerKeyGenIndex;
    while(playerKeyGenIndex.size() < tt){
        int index = GenerateRandomRange (0,distributedplayers.size()-1);
        if(std::find(playerKeyGenIndex.begin(),playerKeyGenIndex.end(),index) == playerKeyGenIndex.end()){
            std::cout << index << std::endl;
            playerKeyGenIndex.push_back(index);
        }
    }
    
    int playerIndexCompleteKeyGen=1;
    while(std::find(playerKeyGenIndex.begin(),playerKeyGenIndex.end(),playerIndexCompleteKeyGen) != playerKeyGenIndex.end()){
        playerIndexCompleteKeyGen = GenerateRandomRange (0,distributedplayers.size()-1);
    }
    
    
    std::cout << "This player will generate a private key with private data from the other players in the group " << playerIndexCompleteKeyGen << std::endl;
    for(int i=0;i<playerKeyGenIndex.size();++i){
        std::string pord = std::to_string(distributedplayers[playerKeyGenIndex[i]].PlayerGroup().find(grpID)->second.m_ordinal);
        BigNumber pKeyShare = distributedplayers[playerKeyGenIndex[i]].PlayerGroup().find(grpID)->second.privateKeyShare();
        
        
        distributedplayers[playerIndexCompleteKeyGen].PlayerGroup().find(grpID)->second.addPrivateKeyInfo(pord,pKeyShare.ToHex());

    }
    
    BigNumber privatekey = distributedplayers[playerIndexCompleteKeyGen].PlayerGroup().find(grpID)->second.CalculateGroupPrivateKey();
    if(distributedplayers[playerIndexCompleteKeyGen].PlayerGroup().find(grpID)->second.ValidateGroupPrivateKey(privatekey))
        std::cout << "Private key calculated and verified " << privatekey.ToHex() << std::endl;
    
}

