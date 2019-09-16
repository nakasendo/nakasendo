#include <string>

#include <emscripten/bind.h>

#include <MessageHash/MessageHash.h>

using namespace emscripten;

std::string wasm_Hash256(const std::string& msg)
{
    MessageHash hasher;
    hasher.HashSha256(msg);
    return hasher.HashHex();
}

EMSCRIPTEN_BINDINGS(JsMessageHash) {
    function("Hash256", &wasm_Hash256);
}