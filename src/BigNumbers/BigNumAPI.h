#ifndef __BIGNUM_API__
#define __BIGNUM_API__

#include <DYNAMIC_LIBRARY_API.hpp>

#ifdef EXPORT_BigNumbers
#    define BigNumbers_API EXPORT_DYNAMIC_LIBRARY
#else
#    define BigNumbers_API IMPORT_DYNAMIC_LIBRARY
#endif


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define BIGNUM_RETURN_TYPE EMSCRIPTEN_KEEPALIVE const char *
#else
#include <memory>
#define BIGNUM_RETURN_TYPE std::unique_ptr<char>
#endif


BigNumbers_API BIGNUM_RETURN_TYPE addFromHex(char*, char*);
BigNumbers_API BIGNUM_RETURN_TYPE addFromDec(char *, char*);
BigNumbers_API BIGNUM_RETURN_TYPE subFromHex (char*, char*);
BigNumbers_API BIGNUM_RETURN_TYPE subFromDec(char*, char*);
BigNumbers_API BIGNUM_RETURN_TYPE BNRandomHex(const int size);
BigNumbers_API BIGNUM_RETURN_TYPE BNRandomDec(const int size);
BigNumbers_API BIGNUM_RETURN_TYPE BNRandomHexWithSeed(const char *, const int size );


#endif //ifndef __BIGNUM_API__
