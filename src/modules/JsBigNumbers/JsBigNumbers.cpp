#include <string>

#include <emscripten/bind.h>

#include <BigNumbers/BigNumbers.h>

using namespace emscripten;

std::string wasm_RandHex(const int size)
{    
    BigNumber rand_bn = GenerateRand (size);
    return rand_bn.ToHex();
}

std::string wasm_RandDec(const int size)
{    
    BigNumber rand_bn = GenerateRand (size);
    return rand_bn.ToDec();
}

EMSCRIPTEN_BINDINGS(JsBigNumbers) {
    function("RandHex", &wasm_RandHex);
    function("RandDec", &wasm_RandDec);
}