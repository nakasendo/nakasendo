
/*
 * A player within a threshold signature group.
 */

#include "Player.h"
#include "nlohmann/json.hpp"

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


void  to_json(nlohmann::json& j, const Player& p)
{
    j["URI"] =  p.getURI();
    j["Ordinal"] = p.getOrdinal();
    j["Accepted"] =  p.accepted();
    j["Rejected"] = p.rejected();
}

void from_json(nlohmann::json& j, Player& p)
{
    p.setURI(j.at("URI").get<std::string>());
    p.setOrdinal(j.at("Ordinal").get<Ordinal>());
    if ( j.at("Accepted").get<bool>())
        p.setAccepted();

    if ( j.at("Rejected").get<bool>())
        p.setRejected();
}
