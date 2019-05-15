#include <AsymKey/AsymKeyAPI.h>

std::pair<std::string, std::string> GenerateKeyPairPEM()
{
    AsymKey keyGen;
    return std::make_pair(keyGen.getPublicKeyPEM(), keyGen.getPrivateKeyPEM());
}