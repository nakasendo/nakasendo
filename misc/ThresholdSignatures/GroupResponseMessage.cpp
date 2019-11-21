// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * A group response message.
 */

#include "GroupResponseMessage.h"
#include <single_include/nlohmann/json.hpp>


// Default constructor.
GroupResponseMessage::GroupResponseMessage()
: Base{Type::GROUP_RESPONSE}
{
}

// Construct setup message from a given proposer.
GroupResponseMessage::GroupResponseMessage(const std::string& uri,
                                           const UUID&        groupID,
                                           GroupState         response)
: Base{Type::GROUP_RESPONSE}, mResponderUri{uri}, mGroupID{groupID}, mResponse{response}
{
}

// Get a unique identifer for this JSONSerialisable type.
const std::string GroupResponseMessage::jsonObjectType() const
{
    return "GroupResponseMessage";
}  

std::string  GroupResponseMessage::to_json()
{
    //std::string baseJSONStr = Base::to_json();
    nlohmann::json j;
    j["Type"] =  enum_cast<std::string>(mType);
    j["Responder"] = mResponderUri;
    j["GroupID"] = mGroupID.getString();
    j["Response"] = enum_cast<std::string>(mResponse);
    //return baseJSONStr+j.dump();
    return j.dump();
}
        
void GroupResponseMessage::from_json(const std::string& groupJson)
{
    // Base class
    //Base::from_json(groupJson);
    nlohmann::json j = nlohmann::json::parse(groupJson);
    mType = enum_cast<Type>(j.at("Type").get<std::string>());
    mResponderUri = j.at("Responder").get<std::string>();
    mGroupID = j.at("GroupID").get<std::string>();
    mResponse  = enum_cast<GroupState>(j.at("Response").get<std::string>());
}
