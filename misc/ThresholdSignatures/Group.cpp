// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * A group of players for threshold signatures.
 */

#include <ThresholdSignatures/Group.h>
#include <single_include/nlohmann/json.hpp>

// Construct from a list of player URIs.
Group::Group(const UUID& uuid, const PlayerMap& players)
: mGroupID{uuid}, mPlayers{players}
{
}

// Get a vector of players ordered by it's ordinal number
std::vector<Player> Group::getPlayersSortedByOrdinal() const {
    std::vector<Player> players;
    for (const auto& elem : mPlayers) {
        players.push_back(elem.second);
    }
    std::sort(players.begin(), players.end(),
                [](const Player& lhs, const Player& rhs) {
                    return lhs.getOrdinal() < rhs.getOrdinal();});
    return players;
}

// Get player details from their ordinal.
Player Group::getPlayerFromOrdinal(Ordinal ordinal) const
{
    // Bounds check
    if(ordinal == 0 || ordinal > size())
    {
        throw std::invalid_argument("Ordinal out of range");
    }

    const std::vector<Player>& allPlayers { getPlayersSortedByOrdinal() };
    return allPlayers[ordinal - 1];
}

void Group::emplace_player(std::pair<std::string, Player> && player)
{
    mPlayers.emplace(std::move(player));
}

std::string  Group::to_json(const Group& g)
{
    nlohmann::json j;
    j["ID"] = g.getID().getString();
    std::vector<std::string> playerList;

    for(const auto& player : g.getPlayers())
    {
        std::string playerString = Player::to_json(player.second);
        playerList.push_back(playerString);
    }
    j["Players"] = playerList;
    return j.dump();
}

Group Group::from_json(const std::string& groupJson)
{
    Group g;
    nlohmann::json j;
    j = nlohmann::json::parse(groupJson);

    g.setID(j.at("ID").get<std::string>());

    for(auto& jsonPlayer : j["Players"])
    {
        Player player = Player::from_json(jsonPlayer); 
	g.emplace_player(std::make_pair(player.getURI(), player));
    }

    return g;
}

// Comparison operator for Groups. Mainly of use in testing.
bool Group::operator==(const Group& that) const
{
    return mGroupID == that.mGroupID &&
           getPlayersSortedByOrdinal() == that.getPlayersSortedByOrdinal();
}
