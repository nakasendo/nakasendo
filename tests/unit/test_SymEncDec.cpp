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
    BOOST_CHECK(msg_text== dencrypted);
}

/// TODO add a loop : similar test with random inputs : random msg, random key, random IV to make sure it is consistent
BOOST_AUTO_TEST_SUITE_END()