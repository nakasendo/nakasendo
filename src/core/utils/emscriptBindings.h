#ifndef __EMSCRIPT_BINDINGS_H__
#define __EMSCRIPT_BINDINGS_H__

#include <vector>
#include <string>
#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> returnEmptyVector ();
#endif

#endif
