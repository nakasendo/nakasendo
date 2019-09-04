#include <emscripten/bind.h>

#include <string>
#include "CoreModuleFile1.hpp"
#include "CoreModuleFile2.hpp"


using namespace emscripten;


std::string wasm_hash256_1(const std::string& crIn){
    return hash_sha256_1(crIn);
}

std::string wasm_hash256_2(const std::string& crIn){
    return hash_sha256_2(crIn);
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
    function("hash_sha256_1", &wasm_hash256_1);
    function("hash_sha256_2", &wasm_hash256_2);
}