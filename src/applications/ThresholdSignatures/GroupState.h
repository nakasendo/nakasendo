// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * The state of a threshold signatures group.
 */
#ifndef _TS_GROUP_STATE_H
#define _TS_GROUP_STATE_H

#include "ThresholdSignatures.h"
#include "interface/EnumCast.h" // ?? What is that?

/// The states a Group can be in.
enum class GroupState
{
    UNKNOWN,
    PROPOSED,
    ACCEPTED,
    REJECTED
};

/// Enable enum_cast for GroupBuilder::State
inline const enumTableT<GroupState>& enumTable(GroupState)
{
    static enumTableT<GroupState> table
    {   
        {GroupState::UNKNOWN,  "Unknown"},
        {GroupState::PROPOSED, "Proposed"},
        {GroupState::ACCEPTED, "Accepted"},
        {GroupState::REJECTED, "Rejected"}
    };
    return table;
}
#endif //_TS_GROUP_STATE
