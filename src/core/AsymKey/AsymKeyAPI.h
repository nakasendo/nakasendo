#ifndef ASYM_KEY_API_H
#define ASYM_KEY_API_H

#include <AsymKey/AsymKey.h>
#include <utility>
#include <string>


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define AsymKey_C_API extern "C" AsymKey_API
#else
#include <memory>
#define AsymKey_C_API AsymKey_API
#endif

std::pair<std::string, std::string> AsymKey_API GenerateKeyPair();


#endif /* ASYM_KEY_API_H */
