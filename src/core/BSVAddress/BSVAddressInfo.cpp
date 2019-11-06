#include <BSVAddress/BSVAddressInfo.h>
#include <MessageHash/conversions.h>

std::unique_ptr<VersionConfig> VersionConfig::m_Instance = nullptr;


std::unique_ptr<VersionConfig>& VersionConfig::Instance(){
    if (m_Instance == nullptr){
        m_Instance.reset (new VersionConfig);
    }
    
    return m_Instance;
}

VersionConfig::VersionConfig() 
    : 
    mVersionPrefixMap 
    {
        //{ VersionPrefix::PUBLIC_KEY_ADDRESS, std::make_tuple( 0x00, NetworkType::MAIN_NET, "Public Key Address" ) },
        { VersionPrefix::PUBLIC_KEY_ADDRESS, { {0x00}, NetworkType::MAIN_NET, "Public Key Address" } },
        { VersionPrefix::SCRIPT_ADDRESS, { {0x05}, NetworkType::MAIN_NET, "Script Address" } },
        { VersionPrefix::PRIVATE_KEY, { {0x80}, NetworkType::MAIN_NET , "Private Key" } },
        { VersionPrefix::TESTNET_PUBLIC_KEY_ADDRESS,  { {0x6F},NetworkType::TEST_NET, "Testnet Public Key Address"  }  },
        { VersionPrefix::TESTNET_SCRIPT_ADDRESS, { {0xC4}, NetworkType::TEST_NET, "Testnet Script Address" } },
        { VersionPrefix::TESTNET_PRIVATE_KEY, { {0xEF}, NetworkType::TEST_NET ,  "Testnet Private Key" } },
        { VersionPrefix::REGTEST_PUBLIC_KEY_ADDRESS,  { {0x6F}, NetworkType::REGTEST, "Regtest Public Key Address" }  },
        { VersionPrefix::REGTEST_SCRIPT_ADDRESS,  { {0xC4},NetworkType::REGTEST , "Regtest Script Address" } },
        { VersionPrefix::REGTEST_PRIVATE_KEY,  { {0xEF}, NetworkType::REGTEST , "Regtest Private Key"}  },
    }
{
    return ;
}


const std::vector<uint8_t>& VersionConfig::getVersionBytes(const VersionPrefix& ver ) const
{

    VersionPrefixMap::const_iterator iter (mVersionPrefixMap.find(ver));
    if(iter == mVersionPrefixMap.end()){
        std::runtime_error err ("VersionConfig::getVersionBytes No version stored for version ");
        throw err;
    }

    // return the versionPrefix
    return std::get< 0 >( iter->second ) ;
    
}

NetworkType VersionConfig::getNetwork(const VersionPrefix& ver) const
{
    VersionPrefixMap::const_iterator iter (mVersionPrefixMap.find(ver));
    if(iter == mVersionPrefixMap.end()){
        std::runtime_error err ("VersionConfig::getVersionBytes No version stored for version ");
        throw err;
    }

    // return the enum for network type
    return std::get< 1 >( iter->second ) ;
}


VersionPrefix VersionConfig::getVersionConfig(const std::vector<uint8_t>& bytes ) const
{
    for (auto const& x : mVersionPrefixMap )
    {
        // if versionPrefix  == bytes   
        if ( bytes == std::get< 0 >( x.second ) )        
            return x.first ;
    }

    std::runtime_error err ("VersionConfig::getVersionConfig version prefix not found" ) ;
    throw err ;
}

std::string VersionConfig::getReadableNetworkType(const std::vector<uint8_t>& bytes ) const
{
    for (auto const& x : mVersionPrefixMap )
    {
        // if versionPrefix  == bytes   
        if ( bytes == std::get< 0 >( x.second ) )        
            return std::get< 2 >( x.second ) ;
    }

    std::runtime_error err ("VersionConfig::getVersionConfig version prefix not found" ) ;
    throw err ;
}
