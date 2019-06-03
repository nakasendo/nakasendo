#define BOOST_TEST_MODULE test_AsymKey
#include <boost/test/unit_test.hpp>

#include <AsymKey/AsymKey.h>
#include <AsymKey/AsymKeyAPI.h>
#include <BigNumbers/BigNumbers.h>

#include <string>
#include <vector>
#include <utility>

#include <openssl/objects.h>

BOOST_AUTO_TEST_SUITE(test_suite_AsymKey)

BOOST_AUTO_TEST_CASE(test_constructor)
{
    AsymKey key;
    
    const std::string pubKeyStr = key.getPublicKeyPEM();
    const std::string priKeyStr = key.getPrivateKeyPEM();
    BOOST_CHECK(!pubKeyStr.empty());
    BOOST_CHECK(!priKeyStr.empty());
}

BOOST_AUTO_TEST_CASE(test_randomness)
{
    const size_t nbSample = 10;
    std::vector<std::pair<std::string, std::string>> list_keystr;
    /// Generate list of keys
    for (size_t i = 0; i < nbSample; ++i) 
    {
        AsymKey key;
        list_keystr.push_back(std::make_pair(key.getPublicKeyPEM(), key.getPrivateKeyPEM()));
    }

    for (size_t i = 0; i < nbSample; ++i)
    {
        for (size_t j = i+1; j < nbSample; ++j)
        {
            const std::pair<std::string, std::string>& pair_key_i = list_keystr[i];
            const std::pair<std::string, std::string>& pair_key_j = list_keystr[j];
            BOOST_TEST(pair_key_i.first  != pair_key_j.first  );// public keys are differents
            BOOST_TEST(pair_key_i.second != pair_key_j.second );// private keys are differents
        }
    }
}

BOOST_AUTO_TEST_CASE(test_copy)
{
    const AsymKey key;
    const std::string pubkey_str = key.getPublicKeyPEM();
    const std::string prikey_str = key.getPrivateKeyPEM();

    const AsymKey copied(key);
    const std::string copied_pubkey_str = copied.getPublicKeyPEM();
    const std::string copied_prikey_str = copied.getPrivateKeyPEM();
    BOOST_TEST(pubkey_str == copied_pubkey_str);// copied public keys are same
    BOOST_TEST(prikey_str == copied_prikey_str);// copied private keys are same

    AsymKey assigned;
    assigned = key;
    const std::string assigned_pubkey_str = assigned.getPublicKeyPEM();
    const std::string assigned_prikey_str = assigned.getPrivateKeyPEM();
    BOOST_TEST(pubkey_str == assigned_pubkey_str);// assigned public keys are same
    BOOST_TEST(prikey_str == assigned_prikey_str);// assigned private keys are same
}

BOOST_AUTO_TEST_CASE(test_IO)
{
    const AsymKey test_key;
    const int test_groupID = test_key.GroupNid();
    const std::string test_pubkey_hex = test_key.getPublicKeyHEX();
    const std::string test_prikey_hex = test_key.getPrivateKeyHEX();
    BOOST_TEST(test_groupID == OBJ_txt2nid("secp256k1"));
    BOOST_TEST(!test_pubkey_hex.empty());
    BOOST_TEST(!test_prikey_hex.empty());

    const std::string test_pubkey_str = test_key.getPublicKeyPEM();
    const std::string test_priKey_Str = test_key.getPrivateKeyPEM();

    AsymKey imported_key;
    imported_key.setPEMPrivateKey(test_priKey_Str);
    const std::string imported_pubkey_str = imported_key.getPublicKeyPEM();
    const std::string imported_priKey_Str = imported_key.getPrivateKeyPEM();

    BOOST_TEST(test_pubkey_str == imported_pubkey_str);// public keys are same
    BOOST_TEST(test_priKey_Str == imported_priKey_Str);// private keys are same
}

BOOST_AUTO_TEST_CASE(test_API)
{
    const std::pair<std::string, std::string> keyPairPEM = GenerateKeyPairPEM();
    BOOST_CHECK(!keyPairPEM.first.empty());
    BOOST_CHECK(!keyPairPEM.second.empty());

    const std::pair<std::string, std::string> keyPairHEX = GenerateKeyPairHEX();
    BOOST_CHECK(!keyPairHEX.first.empty());
    BOOST_CHECK(!keyPairHEX.second.empty());
}

BOOST_AUTO_TEST_CASE(test_Sig_Verify)
{
    const std::string msg{"Alice want to say hello to Bob"};
    const AsymKey ecdsa;
    const std::string pubkey = ecdsa.getPublicKeyPEM();
    const std::pair<std::string, std::string> rs = ecdsa.sign(msg);
    const bool verify_ok = verify(msg, pubkey,rs);
    BOOST_CHECK(verify_ok);
}

BOOST_AUTO_TEST_CASE(test_Sig_Verify_Random)
{
    const size_t nbIter = 10;
    for (size_t i = 0; i < nbIter; ++i)
    {
        BigNumber randBN = GenerateRand(100);
        const std::string random_str = randBN.ToHex();
        const AsymKey ecdsa;
        const std::string pubkey = ecdsa.getPublicKeyPEM();
        const std::pair<std::string, std::string> rs = ecdsa.sign(random_str);
        const bool verify_ok = verify(random_str, pubkey, rs);
        BOOST_CHECK(verify_ok);
    }
}

BOOST_AUTO_TEST_CASE(test_SharedSecret)
{
    // Test a few iteration with keys randomly generated
    const size_t nbIter = 10;
    for (size_t i = 0; i < nbIter; ++i)
    {
        const AsymKey alice_key;
        const AsymKey bob_key;

        const std::string alice_pub_key = alice_key.getPublicKeyPEM();
        const std::string bob_pub_key = bob_key.getPublicKeyPEM();

        const std::string shared_secret_from_bob = bob_key.getSharedSecretHex(alice_pub_key);
        const std::string shared_secret_from_alice = alice_key.getSharedSecretHex(bob_pub_key);
        BOOST_CHECK(shared_secret_from_bob == shared_secret_from_alice);
    }
}

BOOST_AUTO_TEST_SUITE_END()