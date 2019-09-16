#include <string>

#include <emscripten/bind.h>

#include <ECPoint/ECPoint.h>

using namespace emscripten;

std::string wasm_ECRandHex()
{
    ECPoint rand_ec;
    rand_ec.SetRandom();
    return rand_ec.ToHex();
}

EMSCRIPTEN_BINDINGS(JsECPoint) {
    function("ECRandHex", &wasm_ECRandHex);
}