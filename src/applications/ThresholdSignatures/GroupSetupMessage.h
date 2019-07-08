// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * A group setup message.
 */
#ifndef __TS_GROUPSETUPMESSAGE_H
#define __TS_GROUPSETUPMESSAGE_H

#include "interface/Message.h"
#include "UUID.h"
#include "Group.h"

/// A group setup message.
class  ThresholdSignatures_API  GroupSetupMessage final : public Message
{
    using Base = Message;

  public:

    /// Default constructor.
    GroupSetupMessage();

    /**
    * Construct setup message from a given proposer.
    * @param uri The URI of the group setup proposer.
    * @param groupID ID of the proposed group.
    * @param players A set of proposed group members.
    */
    GroupSetupMessage(const std::string&            uri,
                      const UUID&             groupID,
                      const Group::PlayerMap& players);

    /**
    * Get URI of proposer.
    * @return URI of proposer.
    */
    const std::string& getProposerURI() const { return mProposerUri; }

    /**
    * Get ID of proposed group.
    * @return ID of proposed group.
    */
    const UUID& getGroupID() const { return mGroupID; }

    /**
    * Get the group members.
    * @return A map of proposed group members.
    */
    const Group::PlayerMap& getPlayers() const { return mPlayers; }


    /* JSONSerialisable interface */

    /// Get a unique identifer for this JSONSerialisable type.
    const std::string jsonObjectType() const ;


    // JSON serialize
    virtual std::string to_json();

    // JSON deserialize
    virtual void from_json(const std::string& groupJson);
  private:

    /// URI of proposer
    std::string mProposerUri {};

    /// Proposed group ID.
    UUID mGroupID {};

    /// List of proposed group members.
    Group::PlayerMap mPlayers {};

};

/// Pointer type
using GroupSetupMessageSPtr = std::shared_ptr<GroupSetupMessage>;

#endif //__TS_GROUPSETUPMESSAGE_H
