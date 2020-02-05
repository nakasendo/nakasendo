#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include <vector>
#include <map>
#include <orchestrator.h>
#include <player.h>
#include <GroupMetaData.h>
#include <BigNumbers/BigNumbers.h>
#include <MessageHash/MessageHash.h>
// utility functions to send & receive messages to the clients
#include <TSMessageFactory.h>


std::unique_ptr<GlobalUsers> GlobalUsers::m_Instance;
std::once_flag GlobalUsers::m_onceFlag;

std::unique_ptr<GlobalGroups> GlobalGroups::m_Instance;
std::once_flag GlobalGroups::m_onceFlag;

std::unique_ptr<GlobalUsers>& GlobalUsers::Instance(){
    std::call_once(m_onceFlag, []{m_Instance.reset(new GlobalUsers);}); 
    return m_Instance; 
 }
 
void GlobalUsers::addPlayer(const player& obj){
    std::lock_guard<std::mutex> guard(m_users_mutex);
    std::map<std::string, player>::iterator iter (m_users.find(obj.userID()));
    if(iter == m_users.end())
        m_users[obj.userID()] = obj;
        
    return ; 
    
}

void GlobalUsers::removePlayer(const player& obj){
    std::lock_guard<std::mutex> guard(m_users_mutex);
    m_users.erase(obj.userID()); 
}

std::vector<player> GlobalUsers::getPlayerList(){
    std::vector<player> players;
    std::lock_guard<std::mutex> guard(m_users_mutex);
    for(std::map<std::string, player>::const_iterator iter = m_users.begin(); iter != m_users.end(); ++ iter){
        players.push_back(iter->second); 
    }
    return players;
}

const player& GlobalUsers::getPlayer(const std::string& p){
    std::lock_guard<std::mutex> guard(m_users_mutex);
    std::map<std::string, player>::const_iterator iter =  m_users.find(p);
    if(iter == m_users.end()){
        throw std::runtime_error("No users registered with name " + p);
    }
    return iter->second; 
}

void addPlayer (const player& obj){

    GlobalUsers::Instance()->addPlayer(obj);
    return ;      
}

void removePlayer(const player& obj){
    GlobalUsers::Instance()->removePlayer(obj);
    return;
}

std::vector<player> getPlayerList(){
    return GlobalUsers::Instance()->getPlayerList();
}

const player&  getPlayer(const std::string& p){
    return GlobalUsers::Instance()->getPlayer(p);
}

std::unique_ptr<GlobalGroups>& GlobalGroups::Instance(){
    std::call_once(m_onceFlag, []{m_Instance.reset(new GlobalGroups);}); 
    return m_Instance; 
}
 
void GlobalGroups::addGroup(const GroupMetadata& grp){
    std::lock_guard<std::mutex> guard(m_groups_mutex);
    m_groups[grp.groupid()] = grp; 
}

void GlobalGroups::addUserToGroup(const std::string& grpid, const std::string& userid){
    std::lock_guard<std::mutex> guard(m_groups_mutex);
    groupsMap::iterator grpIter = m_groups.find(grpid);
    player p = getPlayer( userid ) ;
    if(grpIter != m_groups.end()){
        grpIter->second.addUserToGroup(userid, p.uri(), p.addr(), p.port()); 
    }
}

bool GlobalGroups::removeUserFromGroup(const std::string& grpid, const std::string& userid){
    bool removed(false);
    std::lock_guard<std::mutex> guard(m_groups_mutex);
    groupsMap::iterator grpIter = m_groups.find(grpid);
    if(grpIter != m_groups.end()){
        removed=grpIter->second.removeUserFromparticipantList(userid);
    }
    return removed; 
}

bool GlobalGroups::deleteGroup(const std::string& grpid){
    bool deleted(false);
    std::lock_guard<std::mutex> guard(m_groups_mutex);
    groupsMap::iterator grpIter = m_groups.find(grpid);
    if(grpIter != m_groups.end()){
        m_groups.erase(grpid);
        deleted = !deleted;
    }
    return(deleted);
}

