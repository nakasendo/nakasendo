
/*
 * A group of players for threshold signatures.
 */

#ifndef __TS_GROUP_H
#define __TS_GROUP_H
#include <single_include/nlohmann/json.hpp>
#include "Player.h"

#include <UUID.h>

#include <unordered_map>
#include <vector>
#include <memory>


/// A group of players for threshold signatures.
class Group final 
{
  public:

    /// A map of plauyer URIs to players.
    using PlayerMap = std::unordered_map<std::string, Player>;


    /// Default constructor.
    Group() = default;

    /**
    * Construct from a list of player URIs.
    * @param playerUris A list of URIs for players in the group.
    */
    Group(const UUID& uuid, const PlayerMap& players);

    /**
    * Get group ID.
    * @return Our group UUID.
    */
    const UUID& getID() const { return mGroupID; }

    void setID(const UUID& groupID) { mGroupID = groupID; }

    /**
    * Get the group members.
    * @return A reference to the map of players.
    */
    const PlayerMap& getPlayers() const { return mPlayers; }

    void clearPlayers() { mPlayers.clear();}

    /**
    * Get the number of players in the group.
    * @return The size of the group.
    */
    size_t size() const { return mPlayers.size(); }

    /**
    * Get a vector of players ordered by it's ordinal number.
    * @return A vector of players.
    */
    std::vector<Player> getPlayersSortedByOrdinal() const;

    /**
    * Get player details from their ordinal.
    * @param ordinal A player ordinal within this group.
    * @return The player details.
    */
    Player getPlayerFromOrdinal(Ordinal ordinal) const;

    /// Comparison operator for Groups. Mainly of use in testing.
    bool operator==(const Group& that) const;

    /* JSONSerialisable interface */

    void emplace_player(std::pair<std::string, Player> && player);
    //void push_player(std::pair<std::string, Player>  player);

  private:

    /// Our group ID.
    UUID mGroupID {};

    /// Our group members.
    PlayerMap mPlayers {};

};

/// Pointer type.
using GroupSPtr = std::shared_ptr<Group>;

void to_json(nlohmann::json& j, const Group& g);
void from_json(nlohmann::json& j, Group& g);
#endif // __TS_GROUP_H
