#ifndef __TS_MESSAGE_FACTORY_H__
#define __TS_MESSAGE_FACTORY_H__


#include <iostream>
#include <player.h>
#include <GroupMetaData.h>
#include <ts_messages.pb.h>
#include <exportTSCore.h>

class BigNumber;
class ECPoint;

enum class TSCore_API TSMessageDefs
{
    TS_UNKNOWN_TYPE,
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
    TS_INITIATE_PRIVATE_EVAL_REQUEST,
    TS_INITIATE_PRIVATE_EVAL_RESPONSE,
    TS_CREATE_PRIVATE_EVAL_REQUEST,
    TS_CREATE_PRIVATE_EVAL_RESPONSE,
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
    TS_CREATE_PRIKEY_REQUEST,
    TS_CREATE_PRIKEY_RESPONSE,
    TS_CREATE_PRIKEYSHARE_EXCHANGE_REQUEST,
    TS_CREATE_PRIKEYSHARE_EXCHANGE_RESPONSE,
    TS_CREATE_PLAYER_PRIKEY_REQUEST,
    TS_CREATE_PLAYER_PRIKEY_RESPONSE
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

TSCore_API void createRegisterRequest (const player&, std::ostream&);
TSCore_API void createRegisterReply (const player&, std::ostream& );
TSCore_API void createListPlayersRequest(const player&, std::ostream&);
TSCore_API void createListPlayersResponse(const player&, std::ostream&);
TSCore_API void createGroupRequest(const player&, const int&, const int&, std::ostream&);
TSCore_API void createGroupReply(const player&, const std::string&, const bool&, std::ostream&);

TSCore_API void createInviteRequest(const std::string&, std::ostream&);
TSCore_API void createInviteReply(const std::string&, const std::string&, const bool&, std::ostream&);

TSCore_API void createGroupIsSetRequest(const std::string&, const GroupMetadata&, std::ostream&);
TSCore_API void createGroupIsSetReply(const std::string&, const std::string&, const bool&, std::ostream&);

TSCore_API void createDeleteTSGroupRequest(const player&, const std::string&, std::ostream&);
TSCore_API void createDeleteTSGroupResponse(const player&, const std::string&, const bool&, std::ostream&);
TSCore_API void createDeleteTSPlayerGroupRequest(const std::string&, const std::string&, std::ostream&);
TSCore_API void createDeleteTSPlayerGroupResponse(const std::string&, const std::string&, const bool&, std::ostream&);


TSCore_API void createSecretSharingRequest(const player&, const std::string&, const std::string&, std::ostream&);
TSCore_API void createSecretSharingResponse(const player&, const std::string&, const std::string&, const bool&, std::ostream&);

TSCore_API void createSecretSharingInitEvalRequest(const std::string&, const std::string&, const std::string&, std::ostream& );
TSCore_API void createSecretSharingInitEvalResponse(const std::string&, const std::string&, const std::string&, const bool&, std::ostream&);
TSCore_API void createPrivateDataEvalRequest(const std::string& , const std::string&, const int&, const int&, const jvrss&, std::ostream& );
TSCore_API void createPrivateDataEvalResponse(const std::string&, const std::string&, const bool&, std::ostream&);

TSCore_API void createSecretSharingPlayerDataRequest(const std::string&, const std::string&, const std::string&, std::ostream&);
TSCore_API void createSecretSharingPlayerDataResponse(const std::string&, const std::string&, const std::string& , const int&, const jvrss&, std::ostream&);

TSCore_API void createSecretSharingCollatedDataRequest(const std::string&, const std::string&, const std::string&, const GroupMetadata&, std::ostream&);

TSCore_API void createSecretSharingCollatedPlayerResponse(const std::string&, const std::string&, const std::string&, std::ostream&);

TSCore_API void createEphemeralKeyDataRequest(const std::string&, const std::string&, std::ostream&);
TSCore_API void createEphemeralKeyDataResponse(const std::string&, const std::string&, std::ostream&);

TSCore_API void createEphermalKeyPlayerDataRequest(const std::string&, const std::string&, std::ostream&);
TSCore_API void createEphemeralKeyPlayerDataResponse(const std::string&, const std::string&, const int&, const std::pair<BigNumber, ECPoint>&, std::ostream&) ;

TSCore_API void createEphemeralKeyCollatedDataRequest(const std::string&, const std::string&, const GroupMetadata&, std::ostream&);
TSCore_API void createAEphemeralKeyCollatedDataResponse(const std::string&, const std::string&, const std::string&, std::ostream&);

TSCore_API void createSignatureRequest(const std::string& , const std::string&, const std::string&, const int&, std::ostream& );
TSCore_API void createSignatureResponse(const std::string&, const std::string&, const GroupMetadata&, const std::string& , const int&, std::ostream&);
TSCore_API void createSignaturePlayerDataRequest(const std::string&, const std::string&, const std::string&, const int&, std::ostream&);
TSCore_API void createSignaturePlayerDataResponse(const std::string&, const std::string&, const std::pair<std::string, BigNumber>&, const int&, std::ostream&);

TSCore_API void createPrivateKeyRequest(const std::string&, const std::string&, const std::vector<std::string>&, std::ostream& );
TSCore_API void createPrivateKeyResponse(const std::string&, const std::string&, const bool&, std::ostream&);

TSCore_API void createPlayerPrivateKeyShareRequest(const std::string&, const std::string&, std::ostream&);
TSCore_API void createPlayerPrivateKeyShareResponse(const std::string&, const std::string&, const std::string&, const std::string&, const bool&, std::ostream&);

TSCore_API void createPlayerPrivateShareExchangeRequest(const std::string&, const std::string&, std::ostream&);
TSCore_API void createPlayerPrivateShareExchangeResponse(const std::string&, const std::string&, const bool&, std::ostream&);

TSCore_API void createPlayerPrivateKeyShareRequest(const playerGroupMetaData&,std::ostream&);
TSCore_API void createPlayerPrivateKeyShareResponse(const std::string&, const std::string&, const bool&, std::ostream&);

#endif //#ifndef __TS_MESSAGE_FACTORY_H__