GroupMetadata& GlobalGroups::GetGroup(const std::string& grpid){
    std::lock_guard<std::mutex> guard(m_groups_mutex);
    groupsMap::iterator grpIter = m_groups.find(grpid);
    if(grpIter == m_groups.end()){
        throw std::runtime_error("No group corresponding to group id: " + grpid);
    }
    return grpIter->second;
}

void addGroup(const GroupMetadata& grp){
    GlobalGroups::Instance()->addGroup(grp);
    return; 
}


void addUserToGroup(const std::string& grpid, const std::string& userid ){
    GlobalGroups::Instance()->addUserToGroup(grpid,userid);
    return;
}

bool removeUserFromGroup(const std::string& grpid, const std::string& userid){
    return  GlobalGroups::Instance()->removeUserFromGroup(grpid,userid);
}

bool deleteGroup(const std::string& grpid){
    return GlobalGroups::Instance()->deleteGroup(grpid);
}

GroupMetadata&  GetGroup(const std::string& grpid){
    return GlobalGroups::Instance()->GetGroup(grpid);
}

const std::string CreateGroup(const std::string& proposer, const int& m, const int& n){
    
    int t = m-1;
    if (t<1){
        std::stringstream errMsg;
        errMsg << "Parameters are not valid! Failed check: t (degree of Polynomial) < 1";
        throw std::runtime_error(errMsg.str()); 
    }
    
    if(2*t +1 > n){
        std::stringstream errMsg;
        errMsg << "Parameters are not valid! Failed check: 2t + 1 <= n";
        throw std::runtime_error(errMsg.str()); 
    }
    
    if(n<=2){
        std::stringstream errMsg;
        errMsg << "Parameters are not valid! Failed check: 2t + 1 <= n";
        throw std::runtime_error(errMsg.str()); 
    }
    
    
    std::string grpID = generateUUID(); 
    playerCommsInfo pinfo;
    pinfo.m_userid = proposer;
    const player& p = getPlayer(proposer);
    pinfo.m_uri = p.uri();
    pinfo.m_ip = p.addr();
    pinfo.m_port = p.port();

    GroupMetadata grp(grpID,pinfo,m,n,t);

    GlobalGroups::Instance()->addGroup(grp);
    
    return grpID ;
}

bool DeleteGrpSharingContainers(const std::string& grp){
    groupsMap::iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    
    GroupMetadata& grpinfo = iter->second;
    grpinfo.clearSharedDataContainers(); 
    return true; 
}

bool deleteGrpSigSharingContainers(const std::string& grp){
    groupsMap::iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    
    GroupMetadata& grpinfo = iter->second;
    grpinfo.clearSignatureContainers(); 
    return true; 
}

bool deleteGrpPreSigContainers(const std::string& grp){
    groupsMap::iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    
    GroupMetadata& grpinfo = iter->second;
    grpinfo.clearPreSignatureContainers(); 
    return true;
} 

bool lockGrp(const std::string& grp){
    groupsMap::iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }

    GroupMetadata& grpinfo = iter->second;
    if(grpinfo.isLocked()){
        std::cout << "Group already locked. Please try again later" << std::endl;
        return false;
    }
    grpinfo.LockGroup(); 
    return true;
}
bool unlockGrp(const std::string& grp ){
    groupsMap::iterator iter = GlobalGroups::Instance()->groups().find(grp);
    if(iter == GlobalGroups::Instance()->groups().end()){
        std::cout << "no grp listed for id: " << grp; 
        return false;
    }
    
    GroupMetadata& grpinfo = iter->second;
    grpinfo.unLockGroup(); 
    return true;

}
std::string generateUUID(const int& length){
    BigNumber ranVal;
    std::string uuidVal = ranVal.generateRandHex(length * 8);
    MessageHash hashMess;
    hashMess.HashSha256 (uuidVal);
    return hashMess.HashHex();
}



