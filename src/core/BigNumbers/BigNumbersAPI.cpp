#include <sstream>
#include <random>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <BigNumbers/BigNumbersAPI.h>
#include <BigNumbers/BigNumbers.h>

// Windows only have _strdup while linux have strdup, the macro help to work it out cross plateform
#ifdef WIN32
#define strdup_func _strdup
#else
#define strdup_func strdup
#endif

#ifdef __EMSCRIPTEN__
    #define _HELP_RETURN_HEX(bnVar) return bnVar.ToHex().c_str();
    #define _HELP_RETURN_DEC(bnVar) return bnVar.ToDec().c_str();
#else
    #define _HELP_RETURN_HEX(bnVar)                          \
        std::unique_ptr<char> returnPtr;                     \
        returnPtr.reset(strdup_func(bnVar.ToHex().c_str())); \
        return (std::move(returnPtr));

    #define _HELP_RETURN_DEC(bnVar)                          \
        std::unique_ptr<char> returnPtr;                     \
        returnPtr.reset(strdup_func(bnVar.ToDec().c_str())); \
        return (std::move(returnPtr));
#endif


BIGNUM_RETURN_TYPE addFromHex (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber Sum = BNValA + BNValB;
    _HELP_RETURN_HEX(Sum);
}

BIGNUM_RETURN_TYPE addFromDec (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber Sum = BNValA + BNValB;
    _HELP_RETURN_DEC(Sum);
}

BIGNUM_RETURN_TYPE subFromHex (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber Diff = BNValA - BNValB;
    _HELP_RETURN_HEX(Diff);
}

BIGNUM_RETURN_TYPE subFromDec (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber Diff = BNValA - BNValB;
    _HELP_RETURN_DEC(Diff);
}

BIGNUM_RETURN_TYPE multiplyFromDec(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber res = BNValA * BNValB;
    _HELP_RETURN_DEC(res);
}

BIGNUM_RETURN_TYPE multiplyFromHex(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber res = BNValA * BNValB;
    _HELP_RETURN_HEX(res);
}

BIGNUM_RETURN_TYPE divideFromDec(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber res = BNValA / BNValB;
    _HELP_RETURN_DEC(res);
}

BIGNUM_RETURN_TYPE divideFromHex(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber res = BNValA / BNValB;
    _HELP_RETURN_HEX(res);
}


BIGNUM_RETURN_TYPE Mod_Hex (const char* pARG, const char* pMod)
{
    BigNumber BNValArg, BNValMod;
    BNValArg.FromHex(pARG);
    BNValMod.FromHex(pMod);
    BigNumber ret = BNValArg % BNValMod;
    _HELP_RETURN_HEX(ret);
}

BIGNUM_RETURN_TYPE Inv_mod_Hex (const char* pARG, const char* pMod)
{
    BigNumber BNValArg, BNValMod;
    BNValArg.FromHex(pARG);
    BNValMod.FromHex(pMod);
    BigNumber ret = Inv_mod(BNValArg, BNValMod);
    _HELP_RETURN_HEX(ret);
}

BIGNUM_RETURN_TYPE Add_mod_Hex (const char* pLHS, const char* pRHS, const char* pMod)
{
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromHex(pLHS);
    BNValRHS.FromHex(pRHS);
    BNValMOD.FromHex(pMod);
    BigNumber ret = Add_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_HEX(ret);
}

BIGNUM_RETURN_TYPE Sub_mod_Hex (const char* pLHS, const char* pRHS, const char* pMod)
{
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromHex(pLHS);
    BNValRHS.FromHex(pRHS);
    BNValMOD.FromHex(pMod);
    BigNumber ret = Sub_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_HEX(ret);
}

BIGNUM_RETURN_TYPE Mul_mod_Hex (const char* pLHS, const char* pRHS, const char* pMod)
{
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromHex(pLHS);
    BNValRHS.FromHex(pRHS);
    BNValMOD.FromHex(pMod);
    BigNumber ret = Mul_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_HEX(ret);
}

BIGNUM_RETURN_TYPE Div_mod_Hex (const char* pLHS, const char* pRHS, const char* pMod)
{
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromHex(pLHS);
    BNValRHS.FromHex(pRHS);
    BNValMOD.FromHex(pMod);
    BigNumber ret = Div_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_HEX(ret);
}

BIGNUM_RETURN_TYPE Mod_Dec (const char* pARG, const char* pMod)
{
    BigNumber BNValArg, BNValMod;
    BNValArg.FromDec(pARG);
    BNValMod.FromDec(pMod);
    BigNumber ret = BNValArg % BNValMod;
    _HELP_RETURN_DEC(ret);
}

