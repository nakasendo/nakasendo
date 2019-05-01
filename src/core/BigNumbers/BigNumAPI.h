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

BigNumbers_API BIGNUM_RETURN_TYPE leftShiftFromHex(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE leftShiftFromDec(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE rightShiftFromHex(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE rightShiftFromDec(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE multiplyFromHex(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE multiplyFromDec(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE divideFromHex(const char*, const char*);
BigNumbers_API BIGNUM_RETURN_TYPE divideFromDec(const char*, const char*);

BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomPrimeHex(const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomPrimeDec(const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomPrimeHexWithSeed(const char * seed, const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomPrimeDecWithSeed(const char * seed, const int size);

BigNumbers_C_API bool isPrimeHex(const char* pBNHex);
BigNumbers_C_API bool isPrimeFasttestHex(const char* pBNHex);
BigNumbers_C_API bool isPrimeDec(const char* pBNDec);
BigNumbers_C_API bool isPrimeFasttestDec(const char* pBNDec);

#endif //ifndef __BIGNUM_API__
