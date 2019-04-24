#include <iostream>
#include <MessageHashAPI.h>
#include <MessageHash.h>

#include <memory>

MESSAGE_HASH_RETURN_TYPE HashMsgSHA256 (const std::string& msg)
    {
         MessageHash hashMess;
         hashMess.HashSha256 (msg);
#ifdef __EMSCRIPTEN__
        return hashMess.HashHex().c_str(); 
#else
         return hashMess.HashHex ();
#endif         
    }

MESSAGE_HASH_RETURN_TYPE HashMsg (const std::string& msg, const std::string& hashContext)
    {
         MessageHash hashMess;
         hashMess.Hash(msg,hashContext);
#ifdef __EMSCRIPTEN__
        return hashMess.HashHex().c_str() ; 
#else         
        return hashMess.HashHex ();
#endif        
    }

MESSAGE_HASH_RETURN_TYPE ListHashFunc ()
    {
        MessageHash hashMess;
#ifdef __EMSCRIPTEN__
        return hashMess.ListAvailableHash().c_str() ; 
#else        
        return hashMess.ListAvailableHash () ; 
#endif        
    }