BIGNUM_RETURN_TYPE Inv_mod_Dec (const char* pARG, const char* pMod)
{
    BigNumber BNValArg, BNValMod;
    BNValArg.FromDec(pARG);
    BNValMod.FromDec(pMod);
    BigNumber ret = Inv_mod(BNValArg, BNValMod);
    _HELP_RETURN_DEC(ret);
}

BIGNUM_RETURN_TYPE Add_mod_Dec (const char* pLHS, const char* pRHS, const char* pMod)
{
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromDec(pLHS);
    BNValRHS.FromDec(pRHS);
    BNValMOD.FromDec(pMod);
    BigNumber ret = Add_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_DEC(ret);
}

BIGNUM_RETURN_TYPE Sub_mod_Dec (const char* pLHS, const char* pRHS, const char* pMod)
{
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromDec(pLHS);
    BNValRHS.FromDec(pRHS);
    BNValMOD.FromDec(pMod);
    BigNumber ret = Sub_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_DEC(ret);
}

BIGNUM_RETURN_TYPE Mul_mod_Dec (const char* pLHS, const char* pRHS, const char* pMod)
{
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromDec(pLHS);
    BNValRHS.FromDec(pRHS);
    BNValMOD.FromDec(pMod);
    BigNumber ret = Mul_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_DEC(ret);
}

BIGNUM_RETURN_TYPE Div_mod_Dec (const char* pLHS, const char* pRHS, const char* pMod)
{
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromDec(pLHS);
    BNValRHS.FromDec(pRHS);
    BNValMOD.FromDec(pMod);
    BigNumber ret = Div_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_DEC(ret);
}


BIGNUM_RETURN_TYPE leftShiftFromDec(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber res = BNValA << BNValB;
    _HELP_RETURN_DEC(res);
}

BIGNUM_RETURN_TYPE leftShiftFromHex(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber res = BNValA << BNValB;
    _HELP_RETURN_HEX(res);
}

BIGNUM_RETURN_TYPE rightShiftFromDec(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber res = BNValA >> BNValB;
    _HELP_RETURN_DEC(res);
}

BIGNUM_RETURN_TYPE rightShiftFromHex(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber res = BNValA >> BNValB;
    _HELP_RETURN_HEX(res);
}


BIGNUM_RETURN_TYPE BNRandomHex (const int size)
{    
    BigNumber BNVal = GenerateRand (size); 
    _HELP_RETURN_HEX(BNVal);
}

BIGNUM_RETURN_TYPE BNRandomDec (const int size)
{
    BigNumber BNVal = GenerateRand (size); 
    _HELP_RETURN_DEC(BNVal);
}

BIGNUM_RETURN_TYPE BNRandomHexWithSeed(const char * seed, const int size)
{
    BigNumber BNVal;     
    BNVal.generateRandHexWithSeed(seed, size);
    _HELP_RETURN_HEX(BNVal);
}

BIGNUM_RETURN_TYPE BNRandomDecWithSeed(const char * seed, const int size)
{
    BigNumber BNVal;     
    BNVal.generateRandDecWithSeed(seed, size);
    _HELP_RETURN_DEC(BNVal);
}

BIGNUM_RETURN_TYPE BNRandomPrimeHex(const int size)
{
    BigNumber BNVal = GenerateRandPrime(size);
    _HELP_RETURN_HEX(BNVal);
}

BIGNUM_RETURN_TYPE BNRandomPrimeDec(const int size)
{
    BigNumber BNVal = GenerateRandPrime(size);
    _HELP_RETURN_DEC(BNVal);
}

BIGNUM_RETURN_TYPE BNRandomPrimeHexWithSeed(const char * seed, const int size)
{
    BigNumber BNVal;
    BNVal.generateRandPrimeHexWithSeed(seed, size);
    _HELP_RETURN_HEX(BNVal);
}

BIGNUM_RETURN_TYPE BNRandomPrimeDecWithSeed(const char * seed, const int size)
{
    BigNumber BNVal;
    BNVal.generateRandPrimeDecWithSeed(seed, size);
    _HELP_RETURN_DEC(BNVal);
}

bool isPrimeHex(const char* pBNHex)
{
    BigNumber primeBN;
    primeBN.FromHex(pBNHex);
    return primeBN.isPrime();
}

bool isPrimeFasttestHex(const char* pBNHex)
{
    BigNumber primeBN;
    primeBN.FromHex(pBNHex);
    return primeBN.isPrimeFasttest();
}

bool isPrimeDec(const char* pBNDec)
{
    BigNumber primeBN;
    primeBN.FromDec(pBNDec);
    return primeBN.isPrime();
}

bool isPrimeFasttestDec(const char* pBNDec)
{
    BigNumber primeBN;
    primeBN.FromDec(pBNDec);
    return primeBN.isPrimeFasttest();
}