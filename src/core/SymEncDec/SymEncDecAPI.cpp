#include <SymEncDec/SymEncDecAPI.h>
#include <MessageHash/MessageHashAPI.h>
#include <SymEncDec/conversions.h>
#include <string.h>
#include <sstream>
#include <memory>
#include <iostream>


SYMENCDEC_RETURN_TYPE GenKeyAndEncode (const std::string& msg, const std::string& key,std::string& iv,uint64_t keylen, uint64_t blocksize){
#if 1
    std::unique_ptr<unsigned char[]> myKey (new unsigned char [key.length()]);
    std::fill_n(myKey.get(), key.length(), 0x0);
    int index(0);

    for(std::string::const_iterator iter = key.begin(); iter != key.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }
    int iterCount(10000);


    std::unique_ptr<unsigned char[]> nounce;
    try{
        //change to return the nouce size. 
        NounceGen(nounce,blocksize);
        iv = binTohexStr(nounce,blocksize); 
    }
    catch(const std::exception& e){
        std::cout << "Failed to generate a nounce value" << e.what () << std::endl;
        return std::string (); 
    }
    std::unique_ptr<unsigned char[]> encodingKey = KeyGen(myKey,key.size(),nounce, blocksize,iterCount, keylen); 
  
       
#endif



    SymEncDec encdec;

    encdec.SetParams(encodingKey, nounce, keylen,blocksize);
    std::unique_ptr<unsigned char[]> encMsg;
    int encMsgLen = encdec.aes_encrypt(msg, encMsg);
  
    std::string hexvals = binTohexStr(encMsg, encMsgLen);

#ifdef __EMSCRIPTEN__
    return hexvals.c_str(); 
#else
    return hexvals;
#endif
}



SYMENCDEC_RETURN_TYPE GenKeyAndDecode (const std::string& msg, const std::string& key, const std::string& iv,uint64_t keylen, uint64_t blocksize){ 
    std::unique_ptr<unsigned char[]> myKey (new unsigned char [key.length() + 1 ]);
    std::fill_n(myKey.get(), key.length()+1, 0x00);
    int index(0);

    for(std::string::const_iterator iter = key.begin(); iter != key.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }


    size_t nounceBufferLen(0);
    std::unique_ptr<unsigned char[]> nounce = HexStrToBin(iv,&nounceBufferLen );

    if(nounceBufferLen != blocksize){
        throw std::runtime_error("Incorrect nounce size");
    }
    int iterCount(10000);
    std::unique_ptr<unsigned char[]> encodingKey = KeyGen(myKey,key.size(),nounce, blocksize,iterCount, keylen); 

    SymEncDec encdec;
    encdec.SetParams(encodingKey, nounce, keylen, blocksize);

    
    size_t bufferLen(0);
    std::unique_ptr<unsigned char[]> recoveredBuf =  HexStrToBin(msg, &bufferLen);


    std::string retval;  
    int decMsgLen = encdec.aes_decrypt(recoveredBuf, bufferLen, retval);
    
    
#ifdef __EMSCRIPTEN__
    return retval.c_str(); 
#else
    return retval;
#endif
}



SYMENCDEC_RETURN_TYPE Encode (const std::string& msg, const std::string& key,const std::string& iv,uint64_t keylen, uint64_t blocksize){

    size_t keyBufferLen(0);
    std::unique_ptr<unsigned char[]> myKey = HexStrToBin(key, &keyBufferLen);
    size_t nounceBufferLen(0);
    std::unique_ptr<unsigned char[]> mySalt = HexStrToBin(iv, &nounceBufferLen);

    if (keyBufferLen != keylen){
        throw std::runtime_error ("Incorrect Key size");
    } 
    if(nounceBufferLen != blocksize){
        throw std::runtime_error ("Incorrect nounce size");
    }
    SymEncDec encdec;

    encdec.SetParams(myKey, mySalt, keylen,blocksize);
    std::unique_ptr<unsigned char[]> encMsg;
    int encMsgLen = encdec.aes_encrypt(msg, encMsg);
  
    std::string hexvals = binTohexStr(encMsg, encMsgLen);

#ifdef __EMSCRIPTEN__
    return hexvals.c_str(); 
#else
    return hexvals;
#endif
}


SYMENCDEC_RETURN_TYPE Decode (const std::string& msg, const std::string& key, const std::string& iv,uint64_t keylen, uint64_t blocksize){ 
    size_t keyBufferLen(0);
    std::unique_ptr<unsigned char[]> myKey = HexStrToBin(key, &keyBufferLen);
    size_t nounceBufferLen(0);
    std::unique_ptr<unsigned char[]> mySalt = HexStrToBin(iv, &nounceBufferLen);

    if (keyBufferLen != keylen){
        throw std::runtime_error ("Incorrect Key size");
    } 
    if(nounceBufferLen != blocksize){
        throw std::runtime_error ("Incorrect nounce size");
    }

    SymEncDec encdec;
    encdec.SetParams(myKey, mySalt, keylen, blocksize);
    std::unique_ptr<unsigned char[]> encMsg;
    int encMsgLen = encdec.aes_encrypt(msg, encMsg);
  
    std::string hexvals = binTohexStr(encMsg, encMsgLen);

#ifdef __EMSCRIPTEN__
    return hexvals.c_str(); 
#else
    return hexvals;
#endif
}


SYMENCDEC_RETURN_TYPE GenerateKey256(const std::string& key, const std::string& iv, uint64_t keylen , uint64_t blocksize){

    // This assumes that the IV is a hex encoded 16 byte number

    std::unique_ptr<unsigned char[]> myKey (new unsigned char [key.length() + 1 ]);
    std::fill_n(myKey.get(), key.length()+1, 0x00);
    int index(0);

    for(std::string::const_iterator iter = key.begin(); iter != key.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }

    size_t ivbufferLen;
    std::unique_ptr<unsigned char[]> mySalt = HexStrToBin(iv, &ivbufferLen);


    if (ivbufferLen != blocksize){
        throw std::runtime_error ("Nounce size is not equal to the block size") ; 
    }

    int iterCount(10000);
    std::unique_ptr<unsigned char[]> encodingKey = KeyGen(myKey,key.size(),mySalt, blocksize,iterCount, keylen);

    std::string hexBuilder = binTohexStr(encodingKey, keylen);

#ifdef __EMSCRIPTEN__
    return hexBuilder.c_str(); 
#else
    return hexBuilder;
#endif
}

SYMENCDEC_C_API SYMENCDEC_RETURN_TYPE GenerateNounce(const int blocksize){

    std::unique_ptr<unsigned char[]> nounce;
    try{
        //change to return the nouce size. 
        NounceGen(nounce);
    }
    catch(const std::exception& e){
        std::cout << "Failed to generate a nounce value" << e.what () << std::endl;
        return std::string (); 
    }
    std::string returnNounce = binTohexStr(nounce, 16);
#ifdef __EMSCRIPTEN__
    return returnNounce.c_str(); 
#else
    return returnNounce;
#endif

}

