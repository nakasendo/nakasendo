// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * Builder for groups.
 */
#ifndef _TS_GROUP_BUILDER_H
#define _TS_GROUP_BUILDER_H

#include "GroupSetupMessage.h"
#include "GroupResponseMessage.h"
#include "Group.h"
#include "GroupState.h"

#include <memory>
#include <vector>
#include <mutex>

/// A builder for groups.
class  ThresholdSignatures_API GroupBuilder final 
{
  public:

    /// Default constructor.
    GroupBuilder();

    /**
    * Construct for the given list of players. Also sets our acceptance state (as
    * group proposer) to ACCEPTED.
    * @param playerUris A list of URIs for players in the group we are building.
    * @param myUri Our URI as the group proposer. Must be one of the playerUris.
    */
    GroupBuilder(const std::vector<std::string>& playerUris, const std::string& myUri);

    /**
    * Construct from a group setup message.
    * @param setupMsg A group setup message.
    * @param myUri The constructing players URI.
    */
    GroupBuilder(const GroupSetupMessageSPtr& setupMsg, const std::string& myUri);

    /**
    * Get the Group we are building. Throws unless the current state is ACCEPTED
    * (all players in the group have accepted the invitation).
    * @return A newly created Group.
    */
    GroupSPtr getGroup() const;

    /**
    * Get group ID.
    * @return Our group UUID.
    */
    const UUID& getID() const { return mGroupID; }

    /**
    * Get the group members.
    * @return A copy of the map of players.
    */
    Group::PlayerMap getPlayers() const;

    /**
    * Get the proposer URI.
    * @return URI of the group proposer.
    */
    const std::string& getProposerURI() const { return mProposerUri; }

    /**
    * Get my URI.
    * @return URI of this player.
    */
    const std::string& getMyURI() const { return mMyUri; }

    /**
    * Get the group state.
    * @return The state of the group being built.
    */
    GroupState getState() const { return mState; }


    /**
    * Get a group proposal message for all other group members.
    * @return A group setup message for sending to every other member of the
    * proposed group.
    */
    GroupSetupMessageSPtr getGroupSetupMessage() const;

    /**
    * Get a group response message for all other group members.
    * @param state Whether we are accepting or rejecting the proposed
    * group membership.
    * @return A group response message for sending to every other member of
    * group.
    */
    GroupResponseMessageSPtr getGroupSetupResponseMessage(GroupState state);

    /**
    * Process group proposal message response.
    * @param response A group response message from one of the group members.
    */
    void processGroupSetupResponse(const GroupResponseMessageSPtr& response);


    /* JSONSerialisable interface */

    /// Get a unique identifer for this JSONSerialisable type.
    const std::string jsonObjectType() const ;

    std::string to_json();
    void from_json(const std::string& groupJson);

  private:

    /// A mutex
    mutable std::mutex mMtx {};

    /// Our group ID.
    UUID mGroupID {};

    /// Our group members.
    Group::PlayerMap mPlayers {};

    /// The group proposer
    std::string mProposerUri {};

    /// My (this players) URI
    std::string mMyUri {};

    /// Our state.
    GroupState mState { GroupState::UNKNOWN };

};

/// Pointer type
using GroupBuilderSPtr = std::shared_ptr<GroupBuilder>;
#endif //_TS_GROUP_BUILDER_H
