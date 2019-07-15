#ifndef __BIGNUMBERS_API__
#define __BIGNUMBERS_API__

#include <BigNumbers/BigNumbers.h>
#include <vector>
#include <string>
#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#define BIGNUM_RETURN_TYPE EMSCRIPTEN_KEEPALIVE const char *
#define BigNumbers_C_API extern "C" BigNumbers_API


EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> addFromHexEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&); 
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> addFromDecEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> subFromHexEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> subFromDecEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> multiplyFromHexEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> multiplyFromDecEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> divideFromHexEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> divideFromDecEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);

EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> BNRandomHexEx(const int); 
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> BNRandomDecEx (const int); 
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> BNRandomDecWithSeedEx(const std::vector<uint8_t>&, const int);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> BNRandomHexWithSeedEx(const std::vector<uint8_t>&, const int);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> BNRandomPrimeHexEx(const int);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> BNRandomPrimeDecEx(const int);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> BNRandomPrimeHexWithSeedEx(const std::vector<uint8_t>&, const int);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> BNRandomPrimeDecWithSeedEx(const std::vector<uint8_t>&, const int);
EMSCRIPTEN_KEEPALIVE bool isPrimeHexEx(const std::vector<uint8_t>&); 
EMSCRIPTEN_KEEPALIVE bool isPrimeFasttestHexEx(const std::vector<uint8_t>&); 
EMSCRIPTEN_KEEPALIVE bool isPrimeDecEx(const std::vector<uint8_t>&); 
EMSCRIPTEN_KEEPALIVE bool isPrimeFasttestDecEx(const std::vector<uint8_t>&); 
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> leftShiftFromHexEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> leftShiftFromDecEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> rightShiftFromHexEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> rightShiftFromDecEx(const std::vector<uint8_t>&, const std::vector<uint8_t>&);

EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Mod_HexEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Inv_mod_HexEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Add_mod_HexEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Sub_mod_HexEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Mul_mod_HexEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Div_mod_HexEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Mod_DecEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Inv_mod_DecEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Add_mod_DecEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Sub_mod_DecEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Mul_mod_DecEx (const std::vector<uint8_t>&, const std::vector<uint8_t>&, const std::vector<uint8_t>&);
EMSCRIPTEN_KEEPALIVE std::vector<uint8_t> Div_mod_DecEx  (const std::vector<uint8_t>&, const std::vector<uint8_t>&, const std::vector<uint8_t>&);

#else
#include <memory>
    struct CD
    {
        void operator() (char *ptr) {
		std::free(ptr);
	}
    };
#define BIGNUM_RETURN_TYPE std::unique_ptr<char, CD>
#define BigNumbers_C_API BigNumbers_API
#endif

BigNumbers_C_API BIGNUM_RETURN_TYPE addFromHex(char*, char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE addFromDec(char *, char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE subFromHex (char*, char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE subFromDec(char*, char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE multiplyFromHex(const char*, const char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE multiplyFromDec(const char*, const char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE divideFromHex(const char*, const char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE divideFromDec(const char*, const char*);

BigNumbers_C_API BIGNUM_RETURN_TYPE Mod_Hex (const char* pARG, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Inv_mod_Hex (const char* pARG, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Add_mod_Hex (const char* pLHS, const char* pRHS, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Sub_mod_Hex (const char* pLHS, const char* pRHS, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Mul_mod_Hex (const char* pLHS, const char* pRHS, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Div_mod_Hex (const char* pLHS, const char* pRHS, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Mod_Dec (const char* pARG, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Inv_mod_Dec (const char* pARG, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Add_mod_Dec (const char* pLHS, const char* pRHS, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Sub_mod_Dec (const char* pLHS, const char* pRHS, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Mul_mod_Dec (const char* pLHS, const char* pRHS, const char* pMod);
BigNumbers_C_API BIGNUM_RETURN_TYPE Div_mod_Dec (const char* pLHS, const char* pRHS, const char* pMod);

BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomHex(const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomDec(const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomDecWithSeed(const char * seed, const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomHexWithSeed(const char *, const int size );

BigNumbers_C_API BIGNUM_RETURN_TYPE leftShiftFromHex(const char*, const char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE leftShiftFromDec(const char*, const char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE rightShiftFromHex(const char*, const char*);
BigNumbers_C_API BIGNUM_RETURN_TYPE rightShiftFromDec(const char*, const char*);

BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomPrimeHex(const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomPrimeDec(const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomPrimeHexWithSeed(const char * seed, const int size);
BigNumbers_C_API BIGNUM_RETURN_TYPE BNRandomPrimeDecWithSeed(const char * seed, const int size);

BigNumbers_C_API bool isPrimeHex(const char* pBNHex);
BigNumbers_C_API bool isPrimeFasttestHex(const char* pBNHex);
BigNumbers_C_API bool isPrimeDec(const char* pBNDec);
BigNumbers_C_API bool isPrimeFasttestDec(const char* pBNDec);

#endif //ifndef __BIGNUMBERS_API__
