#ifndef __TS_MESSAGE_FACTORY_H__
#define __TS_MESSAGE_FACTORY_H__


#include <iostream>
#include <player.h>
#include <GroupMetaData.h>
#include <ts.pb.h>

class BigNumber;
class ECPoint;

enum class TSMessageDefs
{
    TS_UNKNOWN_TYPE,
    TS_TIME_REQUEST,
    TS_TIME_RESPONSE,
    TS_REGISTER_PLAYER_REQUEST,
    TS_REGISTER_PLAYER_RESPONSE,
    TS_LIST_PLAYER_REQUEST,
    TS_LIST_PLAYER_RESPONSE,
    TS_CREATE_GROUP_REQUEST,
    TS_CREATE_GROUP_RESPONSE,
    TS_INVITE_TO_GROUP_REQUEST,
    TS_INVITE_TO_GROUP_RESPONSE,
    TS_BROADCAST_GROUP_DETAILS_REQUEST,
    TS_BROADCAST_GROUP_DETAILS_RESPONSE,
    TS_DELETE_GROUP_REQUEST,
    TS_DELETE_GROUP_RESPONSE,
    TS_DELETE_PLAYER_GROUP_REQUEST,
    TS_DELETE_PLAYER_GROUP_RESPONSE,
    TS_CREATE_GROUP_SECRET_REQUEST,
    TS_CREATE_GROUP_SECRET_RESPONSE,
    TS_CREATE_PLAYER_SECRET_REQUEST,
    TS_CREATE_PLAYER_SECRET_RESPONSE,
    TS_CREATE_COLLATED_SECRET_PLAYER_REQUEST,
    TS_CREATE_COLLATED_SECRET_PLAYER_RESPONSE,
    TS_CREATE_PRE_SIG_REQUEST,
    TS_CREATE_PRE_SIG_RESPONSE,
    TS_CREATE_PRE_SIG_PLAYER_REQUEST,
    TS_CREATE_PRE_SIG_PLAYER_RESPONSE,
    TS_CREATE_COLLATED_PRE_SIG_REQUEST,
    TS_CREATE_COLLATED_PRE_SIG_RESPONSE,
    TS_CREATE_GROUP_SIG_REQUEST,
    TS_CREATE_GROUP_SIG_RESPONSE,
    TS_CREATE_SIG_PLAYER_REQUEST,
    TS_CREATE_SIG_PLAYER_RESPONSE,
};

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

template<typename T>
std::istream& operator>>(typename std::enable_if<std::is_enum<T>::value, std::istream>::type& stream, const T& e)
{
    return stream >> static_cast<typename std::underlying_type<T>::type>(e);
}

void createTimeRequest(const player&, std::ostream&);
void createTimeResponse(const player&, std::ostream&);
void createRegisterPlayerRequest (const player&, std::ostream&);
void createRegisterPlayerResponse(const player&, std::ostream& );
void createListPlayersRequest(const player&, std::ostream&);
void createListPlayersResponse(const player&, std::ostream&);
void createThresholdGroupRequest(const player&, const int&, const int&, std::ostream&);
void createThresholdGroupResponse(const player&, const std::string&, const bool&, std::ostream&);

void createInviteToGroupMessage(const std::string&, const std::string&, std::ostream&); 
void createInviteToGroupResponse(const std::string&, const std::string&, const bool&, std::ostream&);

void createBroadCastGroupDetails(const GroupMetadata&, std::ostream&);
void createBroadCastGroupDetailsResponse(const std::string&, const std::string&, const bool&, std::ostream&);

void createDeleteTSGroupRequest(const player&, const std::string&, std::ostream&);
void createDeleteTSGroupResponse(const player&, const std::string&, const bool&, std::ostream&);
void createDeleteTSPlayerGroupRequest(const std::string&, const std::string&, std::ostream&);
void createDeleteTSPlayerGroupResponse(const std::string&, const std::string&, const bool&, std::ostream&);


void createSecretSharingRequest(const player&, const std::string&, const std::string&, std::ostream&);
void createSecretSharingResponse(const player&, const std::string&, const std::string&, const bool&, std::ostream&);

void createSecretSharingPlayerDataRequest(const std::string&, const std::string&, const std::string&, std::ostream&);
void createSecretSharingPlayerDataResponse(const std::string&, const std::string&, const std::string& , const int&, const jvrss&, std::ostream&);

void createSecretSharingCollatedDataRequest(const std::string&, const std::string&, const std::string&, const GroupMetadata&, std::ostream&);

void createSecretSharingCollatedPlayerResponse(const std::string&, const std::string&, const std::string&, std::ostream&);

void createEphemeralKeyDataRequest(const std::string&, const std::string&, std::ostream&);
void createEphemeralKeyDataResponse(const std::string&, const std::string&, std::ostream&);

void createEphermalKeyPlayerDataRequest(const std::string&, const std::string&, std::ostream&);
void createEphemeralKeyPlayerDataResponse(const std::string&, const std::string&, const int&, const std::pair<BigNumber, ECPoint>&, std::ostream&) ;

void createEphemeralKeyCollatedDataRequest(const std::string&, const std::string&, const GroupMetadata&, std::ostream&);
void createAEphemeralKeyCollatedDataResponse(const std::string&, const std::string&, const std::string&, std::ostream&);

void createSignatureRequest(const std::string& , const std::string&, const std::string&, const int&, std::ostream& );  
void createSignatureResponse(const std::string&, const std::string&, const GroupMetadata&, const std::string& , const int&, std::ostream&);
void createSignaturePlayerDataRequest(const std::string&, const std::string&, const std::string&, const int&, std::ostream&);
void createSignaturePlayerDataResponse(const std::string&, const std::string&, const std::pair<std::string, BigNumber>&, const int&, std::ostream&);

#endif //#ifndef __TS_MESSAGE_FACTORY_H__

