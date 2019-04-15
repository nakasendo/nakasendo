#ifndef __MESSAGE_HASH_API__
#define __MESSAGE_HASH_API__

#include <string>

#include <DYNAMIC_LIBRARY_API.hpp>
#ifdef EXPORT_MessageHash
#    define MessageHash_API EXPORT_DYNAMIC_LIBRARY
#else
#    define MessageHash_API IMPORT_DYNAMIC_LIBRARY
#endif


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define MESSAGE_HASH_RETURN_TYPE EMSCRIPTEN_KEEPALIVE const char *
#else
#define MESSAGE_HASH_RETURN_TYPE std::string
#endif


MessageHash_API MESSAGE_HASH_RETURN_TYPE HashMsgSHA256 (const std::string&);

MessageHash_API MESSAGE_HASH_RETURN_TYPE HashMsg (const std::string&, const std::string&);

MessageHash_API MESSAGE_HASH_RETURN_TYPE ListHashFunc();

#endif //ifndef __MESSAGE_HASH_API__
