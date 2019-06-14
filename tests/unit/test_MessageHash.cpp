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

BOOST_AUTO_TEST_CASE(test_codec_base58check_particular_case)// This case has failed on PyMessageHash on Windows (functional test)
{
    const std::string text{ "Our PHD-qualified scientists and researchers are finding new ways to scale the Bitcoin network and ignite the power of blockchain technology Their discoveries will enable new functions of the Bitcoin SV blockchain and re-invent the way enterprises operate and transform people's lives" };
    const std::string encoded_text = EncodeBase58Checked(text);
    const std::string decoded_text = DecodeBase58Checked(encoded_text);
    BOOST_CHECK(text == decoded_text);
}

BOOST_AUTO_TEST_CASE(test_codec_base64_particular_case)// This case has failed on PyMessageHash on Windows (functional test)
{
    // This case fail, please make it pass
    const std::string text{ "nChain Limited UK branch is fast growing in FinTech industry" };
    const std::string encoded_text = EncodeBase64((const unsigned char*)text.data(), text.size());
    const std::string decoded_text = DecodeBase64((const unsigned char*)encoded_text.data(), text.size());
    BOOST_CHECK(text == decoded_text);
}

/// TODO add a loop : similar test with random inputs : random msg, decode(encode(msg)) should return the message
BOOST_AUTO_TEST_SUITE_END()