
/*
 * A player within a threshold signature group.
 */
#ifndef __TS_PLAYER_H
#define __TS_PLAYER_H

#include <single_include/nlohmann/json.hpp>
#include <memory>

/// Ordinal type
using Ordinal = unsigned int;

/// A player within a threshold signature group.
class Player final 
{
  public:

    /// Default constructor.
    Player() = default;

    /**
    * Constructor.
    * @param uri Unique identifier for this player, eg; their email address.
    * @param ord Ordinal for this player.
    */
    Player(const std::string& uri, Ordinal ord);

    /**
    * Get player URI.
    * @return This players URI.
    */
    const std::string& getURI() const { return mUri; }

    void setURI(const std::string& uri) {mUri = uri;};

    /**
    * Get player ordinal.
    * @return This players ordinal within the group.
    */
    Ordinal getOrdinal() const { return mOrdinal; }

    void setOrdinal(const Ordinal& mordinal) {  mOrdinal = mordinal;}

    /**
    * Get whether player has accepted group membership.
    * @return True if player has accepted group membership.
    */
    bool accepted() const { return mAccepted; }

    /**
    * Set player as accepted.
    */
    void setAccepted();

    /**
    * Get whether player has rejected group membership.
    * @return True if player has rejected group membership.
    */
    bool rejected() const { return mRejected; }

    /**
    * Set player as rejected.
    */
    void setRejected();

    /// Comparison operator.
    bool operator==(const Player& that) const
    {
        return { mUri == that.mUri &&
                 mOrdinal == that.mOrdinal &&
                 mAccepted == that.mAccepted &&
                 mRejected == that.mRejected };
    }


  private:

    /// Our URI.
    std::string mUri {};

    /// Our ordinal.
    Ordinal mOrdinal {};

    /// Player has accepted group membership proposal.
    bool mAccepted {false};
    /// Player has rejected group membership proposal.
    bool mRejected {false};

};

/// Pointer type
using PlayerSPtr = std::shared_ptr<Player>;

void to_json(nlohmann::json& j, const Player& p);
void from_json(nlohmann::json& j, Player& p);
#endif // __TS_PLAYER_H
