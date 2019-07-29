// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * A group response message.
 */

#ifndef __TS_GROUPRESPONSEMESSAGE_h 
#define __TS_GROUPRESPONSEMESSAGE_h

#include "interface/Message.h"
#include "UUID.h"
#include "GroupState.h"


/// A group response message.
class  ThresholdSignatures_API  GroupResponseMessage final : public Message
{
    using Base = Message;

  public:

    /// Default constructor.
    GroupResponseMessage();

    /**
    * Construct response message from a given member.
    * @param uri The URI of the responding group member.
    * @param groupID ID of the proposed group.
    * @param response Acceptance or rejection for the responder.
    */
    GroupResponseMessage(const std::string& uri,
                         const UUID&  groupID,
                         GroupState   response);

    /**
    * Get URI of responder.
    * @return URI of responder.
    */
    const std::string& getResponderURI() const { return mResponderUri; }

    /**
    * Get ID of proposed group.
    * @return ID of proposed group.
    */
    const UUID& getGroupID() const { return mGroupID; }

    /**
    * Get response.
    * @return Acceptance or rejection for the responder.
    */
    GroupState getResponse() const { return mResponse; }


    /* JSONSerialisable interface */

    /// Get a unique identifer for this JSONSerialisable type.
    const std::string jsonObjectType() const ;


    std::string  to_json();
    void from_json(const std::string& groupJson);

  private:

    /// URI of responder
    std::string mResponderUri {};

    /// Proposed group ID.
    UUID mGroupID {};

    /// Acceptance or rejection of proposed group membership.
    GroupState mResponse { GroupState::UNKNOWN };

};

/// Pointer type
using GroupResponseMessageSPtr = std::shared_ptr<GroupResponseMessage>;

#endif //__TS_GROUPRESPONSEMESSAGE_h
