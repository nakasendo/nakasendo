#include <emscripten/bind.h>

#include <string>
#include <map>
#include <utility>

#include <AsymKey/AsymKey.h>

using namespace emscripten;


std::map<std::string, std::string> wasm_GenerateKeyPairPEM()
{
    AsymKey keyGen;
    std::map<std::string, std::string> m;
    m.insert(std::make_pair(keyGen.getPublicKeyPEM(), keyGen.getPrivateKeyPEM()));
    return m;
}

std::map<std::string, std::string> wasm_GenerateKeyPairHEX()
{
    AsymKey keyGen;
    std::map<std::string, std::string> m;
    m.insert(std::make_pair(keyGen.getPublicKeyHEXStr(), keyGen.getPrivateKeyHEX()));
    return m;
}

EMSCRIPTEN_BINDINGS(JsAsymKey) {
    function("GenerateKeyPairPEM", &wasm_GenerateKeyPairPEM);
    function("GenerateKeyPairHEX", &wasm_GenerateKeyPairHEX);

    // register bindings for std::map<std::string, std::string>
    register_map<std::string, std::string>("std::map<std::string, std::string>");
}