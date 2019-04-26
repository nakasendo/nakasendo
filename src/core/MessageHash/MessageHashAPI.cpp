#include <iostream>
<<<<<<< Updated upstream:src/core/MessageHash/MessageHashAPI.cpp
#include <MessageHash/MessageHashAPI.h>
#include <MessageHash/MessageHash.h>
=======
#include "MessageHashAPI.h"
#include "MessageHash.h"
#include "Base64EncDec.h"
>>>>>>> Stashed changes:src/MessageHash/MessageHashAPI.cpp

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


    MESSAGE_HASH_RETURN_TYPE EncodeBase64 (const std::string& msg)
    {
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msg.size() + 1 ]);        
        int index(0);
        for (std::string::const_iterator iter = msg.begin(); iter != msg.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ; 
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msg.size () + 1, 0 );
        std::string retVal ((char*)retValPtr.get());
#ifdef __EMSCRIPTEN__
        return retVal.c_str() ; 
#else        
        return retVal ; 
#endif        
    }

    MESSAGE_HASH_RETURN_TYPE DecodeBase64 (const std::string& msg)
    {
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int;  
        
        std::string nonConstMsg ( msg ); 
        nonConstMsg = nonConstMsg.erase(nonConstMsg.find_last_not_of("\t\n\v\f\r ")+1);
        
        
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [nonConstMsg.length()+1]);  
        std::fill_n(msgPtr.get(), msg.length()+1, 0x00);      
        std::string::const_iterator iter = nonConstMsg.begin();
        for (unsigned int i = 0; i < nonConstMsg.size();++i){
            msgPtr.get()[i] = *iter ; ++ iter ; 
        }        
        Base64EncDec encdec;
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(msgPtr,value, strict, err);
        std::string retVal; 
        if (decodedValPtr != nullptr){
            retVal = (char*)decodedValPtr.get() ; 
        }else{retVal="ERROR";}
#ifdef __EMSCRIPTEN__
        return retVal.c_str() ; 
#else        
        return retVal ; 
#endif        
    }
};
