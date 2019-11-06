#ifndef __BSV_ADDRESS_INFO_H__
#define __BSV_ADDRESS_INFO_H__

#include <type_traits>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <tuple>


/// Convert an enum class value to its underlying type (for example, to print it)
template <typename Enumeration>
auto EnumAsUnderlying(Enumeration value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}


    /// Metadata value types for key = NETWORK_TYPE
enum class NetworkType
{
    UNKNOWN,
    MAIN_NET,
    TEST_NET,
    REGTEST
};
    
enum class VersionPrefix
{
    PUBLIC_KEY_ADDRESS,
    SCRIPT_ADDRESS,
    PRIVATE_KEY,
 
    TESTNET_PUBLIC_KEY_ADDRESS,
    TESTNET_SCRIPT_ADDRESS,
    TESTNET_PRIVATE_KEY,
 
    REGTEST_PUBLIC_KEY_ADDRESS,
    REGTEST_SCRIPT_ADDRESS,
    REGTEST_PRIVATE_KEY,
};


// map of enum (version) : tuple (  < network byte > | Network type | human-readable-description )
using VersionPrefixMap = std::map
    <
        VersionPrefix, 
        std::tuple
            < 
                std::vector<uint8_t>,
                 NetworkType, 
                 std::string     
            >
    > ;




class VersionConfig
{
    public:
    
        static std::unique_ptr<VersionConfig>& Instance();
        const std::vector<uint8_t>& getVersionBytes(const VersionPrefix& ) const ; 
        NetworkType getNetwork(const VersionPrefix&) const ;
        VersionPrefix getVersionConfig(const std::vector<uint8_t>& bytes ) const ;
        std::string getReadableNetworkType(const std::vector<uint8_t>& bytes ) const ;
    private:
        VersionConfig();
        VersionConfig(const VersionConfig&);
        
        static std::unique_ptr<VersionConfig> m_Instance;
        VersionPrefixMap mVersionPrefixMap;          
};
   
#endif //#ifndef __BSV_ADDRESS_INFO_H__

