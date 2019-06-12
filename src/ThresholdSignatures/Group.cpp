// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * A group of players for threshold signatures.
 */

#include "Group.h"

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
void  to_json(nlohmann::json& j, const Group& g)
{
    j["ID"] = g.getID().getString();
    std::vector<nlohmann::json> jsonPlayerList;

    for(const auto& player : g.getPlayers())
    {
        nlohmann::json jsonPlayer;
	to_json(jsonPlayer, player.second);
        jsonPlayerList.push_back(jsonPlayer);
    }
    j["Players"] = jsonPlayerList;
}

void from_json(nlohmann::json& j, Group& g)
{
    g.setID(j.at("ID").get<std::string>());
    
    // Parse players
    g.clearPlayers();
    for(auto& jsonPlayer : j["Players"])
    {
        Player player {};
	from_json(jsonPlayer, player);
        g.emplace_player(std::make_pair(player.getURI(), player));
    }
}

// Comparison operator for Groups. Mainly of use in testing.
bool Group::operator==(const Group& that) const
{
    return mGroupID == that.mGroupID &&
           getPlayersSortedByOrdinal() == that.getPlayersSortedByOrdinal();
}
