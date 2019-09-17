/// Define test module name with debug postfix
#ifdef NDEBUG 
#  define BOOST_TEST_MODULE test_BCHAddress
#else
#  define BOOST_TEST_MODULE test_BCHAddressd
#endif

#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>

#include <BCHAddress/BCHAddress.h>
#include <BCHAddress/BCHAddressInfo.h>
#include <AsymKey/AsymKey.h>


#include <string>
#include <iostream>





BOOST_AUTO_TEST_SUITE(test_suite_BCHAddress)

BOOST_AUTO_TEST_CASE( test_number_one )
{

    AsymKey key;
    BOOST_CHECK(key.is_valid());


    VersionPrefix version = VersionPrefix::PUBLIC_KEY_ADDRESS ; // main_net

    BCHAddress addressOne 
        ( 
            key.getPublicKeyHEXStr( ), 
            VersionPrefix::PUBLIC_KEY_ADDRESS  
        ) ;

    // check is validity
    BOOST_CHECK( addressOne.valid() == true ) ;
    BOOST_CHECK( addressOne.getVersion().first == "00" ) ;
    BOOST_CHECK( addressOne.getVersionPrefix() == VersionPrefix::PUBLIC_KEY_ADDRESS  ) ;


    BCHAddress addressTwo 
        ( 
            "023cba1f4d12d1ce0bced725373769b2262c6daa97be6a0588cfec8ce1a5f0bd09", 
            VersionPrefix::TESTNET_PUBLIC_KEY_ADDRESS 
        ) ;

    // check validity
    BOOST_CHECK( addressTwo.valid() == true ) ;
    BOOST_CHECK( addressTwo.getVersion().first == "6f" ) ;
    BOOST_CHECK( addressTwo.getVersionPrefix() == VersionPrefix::TESTNET_PUBLIC_KEY_ADDRESS  ) ;


    // check creating from existing address
    BCHAddress validAddress ( "17nDmDt3ZsHqQWAwuc5H8y7cNdZqDyfXAd") ;

    BOOST_CHECK( validAddress.valid() == true ) ;
    BOOST_CHECK( validAddress.decode() == "004a5e2064e49baf44a7811a40b7138b46350ab4f3ad766b56" ) ;


    // check creating from invalid address
    BOOST_CHECK_THROW
        ( 
            BCHAddress invalidAddress ( "017nDmDt3ZsHqQWAwuc5H8y7cNdZqDyfXAd") ,
            std::runtime_error  
        );
}

BOOST_AUTO_TEST_CASE( test_against_web )
{
    VersionPrefix version = VersionPrefix::PUBLIC_KEY_ADDRESS ; // main_net
    
    // http://gobittest.appspot.com/Address

    std::vector < std::string > keys 
    {
        "0250863ad64a87ae8a2fe83c1af1a8403cb53f53e486d8511dad8a04887e5b2352",
        "023cba1f4d12d1ce0bced725373769b2262c6daa97be6a0588cfec8ce1a5f0bd09",
        "04CEDDDD570F85247B47C422FEB48C5DE707CEF9DA4CAD601EEDD4DFD532FD4FD2EBC327CFE5AD890CF7897BE959F56D51FF6D8731DD320272E53C16DD3E0A0F55",
        "04275AC577EBDBCD55A1CC90C1412FC53ADBBFC6BCF874BD8AE0C3C20C745FE268CF3552CB2E1D39F71E349507DB6762E34A8D03CD1FBE0014CA4BDBF85C89F200",
        "04C2576C7B2B0001733AACCA9377AB39A11096074E3683CF447935C66C3D9B42303DFBB3E1B31EFDA03B0F6F1F22ECE77E3CC37BCF94192EDACE7637A7BA3269AD",
        "0402B3AE96CB64D87B386BD4DE6B26C5F17E10007CCC668B647873FA641CB376ABAA80FFEB6B5F01828A6530C386C3075E5C47B0606B0CC7092C813F56F066EB92"
    } ;

    std::vector < std::string > addresses
    {
        "1PMycacnJaSqwwJqjawXBErnLsZ7RkXUAs",
        "16JrGhLx5bcBSA34kew9V6Mufa4aXhFe9X",
        "1P4errpos8jpzPGLQEngSBctYa5kQnztre",
        "17nDmDt3ZsHqQWAwuc5H8y7cNdZqDyfXAd",
        "1CX6UeYvUu8nRjph4sv7AC9U2FKAonZpyP",
        "1MMsBKYNSSRbanFBRmpmwPXHWCCCfDrczt"
    } ;

    for (
            auto key = keys.begin(), address = addresses.begin( ) ; 
            key != keys.end(); 
            ++key, ++address
        ) 
    {
        BCHAddress bch( *key, version ) ;
        BOOST_CHECK ( bch.getAddress( ) == *address ) ;
    }  

}



BOOST_AUTO_TEST_SUITE_END()
