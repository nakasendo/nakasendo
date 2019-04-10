#ifndef __MESSAGE_HASH_API__
#define __MESSAGE_HASH_API__

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <string>
extern "C"{
#ifdef __EMSCRIPTEN__
const char *
#else
std::string
#endif    
HashMsgSHA256 (const std::string&);

#ifdef __EMSCRIPTEN__
const char *
#else
std::string
#endif
HashMsg (const std::string&, const std::string&);

#ifdef __EMSCRIPTEN__
const char *
#else
std::string
#endif
ListHashFunc ();


};

#endif //ifndef __MESSAGE_HASH_API__
