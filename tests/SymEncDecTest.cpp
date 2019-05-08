#include <string.h>
#include <iostream>
#include <string>

#include <sstream>
#include "SymmetricEncryptDecrypt/SymEncryptDecrypt.h"
#include "SymmetricEncryptDecrypt/SymEncryptDecryptAPI.h"

int main (int argc, char** argv)
{
    std::string UserPass ("mypassword");
    std::unique_ptr<unsigned char> myPass (new unsigned char [UserPass.length() + 1 ]);
    std::fill_n(myPass.get(), UserPass.length()+1, 0x00);
    int index(0);
    for(std::string::const_iterator iter = UserPass.begin(); iter != UserPass.end(); ++ iter, ++index){
      myPass.get()[index]=*iter;
    }
    std::string UserSalt ("0123456789");
    std::unique_ptr<unsigned char> mySalt (new unsigned char [UserSalt.length() + 1 ]);
    std::fill_n(mySalt.get(), UserSalt.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = UserSalt.begin(); iter != UserSalt.end(); ++ iter, ++index){
      mySalt.get()[index]=*iter;
    }
    std::unique_ptr<unsigned char> mykey;

    {      
      uint64_t keylen(32);
      int iterCount(10000);
      mykey = KeyGen(myPass,UserPass.length(),mySalt, UserSalt.length(), iterCount,keylen);
      
      if (mykey != nullptr){
          for ( int i = 0; i < keylen; ++i){
            //p += snprintf(p, 3, "%02x", *h);
            printf("%02x",mykey.get()[i]);
          }
          std::cout << std::endl; 
      }
    }
    {
 #if 1
      std::cout << "Set the paramerters:" << std::endl; 
      SymEncryptDecrypt symencdec;
      if (mykey != nullptr){
          for ( int i = 0; i < 32; ++i){
            //p += snprintf(p, 3, "%02x", *h);
            printf("%02x",mykey.get()[i]);
          }
          std::cout << std::endl; 
      }

      std::unique_ptr<unsigned char> genKey; 
      std::unique_ptr<unsigned char> genSalt; 
      unsigned int keysize = 0, saltsize = 0; 
      symencdec.GetParams (genKey, genSalt, keysize, saltsize);
      
      if (genKey != nullptr){
        std::cout << "default generated key" << std::endl;
        for (int i=0; i<32; ++ i){
          printf("%02x", genKey.get()[i]);
        }
        std::cout << std::endl; 
      }

      if (genSalt != nullptr){        
        std::cout << "default generated salt" << std::endl; 
        for (int i=0; i<16; ++ i){
          printf("%02x", genSalt.get()[i]);
        }
        std::cout << std::endl; 
      }
      
      symencdec.SetParams(mykey,mySalt, 32, 16); 

      std::string sharedSecret("Now is the time for all good men to come to the aide of their country");

      std::cout << "Calling aes_encrypt..." << std::endl; 
      //std::unique_ptr<unsigned char> encodedData = symencdec.aes_encrypt(sharedSecret);
      std::unique_ptr<unsigned char> encodedData;
      int encodedLen = symencdec.aes_encrypt(sharedSecret, encodedData);
      std::cout << "Encoded length: " << encodedLen << std::endl; 

      std::string outputData;
      for(int i=0;i<encodedLen;++i){
          outputData.push_back(encodedData.get()[i]);
      }
    
      std::unique_ptr<unsigned char> decodedData;
      int decodedLen = symencdec.aes_decrypt(outputData, decodedData);
      if(decodedData != nullptr){
        std::string decode; 
        for (int i=0;i<decodedLen;++i){
          decode.push_back(decodedData.get()[i]);
        }
        std::cout << "Decoded: " << decode << std::endl;
      }
#endif
    }
    {

      std::cout << "And now via the API" << std::endl;
      std::string ptext ("the quick brown fox jumped over the lazy dog");
      // Test key & IV
       /* A 256 bit key */
      //unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
      std::string UserPass ("j.murphy@nchain.com");
      std::string UserSalt ("blahblhablhah");
      
      std::string encMsg = Encode(ptext, UserPass, UserSalt);
      std::string decMsg = Decode(encMsg, UserPass, UserSalt);

      std::cout << "Decoded Message: " << decMsg << std::endl;

    }
  return 0 ; 
}
