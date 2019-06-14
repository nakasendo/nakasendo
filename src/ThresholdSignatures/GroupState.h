// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in project root for licensing information.

/*
 * The state of a threshold signatures group.
 */

#pragma once

#include <impl/utils/EnumCast.h>

namespace nakasendo { namespace impl
{

/// The states a Group can be in.
enum class GroupState
{   
    UNKNOWN,
    PROPOSED,
    ACCEPTED,
    REJECTED
};

/// Enable enum_cast for GroupBuilder::State
inline const utils::enumTableT<GroupState>& enumTable(GroupState)
{
    static utils::enumTableT<GroupState> table
    {   
        {GroupState::UNKNOWN,  "Unknown"},
        {GroupState::PROPOSED, "Proposed"},
        {GroupState::ACCEPTED, "Accepted"},
        {GroupState::REJECTED, "Rejected"}
    };
    return table;
}

}}

