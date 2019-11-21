// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * Base class for communications messages.
 */

#include "Message.h"
#include <single_include/nlohmann/json.hpp>

std::string  Message::to_json()
{
    nlohmann::json j;

    j["Type"] =  enum_cast<std::string>(mType);
    return j.dump();
}

void Message::from_json(const std::string& typeJson)
{
    nlohmann::json j = nlohmann::json::parse(typeJson);
    mType = enum_cast<Type>(j.at("Type").get<std::string>());
}
