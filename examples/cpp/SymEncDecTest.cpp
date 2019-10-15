#include <string.h>
#include <iostream>
#include <string>

#include <sstream>
#include <SymEncDec/SymEncDec.h>
#include <SymEncDec/conversions.h>

int main (int argc, char** argv)
{
#if 0
    std::string UserPass ("j.murphy@nchain.com");
    std::unique_ptr<unsigned char[]> myPass (new unsigned char [UserPass.length() + 1 ]);
    std::fill_n(myPass.get(), UserPass.length()+1, 0x00);
    int index(0);
    for(std::string::const_iterator iter = UserPass.begin(); iter != UserPass.end(); ++ iter, ++index){
      myPass.get()[index]=*iter;
    }
    std::string UserSalt ("05101974");
    std::unique_ptr<unsigned char[]> mySalt (new unsigned char [UserSalt.length() + 1 ]);
    std::fill_n(mySalt.get(), UserSalt.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = UserSalt.begin(); iter != UserSalt.end(); ++ iter, ++index){
      mySalt.get()[index]=*iter;
    }
    std::unique_ptr<unsigned char[]> mykey;
     

    {      
      uint64_t keylen(32);
      int iterCount(10000);
      mykey = KeyGen(myPass,UserPass.length(),mySalt, UserSalt.length(), iterCount,keylen);
       std::unique_ptr<char[]> mykeyCopy (new char[keylen]);   
        std::string myKeyHex;

        std::stringstream hexBuilder; 
      if (mykey != nullptr){
          for ( int i = 0; i < keylen; ++i){
            //p += snprintf(p, 3, "%02x", *h);
            printf("%02x",mykey.get()[i]);
            sprintf(mykeyCopy.get(), "%02x", mykey.get()[i]); 
            hexBuilder << mykeyCopy.get() ; 
          }
          std::cout << "\n" << hexBuilder.str() << std::endl; 
        }

        // Take what's in hexBuilder & convert it back to a pointer of unsigned char.
        //var bytes = new Uint8Array(Math.ceil(hex.length / 2));
        //for (var i = 0; i < bytes.length; i++) bytes[i] = parseInt(hex.substr(i * 2, 2), 16);
        
        std::unique_ptr<unsigned char[]> myRebuiltKey ( new unsigned char[hexBuilder.str().length()/2]) ; 
        int i = 0 ; 
        for ( int len = 0; len < hexBuilder.str().length()/2; ++len ){
            std::string val =  hexBuilder.str().substr(len*2, 2) ; 
            myRebuiltKey.get()[len] = strtol ( val.c_str() , nullptr, 16) ; 
        }

        std::cout << "My rebuilt key:" << std::endl; 
        if ( myRebuiltKey != nullptr){
            for ( int i = 0; i < keylen; ++i){
                printf("%02x", myRebuiltKey.get()[i]); 
            }
        }
        std::cout << std::endl ;
    }
    {
 #if 1
      std::cout << "Set the paramerters:" << std::endl; 
      SymEncDec symencdec;
      if (mykey != nullptr){
          for ( int i = 0; i < 32; ++i){
            //p += snprintf(p, 3, "%02x", *h);
            printf("%02x",mykey.get()[i]);
          }
          std::cout << std::endl; 
      }

      std::unique_ptr<unsigned char[]> genKey; 
      std::unique_ptr<unsigned char[]> genSalt; 
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
      std::unique_ptr<unsigned char[]> encodedData;
      int encodedLen = symencdec.aes_encrypt(sharedSecret, encodedData);
      std::cout << "Encoded length: " << encodedLen << std::endl; 

      std::unique_ptr<char[]> vals = binToHex(encodedData, encodedLen);
      std::cout << vals.get() << std::endl; 

      std::string outputData;
      //for(int i=0;i<encodedLen;++i){
      for(int i=0;i<(strlen(vals.get())); ++i){
          outputData.push_back(vals.get()[i]);
      }
    
      size_t RebuiltMsgLen(0); 

      std::unique_ptr<unsigned char[]> RebuiltMsg = spc_hex2binTwo(outputData, &RebuiltMsgLen);
      
      std::string decodedData; 
      int decodedLen = symencdec.aes_decrypt(RebuiltMsg, RebuiltMsgLen, decodedData);
      
      std::cout << "Decoded: " << decodedData << std::endl;
      
#endif
    }
  #endif

    {
      std::cout << "API Test of nounce & key generation" << std::endl;
      std::string UserPass ("j.murphy@nchain.com"); 
      std::string nounceAsHex = GenerateNounce ();
      std::string keyAsHex = GenerateKey256(UserPass, nounceAsHex); 

      std::cout << "nounce: " << nounceAsHex << "\n" << "keyAsHex: " << keyAsHex << std::endl; 
    }
    {

      std::cout << "And now via the API ... the hex representations" << std::endl;
      std::string ptext ("Now is the time for all good men to come to the aide of their country");
      // Test key & IV
       /* A 256 bit key */
      
      std::string UserPass ("j.murphy@nchain.com");
      
      std::string nounceAsHex = GenerateNounce();
      std::string keyAsHex = GenerateKey256(UserPass, nounceAsHex);

      std::string encMsg = Encode(ptext, keyAsHex, nounceAsHex);
     
     
      std::string decMsg = Decode(encMsg, keyAsHex, nounceAsHex);
     
      std::cout << "Decoded Message: " << decMsg << std::endl;
      if (ptext != decMsg ){
        std::cout << "API PANIC!" << std::endl; 
      }
 
    }
    {

      std::cout << "And now via the API ... the hex representations .... long string" << std::endl;
      std::string ptext ("This is a test of a long message to encode. This is a test of a long message to encode. This is a test of a long message to encode. ");
      ptext += ("This is a test of a long message to encode. This is a test of a long message to encode. This is a test of a long message to encode. This is a test of a long message to encode. ");
      ptext += ("This is a test of a long message to encode. This is a test of a long message to encode. This is a test of a long message to encode. This is a test of a long message to encode. This is a test of a long message to encode. This is a test of a long message to encode. ");
      
      // Test key & IV
       /* A 256 bit key */
      
      std::string UserPass ("j.murphy@nchain.com");
      
      std::string nounceAsHex = GenerateNounce();
      std::string keyAsHex = GenerateKey256(UserPass, nounceAsHex);

      std::string encMsg = Encode(ptext, keyAsHex, nounceAsHex);
     
     
      std::string decMsg = Decode(encMsg, keyAsHex, nounceAsHex);
     
      std::cout << "Decoded Message: " << decMsg << std::endl;
      if (ptext != decMsg ){
        std::cout << "API PANIC!" << std::endl; 
      }
 
    }
  return 0 ; 
}
