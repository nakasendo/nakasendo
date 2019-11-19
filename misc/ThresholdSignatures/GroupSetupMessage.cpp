// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * A group setup message.
 */

#include "GroupSetupMessage.h"
#include <single_include/nlohmann/json.hpp>

// Default constructor.
GroupSetupMessage::GroupSetupMessage()
: Base{Type::GROUP_SETUP}
{
}

// Construct setup message from a given proposer.
GroupSetupMessage::GroupSetupMessage(const std::string&      uri,
                                     const UUID&             groupID,
                                     const Group::PlayerMap& players)
: Base{Type::GROUP_SETUP}, mProposerUri{uri}, mGroupID{groupID}, mPlayers{players}
{
}

// Get a unique identifer for this JSONSerialisable type.
const std::string GroupSetupMessage::jsonObjectType() const
{
    return "GroupSetupMessage";
}  

std::string GroupSetupMessage::to_json()
{
    //std::string baseJSONStr = Base::to_json();
    nlohmann::json j;
    j["Type"] =  enum_cast<std::string>(mType);
    j["Proposer"] = mProposerUri;
    j["GroupID"] = mGroupID.getString();

    std::vector<std::string> playerList;
    for(const auto& player : mPlayers)
    {
        std::string playerString = Player::to_json(player.second);
        playerList.push_back(playerString);
    }
    j["Players"] = playerList;
    //return baseJSONStr+j.dump();
    return j.dump();
}

        
void GroupSetupMessage::from_json(const std::string& groupJson)
{
    //Base::from_json(groupJson);
    nlohmann::json j = nlohmann::json::parse(groupJson);
    mType = enum_cast<Type>(j.at("Type").get<std::string>());
    mProposerUri = j.at("Proposer").get<std::string>();
    mGroupID = j.at("GroupID").get<std::string>();
    
    // Parse players
    mPlayers.clear();
    for(auto& jsonPlayer : j["Players"])
    {
        Player player = Player::from_json(jsonPlayer); 
        mPlayers.emplace(std::make_pair(player.getURI(), player));
    }
}



