
/*
 * A player within a threshold signature group.
 */

#include "Player.h"
#include <single_include/nlohmann/json.hpp>

// Constructor
Player::Player(const std::string& uri, Ordinal ord)
: mUri{uri}, mOrdinal{ord}
{
}

// Set player as accepted.
void Player::setAccepted()
{
    if(mRejected)
    {
        throw std::runtime_error("Attempt to set player as both accepted and rejected");
    }
    mAccepted = true;
}

// Set player as rejected.
void Player::setRejected()
{
    if(mAccepted)
    {
        throw std::runtime_error("Attempt to set player as both accepted and rejected");
    }
    mRejected = true;
}


std::string  Player::to_json(const Player& p)
{
    nlohmann::json j;

    j["URI"] =  p.getURI();
    j["Ordinal"] = p.getOrdinal();
    j["Accepted"] =  p.accepted();
    j["Rejected"] = p.rejected();
    return j.dump();
}

Player Player::from_json(const std::string& playerJson)
{
    Player p;
    nlohmann::json j;
    j = nlohmann::json::parse(playerJson);
    p.setURI(j.at("URI").get<std::string>());
    p.setOrdinal(j.at("Ordinal").get<Ordinal>());
    if ( j.at("Accepted").get<bool>())
        p.setAccepted();

    if ( j.at("Rejected").get<bool>())
        p.setRejected();
    
    return p;
}
