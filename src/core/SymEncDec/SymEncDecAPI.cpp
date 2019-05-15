#include <SymEncDec/SymEncDecAPI.h>
#include <MessageHash/MessageHashAPI.h>

#include <string.h>
#include <memory>
#include <iostream>

SYMENCDEC_RETURN_TYPE Encode (const std::string& msg, const std::string& key,const std::string& iv){

    std::cout << "Encoding" << std::endl; 
    std::unique_ptr<unsigned char> myKey (new unsigned char [key.length() + 1 ]);
    std::fill_n(myKey.get(), key.length()+1, 0x00);
    int index(0);

    for(std::string::const_iterator iter = key.begin(); iter != key.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }

    std::unique_ptr<unsigned char> mySalt (new unsigned char [iv.length() + 1 ]);
    std::fill_n(mySalt.get(), iv.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
        mySalt.get()[index]=*iter;
    }

    uint64_t keylen(32);
    int iterCount(10000);
    std::unique_ptr<unsigned char> encodingKey = KeyGen(myKey,key.size(),mySalt, iv.length(),iterCount, keylen);    


    SymEncDec encdec;
   
    encdec.SetParams(encodingKey, mySalt, keylen,16);
    std::unique_ptr<unsigned char> encMsg;
    int encMsgLen = encdec.aes_encrypt(msg, encMsg);
    
    std::string retval; 
    for (int i=0;i<encMsgLen; ++ i){
        retval.push_back((char)encMsg.get()[i]);
    } 
#ifdef __EMSCRIPTEN__
    return retval.c_str(); 
#else
    return retval;
#endif
}

std::vector<uint8_t> EncodeEx (const std::string& msg, const std::string& key,const std::string& iv){
    std::cout << "EncodingEx" << std::endl; 
    std::unique_ptr<unsigned char> myKey (new unsigned char [key.length() + 1 ]);
    std::fill_n(myKey.get(), key.length()+1, 0x00);
    int index(0);

    for(std::string::const_iterator iter = key.begin(); iter != key.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }

    std::unique_ptr<unsigned char> mySalt (new unsigned char [iv.length() + 1 ]);
    std::fill_n(mySalt.get(), iv.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
        mySalt.get()[index]=*iter;
    }

    uint64_t keylen(32);
    int iterCount(10000);
    std::unique_ptr<unsigned char> encodingKey = KeyGen(myKey,key.size(),mySalt, iv.length(),iterCount, keylen);    


    SymEncDec encdec;
   
    encdec.SetParams(encodingKey, mySalt, keylen,16);
    std::unique_ptr<unsigned char> encMsg;
    int encMsgLen = encdec.aes_encrypt(msg, encMsg);
    std::vector<uint8_t> retVal; 
    for(int i=0;i<encMsgLen;++i){
        retVal.push_back(encMsg.get()[i]);
    }
    return retVal ;
}

SYMENCDEC_RETURN_TYPE Decode (const std::string& msg, const std::string& key, const std::string& iv ){ 
    std::unique_ptr<unsigned char> myKey (new unsigned char [key.length() + 1 ]);
    std::fill_n(myKey.get(), key.length()+1, 0x00);
    int index(0);

    for(std::string::const_iterator iter = key.begin(); iter != key.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }

    std::unique_ptr<unsigned char> mySalt (new unsigned char [iv.length() + 1 ]);
    std::fill_n(mySalt.get(), iv.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
        mySalt.get()[index]=*iter;
    }

    uint64_t keylen(32);
    int iterCount(10000);
    std::unique_ptr<unsigned char> encodingKey = KeyGen(myKey,key.size(),mySalt, iv.length(),iterCount, keylen); 

    SymEncDec encdec;
    encdec.SetParams(encodingKey, mySalt, keylen, 16);
    std::unique_ptr<unsigned char> decMsg;
    int decMsgLen = encdec.aes_decrypt(msg, decMsg);
    
    std::string retval; 
    for (int i=0;i<decMsgLen; ++ i){
        retval.push_back(decMsg.get()[i]);
    }    
#ifdef __EMSCRIPTEN__
    return retval.c_str(); 
#else
    return retval;
#endif
}

SYMENCDEC_RETURN_TYPE DecodeEx (const std::vector<uint8_t>& msg, const std::string& key, const std::string& iv ){ 
    std::unique_ptr<unsigned char> myKey (new unsigned char [key.length()+1]);
    std::fill_n(myKey.get(), key.length()+1, 0x00);
    int index(0);

    std::cout << "Key: " << key  << "Key Len: " << key.length() << "\tiv: " << iv << "\tiv len: " << iv.length() << std::endl; 
    for(std::string::const_iterator iter = key.begin(); iter != key.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }

    std::unique_ptr<unsigned char> mySalt (new unsigned char [iv.length()+1]);
    std::fill_n(mySalt.get(), iv.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
        mySalt.get()[index]=*iter;
    }

    uint64_t keylen(32);
    int iterCount(10000);
    std::unique_ptr<unsigned char> encodingKey = KeyGen(myKey,key.size(),mySalt, iv.length(),iterCount, keylen); 

    SymEncDec encdec;
    encdec.SetParams(encodingKey, mySalt, keylen, 16);
    std::unique_ptr<unsigned char> decMsg;
    std::unique_ptr<unsigned char> msgPtr(new unsigned char[msg.size()]); 
    std::fill_n(msgPtr.get(), msg.size(), 0x00);
    index=0;
    for(std::vector<uint8_t>::const_iterator iter = msg.begin();iter != msg.end(); ++iter){
        msgPtr.get()[index++] = *iter;      
        std::cout << *iter ;    
    }
    std::cout << std::endl;
    int decMsgLen = encdec.aes_decryptEx(msgPtr,(msg.size()),decMsg);
    
    std::string retval; 
    for (int i=0;i<decMsgLen; ++ i){
        retval.push_back(decMsg.get()[i]);
    }    
#ifdef __EMSCRIPTEN__
    return retval.c_str(); 
#else
    return retval;
#endif
}

SYMENCDEC_RETURN_TYPE GenerateKey256(const std::string& key, const std::string& iv){

    std::unique_ptr<unsigned char> myKey (new unsigned char [key.length() + 1 ]);
    std::fill_n(myKey.get(), key.length()+1, 0x00);
    int index(0);

    for(std::string::const_iterator iter = key.begin(); iter != key.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }

    std::unique_ptr<unsigned char> mySalt (new unsigned char [iv.length() + 1 ]);
    std::fill_n(mySalt.get(), iv.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
        mySalt.get()[index]=*iter;
    }

    uint64_t keylen(32);
    int iterCount(10000);
    std::unique_ptr<unsigned char> encodingKey = KeyGen(myKey,key.size(),mySalt, iv.length(),iterCount, keylen);

    std::string retval; 
    if (encodingKey != nullptr){
        for(int i=0; i<32;++i){
            retval.push_back(encodingKey.get()[i]);
        }
    }
#ifdef __EMSCRIPTEN__
    return retval.c_str(); 
#else
    return retval;
#endif
}
