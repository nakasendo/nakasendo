#include <AsymKey/AsymKeyAPI.h>

std::pair<std::string, std::string> GenerateKeyPairPEM()
{
    AsymKey keyGen;
    return std::make_pair(keyGen.getPublicKeyPEM(), keyGen.getPrivateKeyPEM());
}

std::pair<std::string, std::string> GenerateKeyPairHEX()
{
    AsymKey keyGen;
    return std::make_pair(keyGen.getPublicKeyHEX(), keyGen.getPrivateKeyHEX());
}

std::pair<std::string, std::string> GetKeyPairHEX(const std::string& crPrivateKeyPEM)
{
    AsymKey imported_key;
    imported_key.setPEMPrivateKey(crPrivateKeyPEM);
    return std::make_pair(imported_key.getPublicKeyHEX(), imported_key.getPrivateKeyHEX());
}