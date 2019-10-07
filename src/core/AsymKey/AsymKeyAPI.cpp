#include <AsymKey/AsymKeyAPI.h>

std::pair<std::string, std::string> GenerateKeyPairPEM()
{
    AsymKey keyGen;
    return std::make_pair(keyGen.exportPublicPEM(), keyGen.exportPrivatePEM());
}

std::pair<std::string, std::string> GenerateKeyPairHEX()
{
    AsymKey keyGen;
    return std::make_pair(keyGen.exportPublicHEXStr(), keyGen.exportPrivateHEX());
}

std::pair<std::string, std::string> exportKeyPairHEX(const std::string& crPrivateKeyPEM)
{
    AsymKey imported_key;
    imported_key.importPrivatePEM(crPrivateKeyPEM);
    return std::make_pair(imported_key.exportPublicHEXStr(), imported_key.exportPrivateHEX());
}