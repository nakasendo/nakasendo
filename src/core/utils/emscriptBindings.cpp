#ifdef __EMSCRIPTEN__


#include "utils/emscriptBindings.h"
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> returnEmptyVector () {
  std::vector<uint8_t> v;
  return v;
}

EMSCRIPTEN_BINDINGS(moduleUtils) {

    emscripten::function("returnEmptyVector", &returnEmptyVector);  
    emscripten::register_vector<uint8_t>("std::vector<uint8_t>");
  
}
#endif