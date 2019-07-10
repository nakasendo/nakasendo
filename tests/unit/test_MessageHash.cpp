#define BOOST_TEST_MODULE test_MessageHash
#include <boost/test/unit_test.hpp>

#include <string.h>
#include <iostream>
#include <memory>

#include <MessageHash/MessageHash.h>
#include <MessageHash/MessageHashAPI.h>

BOOST_AUTO_TEST_SUITE(test_suite_MessageHash)

BOOST_AUTO_TEST_CASE(test_codec_base58_particular_case)// This case has failed on PyMessageHash on Windows (functional test)
{
    const std::string text{ "Our PHD-qualified scientists and researchers are finding new ways to scale the Bitcoin network and ignite the power of blockchain technology Their discoveries will enable new functions of the Bitcoin SV blockchain and re-invent the way enterprises operate and transform people's lives" };
    const std::string encoded_text = EncodeBase58(text);
    const std::string decoded_text = DecodeBase58(encoded_text);
    BOOST_CHECK(text== decoded_text);
}

<<<<<<< HEAD
BOOST_AUTO_TEST_CASE(test_codec_base58check_particular_case)// This case has failed on PyMessageHash on Windows (functional test)
{
    const std::string text{ "Our PHD-qualified scientists and researchers are finding new ways to scale the Bitcoin network and ignite the power of blockchain technology Their discoveries will enable new functions of the Bitcoin SV blockchain and re-invent the way enterprises operate and transform people's lives" };
    const std::string encoded_text = EncodeBase58Checked(text);
    const std::string decoded_text = DecodeBase58Checked(encoded_text);
    BOOST_CHECK(text == decoded_text);
=======

BOOST_AUTO_TEST_CASE(test_codec_base58)
{
  std::string msgVal ("Murphy the monkey and bradley the kinkajou are friends"); 
  std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
  std::vector<uint8_t> vec; 
  int index(0);
  for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
      msgPtr.get()[index] = *iter ; 
      vec.push_back(*iter);
      ++ index; 
  }        
  Base58EncDec encdec ; 
  std::string retValPtr =  encdec.encode (vec);
  std::vector<uint8_t> decodedVec = encdec.decode(retValPtr);
  std::string decodedRes;
  for(std::vector<uint8_t>::const_iterator iter = decodedVec.begin(); iter!=decodedVec.end(); ++iter){
      decodedRes.push_back(*iter); 
  }
  BOOST_CHECK(msgVal == decodedRes);
}
BOOST_AUTO_TEST_CASE(test_codec_base58_checked)
{
  {
    std::string msgVal ("The quick brown fox jumped over the lazy dog"); 
    std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
    std::vector<uint8_t> vec; 
    int index(0);
    for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
        msgPtr.get()[index] = *iter ; 
        vec.push_back(*iter);
        ++ index; 
    }        
    Base58EncDec encdec ; 
    std::string retValPtr =  encdec.encodeCheck(vec);

    std::vector<uint8_t> decodedVec = encdec.decodeCheck(retValPtr);
    std::string decodedRes;
    for(std::vector<uint8_t>::const_iterator iter = decodedVec.begin(); iter!=decodedVec.end(); ++iter){
        decodedRes.push_back(*iter); 
    }

    BOOST_CHECK(msgVal == decodedRes);
  }
>>>>>>> 72e2bba... Commited for Base64 encoding and decoding issues
}

BOOST_AUTO_TEST_CASE(test_codec_base64_particular_case)// This case has failed on PyMessageHash on Windows (functional test)
{
    // This case fail, please make it pass
    const std::string text{ "nChain Limited UK branch is fast growing in FinTech industry" };
<<<<<<< HEAD
<<<<<<< HEAD
    const std::string encoded_text = EncodeBase64((const unsigned char*)text.data(), text.size());
    const std::string decoded_text = DecodeBase64((const unsigned char*)encoded_text.data(), text.size());
    BOOST_CHECK(text == decoded_text);
=======
=======
>>>>>>> 18a824c... Commited for Base64 encoding and decoding issues
    std::unique_ptr<unsigned char[]> textPtr(new unsigned char[text.size()]);
    int index(0);
    for(std::string::const_iterator iter = text.begin();iter != text.end(); ++iter){
      textPtr.get()[index++] = *iter ;
    }
    const std::string encoded_text = EncodeBase64(textPtr, text.size());

    int decodedLen(0);
    std::unique_ptr<unsigned char[]> decodedptr = DecodeBase64(encoded_text, decodedLen);
    std::string decoded_text;
    for(int i=0;i<decodedLen;++i){
      decoded_text.push_back(decodedptr.get()[i]);
    }
    BOOST_CHECK(text == decoded_text);
  }
  {
    const std::string text{ "When I was a young man I carried my pack and I lived the free life of a rover" };
    std::unique_ptr<unsigned char[]> textPtr(new unsigned char[text.size()]);
    int index(0);
    for(std::string::const_iterator iter = text.begin();iter != text.end(); ++iter){
      textPtr.get()[index++] = *iter ;
    }
    const std::string encoded_text = EncodeBase64(textPtr, text.size());

    int decodedLen(0);
    std::unique_ptr<unsigned char[]> decodedptr = DecodeBase64(encoded_text, decodedLen);
    std::string decoded_text;
    for(int i=0;i<decodedLen;++i){
      decoded_text.push_back(decodedptr.get()[i]);
    }
    BOOST_CHECK(text == decoded_text);
  } 
>>>>>>> 72e2bba... Commited for Base64 encoding and decoding issues
}

/// TODO add a loop : similar test with random inputs : random msg, decode(encode(msg)) should return the message
BOOST_AUTO_TEST_SUITE_END()