#include <AsymKey/AsymKeyAPI.h>

std::pair<std::string, std::string> GenerateKeyPair()
{
    AsymKey keyGen;
    return std::make_pair(keyGen.getPublicKey(), keyGen.getPrivateKey());
}