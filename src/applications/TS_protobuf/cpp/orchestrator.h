#ifndef __ORCHESTRATOR_H__
#define __ORCHESTRATOR_H__



#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <player.h>
#include <exportTSCore.h>




class GroupMetadata;

using players = std::map<std::string, player>; 
class TSCore_API GlobalUsers
{
    public:
        
        static std::unique_ptr<GlobalUsers>& Instance(); 
        const std::map<std::string, player>& users () const { return m_users ;}
        void addPlayer(const player&);
        void removePlayer(const player&);
        std::vector<player> getPlayerList();
        const player& getPlayer(const std::string& ); 

    private:
    
        GlobalUsers()=default;
        GlobalUsers(const GlobalUsers&) = delete;
        GlobalUsers& operator=(const GlobalUsers&) = delete; 
        static std::unique_ptr<GlobalUsers> m_Instance; 
        static std::once_flag m_onceFlag;
        
        std::map<std::string, player> m_users;
        std::mutex m_users_mutex;
    
};

TSCore_API void addPlayer (const player& obj);
TSCore_API void removePlayer(const player& obj);
TSCore_API std::vector<player> getPlayerList();
TSCore_API const player& getPlayer(const std::string&) ;


using groupsMap = std::map<std::string, GroupMetadata>;
class TSCore_API GlobalGroups
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


TSCore_API const std::string CreateGroup(const std::string&, const int&, const int&);
TSCore_API void addUserToGroup(const std::string&, const std::string& );
TSCore_API GroupMetadata&  GetGroup(const std::string&);
TSCore_API bool removeUserFromGroup(const std::string&, const std::string&);
TSCore_API bool deleteGroup(const std::string&);

TSCore_API bool DeleteGrpSharingContainers(const std::string&);
TSCore_API bool deleteGrpSigSharingContainers(const std::string&);
TSCore_API bool deleteGrpPreSigContainers(const std::string&);

TSCore_API bool lockGrp(const std::string&);
TSCore_API bool unlockGrp(const std::string&);

TSCore_API std::string generateUUID(const int& length=32);

#endif //iifndef __ORCESTRATOR_H__


