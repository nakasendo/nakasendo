#ifndef __ORCHESTRATOR_H__
#define __ORCHESTRATOR_H__



#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <player.h>
class GroupMetadata;

using players = std::map<std::string, player>; 
class GlobalUsers
{
    public:
        
        static std::unique_ptr<GlobalUsers>& Instance(); 
        const std::map<std::string, player>& users () const { return m_users ;}
        void addPlayer(const player&);
        void removePlayer(const player&);
        std::vector<player> getPlayerList();
        
    private:
    
        GlobalUsers()=default;
        GlobalUsers(const GlobalUsers&) = delete;
        GlobalUsers& operator=(const GlobalUsers&) = delete; 
        static std::unique_ptr<GlobalUsers> m_Instance; 
        static std::once_flag m_onceFlag;
        
        std::map<std::string, player> m_users;
        std::mutex m_users_mutex;
    
};

void addPlayer (const player& obj);
void removePlayer(const player& obj);
std::vector<player> getPlayerList();


using groupsMap = std::map<std::string, GroupMetadata>;
class GlobalGroups
{
   public:
        
        static std::unique_ptr<GlobalGroups>& Instance(); 
        const groupsMap& groups () const { return m_groups ;}
        groupsMap& groups () { return m_groups ;}
        void addGroup(const GroupMetadata&);
        void addUserToGroup(const std::string&, const std::string&);
        bool removeUserFromGroup(const std::string&, const std::string&);
        bool deleteGroup(const std::string&);
        GroupMetadata& GetGroup(const std::string&);
        
    private:
    
        GlobalGroups()=default;
        GlobalGroups(const GlobalGroups&) = delete;
        GlobalGroups& operator=(const GlobalGroups&) = delete; 
        static std::unique_ptr<GlobalGroups> m_Instance; 
        static std::once_flag m_onceFlag;
        
        groupsMap m_groups;
        std::mutex m_groups_mutex;
}; 


const std::string CreateGroup(const std::string&, const int&, const int&); 
void addUserToGroup(const std::string&, const std::string&);
GroupMetadata& GetGroup(const std::string&);
bool removeUserFromGroup(const std::string&, const std::string&);
bool deleteGroup(const std::string&);

bool DeleteGrpSharingContainers(const std::string&);
bool deleteGrpSignatureSharingContainsers(const std::string&);
bool deleteGrpPreSignatureContainers(const std::string&);

bool lockGrp(const std::string&);
bool unlockGrp(const std::string&);

std::string generateUUID(const int& length=32); 

#endif //iifndef __ORCESTRATOR_H__


