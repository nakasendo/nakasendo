#include <emscripten/bind.h>

#include <string>
#include "CoreModule.hpp"


using namespace emscripten;


std::string wasm_hash256(const std::string& crIn){
    return hash_sha256(crIn);
}

std::string wasm_say_hello(const std::string& name) {
    return std::string{"hello " + name};
}

float wasm_wrap_lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

EMSCRIPTEN_BINDINGS(JsModule) {
    function("lerp", &wasm_wrap_lerp);
    function("say_hello", &wasm_say_hello);
    function("hash_sha256", &wasm_hash256);
}