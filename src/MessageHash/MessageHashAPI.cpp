#include <iostream>
#include "MessageHashAPI.h"
#include "MessageHash.h"

#include <memory>

extern "C"{
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE const char *
#else        
    std::string 
#endif    
    HashMsgSHA256 (const std::string& msg)
    {
         MessageHash hashMess;
         hashMess.HashSha256 (msg);
#ifdef __EMSCRIPTEN__
        return hashMess.HashHex().c_str(); 
#else
         return hashMess.HashHex ();
#endif         
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE const char *
#else    
    std::string 
#endif
    HashMsg (const std::string& msg, const std::string& hashContext)
    {
         MessageHash hashMess;
         hashMess.Hash(msg,hashContext);
#ifdef __EMSCRIPTEN__
        return hashMess.HashHex().c_str() ; 
#else         
        return hashMess.HashHex ();
#endif        
    }

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE const char *
#else    
    std::string 
#endif
    ListHashFunc ()
    {
        MessageHash hashMess;
#ifdef __EMSCRIPTEN__
        return hashMess.ListAvailableHash().c_str() ; 
#else        
        return hashMess.ListAvailableHash () ; 
#endif        
    }
};