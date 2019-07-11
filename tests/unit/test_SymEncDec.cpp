#define BOOST_TEST_MODULE test_SymEncDec
#include <boost/test/unit_test.hpp>

#include <SymEncDec/SymEncDec.h>
#include <SymEncDec/SymEncDecAPI.h>
#include <string>
#include <vector>
#include <utility>

BOOST_AUTO_TEST_SUITE(test_suite_SymEncDec)

/// TODO add test cases
BOOST_AUTO_TEST_CASE(test_encryp_decrypt_API)
{
    // This case fail
    const std::string msg_text{ "I am a message to encrypt" };
    const std::string secret_key{ "MyKey" };
    const std::string iv{ "IV" };
    const std::string encrypted = Encode(msg_text, secret_key, iv);

    const std::string dencrypted = Decode(encrypted, secret_key, iv);
    std::cout << msg_text << "\n" << dencrypted << std::endl;

    BOOST_CHECK(msg_text== dencrypted);
}

/// TODO add a loop : similar test with random inputs : random msg, random key, random IV to make sure it is consistent

BOOST_AUTO_TEST_CASE(test_encryp_decrypt_CLASS_IMPL_T1)
{
  const std::string msg_text{ "I am a message to encrypt" };
  const std::string secret_key{ "MyKey" };
  const std::string iv{ "IV" };
  std::unique_ptr<unsigned char[]> myPass (new unsigned char [secret_key.length() + 1 ]);
  std::fill_n(myPass.get(), secret_key.length()+1, 0x00);
  int index(0);
  for(std::string::const_iterator iter = secret_key.begin(); iter != secret_key.end(); ++ iter, ++index){
    myPass.get()[index]=*iter;
  }

  std::unique_ptr<unsigned char[]> mySalt (new unsigned char [iv.length() + 1 ]);
  std::fill_n(mySalt.get(), iv.length()+1, 0x00);
  index = 0;
  for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
    mySalt.get()[index]=*iter;
  }

    
  uint64_t keylen(32);
  int iterCount(10000);
  std::unique_ptr<unsigned char[]> mykey = KeyGen(myPass,secret_key.length(),mySalt, iv.length(), iterCount,keylen);
  //std::cout << "Set the paramerters:" << std::endl; 
  SymEncDec symencdec;

    
  symencdec.SetParams(mykey,mySalt, 32, 16); 

    

    //std::cout << "Calling aes_encrypt..." << std::endl; 
    //std::unique_ptr<unsigned char> encodedData = symencdec.aes_encrypt(sharedSecret);
    std::unique_ptr<unsigned char[]> encodedData;
    int encodedLen = symencdec.aes_encrypt(msg_text, encodedData);
    //std::cout << "Encoded length: " << encodedLen << std::endl; 

    std::string outputData;
    int decodedLen = symencdec.aes_decrypt(encodedData,encodedLen, outputData);


  //std::cout << "Encoded: " << outputData << "\n" << "Decoded: " << decode << std::endl; 
  BOOST_CHECK(msg_text== outputData);
}
BOOST_AUTO_TEST_CASE(test_encryp_decrypt_CLASS_IMPL_T2)
{
    const std::string msg_text{ "its now or never. Give it to me. Delicious icecream. from ITALY!" };
    const std::string secret_key{ "j.murphy@nchain.com" };
    const std::string iv{ "05101974" };

    std::unique_ptr<unsigned char[]> myPass (new unsigned char [secret_key.length() + 1 ]);
    std::fill_n(myPass.get(), secret_key.length()+1, 0x00);
    int index(0);
    for(std::string::const_iterator iter = secret_key.begin(); iter != secret_key.end(); ++ iter, ++index){
      myPass.get()[index]=*iter;
    }

    std::unique_ptr<unsigned char[]> mySalt (new unsigned char [iv.length() + 1 ]);
    std::fill_n(mySalt.get(), iv.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
      mySalt.get()[index]=*iter;
    }

      
    uint64_t keylen(32);
    int iterCount(10000);
    std::unique_ptr<unsigned char[]> mykey = KeyGen(myPass,secret_key.length(),mySalt, iv.length(), iterCount,keylen);
    //std::cout << "Set the paramerters:" << std::endl; 
    SymEncDec symencdec;

      
    symencdec.SetParams(mykey,mySalt, 32, 16); 

      

      //std::cout << "Calling aes_encrypt..." << std::endl; 
      //std::unique_ptr<unsigned char> encodedData = symencdec.aes_encrypt(sharedSecret);
      std::unique_ptr<unsigned char[]> encodedData;
      int encodedLen = symencdec.aes_encrypt(msg_text, encodedData);
      //std::cout << "Encoded length: " << encodedLen << std::endl; 

      std::string outputData;

      int decodedLen = symencdec.aes_decrypt(encodedData,encodedLen, outputData);
     
    //std::cout << "Encoded: " << outputData << "\n" << "Decoded: " << decode << std::endl; 
    BOOST_CHECK(msg_text== outputData);
}
BOOST_AUTO_TEST_CASE(test_encryp_decrypt_CLASS_IMPL_T3)
{
    const std::string msg_text{ "hello short message" };
    const std::string secret_key{ "j.murphy@nchain.com" };
    const std::string iv{ "05101974" };

    std::unique_ptr<unsigned char[]> myPass (new unsigned char [secret_key.length() + 1 ]);
    std::fill_n(myPass.get(), secret_key.length()+1, 0x00);
    int index(0);
    for(std::string::const_iterator iter = secret_key.begin(); iter != secret_key.end(); ++ iter, ++index){
      myPass.get()[index]=*iter;
    }

    std::unique_ptr<unsigned char[]> mySalt (new unsigned char [iv.length() + 1 ]);
    std::fill_n(mySalt.get(), iv.length()+1, 0x00);
    index = 0;
    for(std::string::const_iterator iter = iv.begin(); iter != iv.end(); ++ iter, ++index){
      mySalt.get()[index]=*iter;
    }

      
    uint64_t keylen(32);
    int iterCount(10000);
    std::unique_ptr<unsigned char[]> mykey = KeyGen(myPass,secret_key.length(),mySalt, iv.length(), iterCount,keylen);
    //std::cout << "Set the paramerters:" << std::endl; 
    SymEncDec symencdec;

      
    symencdec.SetParams(mykey,mySalt, 32, 16); 

      

      //std::cout << "Calling aes_encrypt..." << std::endl; 
      //std::unique_ptr<unsigned char> encodedData = symencdec.aes_encrypt(sharedSecret);
      std::unique_ptr<unsigned char[]> encodedData;
      int encodedLen = symencdec.aes_encrypt(msg_text, encodedData);
      //std::cout << "Encoded length: " << encodedLen << std::endl; 

      std::string outputData;
      
      SymEncDec symdec;

      
      symdec.SetParams(mykey,mySalt, 32, 16); 

      int decodedLen = symdec.aes_decrypt(encodedData,encodedLen, outputData);
      

    //std::cout << "Encoded: " << outputData << "\n" << "Decoded: " << decode << std::endl; 
    BOOST_CHECK(msg_text== outputData);
}

BOOST_AUTO_TEST_SUITE_END()

