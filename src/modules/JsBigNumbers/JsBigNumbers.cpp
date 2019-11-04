#include <string>

#include <emscripten/bind.h>

#include <BigNumbers/BigNumbers.h>

using namespace emscripten;

std::string wasm_RandHex(const int size)
{    
    BigNumber bn;
    const std::string rand_hex = bn.generateRandHex(size);
    return rand_hex;
}

std::string wasm_RandDec(const int size)
{    
    BigNumber bn;
    const std::string rand_dec = bn.generateRandDec(size);
    return rand_dec;
}

EMSCRIPTEN_BINDINGS(JsBigNumbers) {
    function("RandHex", &wasm_RandHex);
    function("RandDec", &wasm_RandDec);
}