// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * Builder for groups.
 */

#include "GroupBuilder.h"
#include <single_include/nlohmann/json.hpp>

#include <stdexcept>


// Default constructor.
GroupBuilder::GroupBuilder()
{
}

// Construct for the given list of players.
GroupBuilder::GroupBuilder(const std::vector<std::string>& playerUris, const std::string& myUri)
: mState { GroupState::PROPOSED }, mProposerUri {myUri}, mMyUri {myUri}
{
    // Need to check proposer is one of the players
    bool proposerIsPlayer {false};

    // Create players with supplied URIs and distinct ordinals
    Ordinal ordinal {1};
    for(const std::string& uri : playerUris)
    {
        std::pair<std::string,Player> player { std::make_pair(uri, Player { uri, ordinal++ }) };
        if(!proposerIsPlayer && uri == mMyUri)
        {
            proposerIsPlayer = true;
            // Proposer is ACCEPTED right away
            player.second.setAccepted();
        }
        auto res { mPlayers.emplace(std::move(player)) };
        if(!res.second)
        {
            throw std::runtime_error("Failed to create and insert player details for " + uri);
        }
    }

    if(!proposerIsPlayer)
    {
        throw std::invalid_argument("The group proposer must be a member of the proposed group");
    }
}

// Construct from a group setup message.
GroupBuilder::GroupBuilder(const GroupSetupMessageSPtr& setupMsg, const std::string& myUri)
: mState { GroupState::PROPOSED }, mGroupID { setupMsg->getGroupID() }, mPlayers { setupMsg->getPlayers() },
  mProposerUri { setupMsg->getProposerURI() }, mMyUri {myUri}
{
    // Check our URI is in the list of players
    const auto player { mPlayers.find(mMyUri) };
    if(player == mPlayers.end())
    {
        throw std::runtime_error("Our URI not found in group setup message players");
    }
}

// Get the group members.
Group::PlayerMap GroupBuilder::getPlayers() const
{
    std::lock_guard<std::mutex> lck { mMtx };
    return mPlayers;
}

// Get the Group we are building.
GroupSPtr GroupBuilder::getGroup() const
{
    std::lock_guard<std::mutex> lck { mMtx };

    // Will only create a group if we've heard back ok from all players
    if(mState != GroupState::ACCEPTED)
    {
        throw std::runtime_error("Can't create group in state " +
            enum_cast<std::string>(mState));
    }

    return std::make_shared<Group>(mGroupID, mPlayers);
}

// Get group proposal messages for all other group members.
GroupSetupMessageSPtr GroupBuilder::getGroupSetupMessage() const
{
    std::lock_guard<std::mutex> lck { mMtx };
    return std::make_shared<GroupSetupMessage>(mProposerUri, mGroupID, mPlayers);
}

// Get a group response message for all other group members.
GroupResponseMessageSPtr GroupBuilder::getGroupSetupResponseMessage(GroupState state)
{
    // Set our state
    if(state != GroupState::ACCEPTED && state != GroupState::REJECTED)
    {
        throw std::invalid_argument("Group response must be either ACCEPTED or REJECTED");
    }

    // Create and process our response immediately
    GroupResponseMessageSPtr response { std::make_shared<GroupResponseMessage>(mMyUri, mGroupID, state) };
    processGroupSetupResponse(response);
    return response;
}

// Process group proposal message response.
void GroupBuilder::processGroupSetupResponse(const GroupResponseMessageSPtr& response)
{
    std::lock_guard<std::mutex> lck { mMtx };

    // Is response from someone in our group?
    if(!(response->getGroupID() == mGroupID))
    {
        throw std::runtime_error("Group response message is not for our group");
    }
    Group::PlayerMap::iterator player { mPlayers.find(response->getResponderURI()) };
    if(player == mPlayers.end())
    {
        throw std::runtime_error("Group response message is not from a proposed group member");
    }

    // Mark responding player as accepted or rejected
    if(response->getResponse() == GroupState::ACCEPTED)
    {
        player->second.setAccepted();

        // If all players have accepted, update our state
        if(mState != GroupState::REJECTED)
        {
            bool allAccepted {true};
            for(auto& player : mPlayers)
            {
                if(!player.second.accepted())
                {
                    allAccepted = false;
                    break;
                }
            }
            if(allAccepted)
            {
                mState = GroupState::ACCEPTED;
            }
        }
    }
    else if(response->getResponse() == GroupState::REJECTED)
    {
        player->second.setRejected();
        // Any rejection means we are rejected
        mState = GroupState::REJECTED;
    }
    else
    {
        throw std::runtime_error("Bad state in group response message: " +
            enum_cast<std::string>(response->getResponse()));
    }
}


// Get a unique identifer for this JSONSerialisable type.
const std::string GroupBuilder::jsonObjectType() const
{
    return "GroupBuilder";
}

std::string GroupBuilder::to_json()
{
    std::lock_guard<std::mutex> lck { mMtx };

    nlohmann::json j;
    j["ID"] = mGroupID.getString();
    j["State"] = enum_cast<std::string>(mState);
    j["ProposerURI"] = mProposerUri;
    j["MyURI"] = mMyUri;

    std::vector<std::string> playerList;
    for(const auto& player : mPlayers)
    {
        std::string playerString = Player::to_json(player.second);
	playerList.push_back(playerString);
    }
    j["Players"] = playerList;
    return j.dump();
}

void GroupBuilder::from_json(const std::string& groupJson)
{
    std::lock_guard<std::mutex> lck { mMtx };
    
    nlohmann::json j = nlohmann::json::parse(groupJson);
    mGroupID = j.at("ID").get<std::string>();
    mState = enum_cast<GroupState>(j.at("State").get<std::string>());
    mProposerUri = j.at("ProposerURI").get<std::string>();
    mMyUri = j.at("MyURI").get<std::string>();

    // Parse players
    mPlayers.clear();
    for(auto& jsonPlayer : j["Players"])
    {
        Player player = Player::from_json(jsonPlayer); 
	mPlayers.emplace(std::make_pair(player.getURI(), player));
    }
}
