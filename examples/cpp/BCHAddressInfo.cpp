#include <BCHAddressInfo.h>


std::unique_ptr<VersionConfig> VersionConfig::m_Instance = nullptr;


std::unique_ptr<VersionConfig>& VersionConfig::Instance(){
    if (m_Instance == nullptr){
        m_Instance.reset (new VersionConfig);
    }
    
    return m_Instance;
}

VersionConfig::VersionConfig() : mVersionPrefixMap {
    { VersionPrefix::PUBLIC_KEY_ADDRESS, { {0x00}, NetworkType::MAIN_NET } },
    { VersionPrefix::SCRIPT_ADDRESS, { {0x05}, NetworkType::MAIN_NET } },
    { VersionPrefix::PRIVATE_KEY, { {0x80}, NetworkType::MAIN_NET } },
    { VersionPrefix::TESTNET_PUBLIC_KEY_ADDRESS, { {0x6F},NetworkType::TEST_NET } },
    { VersionPrefix::TESTNET_SCRIPT_ADDRESS, { {0xC4}, NetworkType::TEST_NET } },
    { VersionPrefix::TESTNET_PRIVATE_KEY, { {0xEF}, NetworkType::TEST_NET } },
    { VersionPrefix::REGTEST_PUBLIC_KEY_ADDRESS, { {0x6F}, NetworkType::REGTEST } },
    { VersionPrefix::REGTEST_SCRIPT_ADDRESS, { {0xC4},NetworkType::REGTEST } },
    { VersionPrefix::REGTEST_PRIVATE_KEY, { {0xEF}, NetworkType::REGTEST } },
}
{
    return ;
}


const std::vector<uint8_t>& VersionConfig::getVersionBytes(const VersionPrefix& ver ){

    std::cout << EnumAsUnderlying(ver) << std::endl;
    VersionPrefixMap::const_iterator iter (mVersionPrefixMap.find(ver));
    if(iter == mVersionPrefixMap.end()){
        std::runtime_error err ("VersionConfig::getVersionBytes No version stored for version ");
        throw err;
    }
    return iter->second.first;
    
}

const NetworkType VersionConfig::getNetwork(const VersionPrefix& ver){
    VersionPrefixMap::const_iterator iter (mVersionPrefixMap.find(ver));
    if(iter == mVersionPrefixMap.end()){
        std::runtime_error err ("VersionConfig::getVersionBytes No version stored for version ");
        throw err;
    }
    return iter->second.second;
}

