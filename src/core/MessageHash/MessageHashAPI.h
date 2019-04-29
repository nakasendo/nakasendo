#ifndef __MESSAGE_HASH_API__
#define __MESSAGE_HASH_API__

#include <MessageHash/MessageHash.h>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define MESSAGE_HASH_RETURN_TYPE EMSCRIPTEN_KEEPALIVE const char *
#else
#define MESSAGE_HASH_RETURN_TYPE std::string
#endif


MessageHash_API MESSAGE_HASH_RETURN_TYPE HashMsgSHA256 (const std::string&);

MessageHash_API MESSAGE_HASH_RETURN_TYPE HashMsg (const std::string&, const std::string&);

MessageHash_API MESSAGE_HASH_RETURN_TYPE ListHashFunc();

MessageHash_API MESSAGE_HASH_RETURN_TYPE EncodeBase64(const std::string&);

MessageHash_API MESSAGE_HASH_RETURN_TYPE DecodeBase64(const std::string&);

#endif //ifndef __MESSAGE_HASH_API__
