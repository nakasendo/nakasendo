#ifndef __BIGNUM_API__
#define __BIGNUM_API__

#include <BigNumbers/BigNumbers.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define BIGNUM_RETURN_TYPE EMSCRIPTEN_KEEPALIVE const char *
#define BigNumbers_C_API extern "C" BigNumbers_API
#else
#include <memory>
#define BIGNUM_RETURN_TYPE std::unique_ptr<char>
#define BigNumbers_C_API BigNumbers_API
#endif

BigNumbers_C_API BIGNUM_RETURN_TYPE addFromHex(char*, char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE addFromDec(char *, char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE subFromHex (char*, char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE subFromDec(char*, char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomHex(const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomDec(const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomDecWithSeed(const char * seed, const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomHexWithSeed(const char *, const int size );

BigNumbers_API BIGNUM_RETURN_TYPE leftShift(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE rightShift(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE multiply(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE divide(const char*, const char*);


#endif //ifndef __BIGNUM_API__
