#include <sstream>
#include <random>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "BigNumAPI.h"
#include "BigNumbers.h"

#ifdef WIN32
#define strdup_func _strdup
#else
#define strdup_func strdup
#endif

BIGNUM_RETURN_TYPE addFromHex (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber Sum = BNValA + BNValB;
#ifdef __EMSCRIPTEN__
    return Sum.ToHex().c_str(); 
#else    
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset (strdup_func(Sum.ToHex().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE addFromDec (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber Sum = BNValA + BNValB;
#ifdef __EMSCRIPTEN__
    return Sum.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset (strdup_func(Sum.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE subFromHex (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber Diff = BNValA - BNValB;
#ifdef __EMSCRIPTEN__
    return Diff.ToHex().c_str(); 
#else    
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset (strdup_func(Diff.ToHex().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE subFromDec (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber Diff = BNValA - BNValB;
#ifdef __EMSCRIPTEN__    
    return Diff.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset (strdup_func(Diff.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}


BIGNUM_RETURN_TYPE multiplyFromDec(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber res = BNValA * BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE multiplyFromHex(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber res = BNValA * BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE divideFromDec(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber res = BNValA / BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE divideFromHex(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber res = BNValA / BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE leftShiftFromDec(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber res = BNValA << BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE leftShiftFromHex(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber res = BNValA << BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE rightShiftFromDec(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber res = BNValA >> BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE rightShiftFromHex(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber res = BNValA >> BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE BNRandomHex (const int size)
{    
    BigNumber BNVal = GenerateRand (size); 
#ifdef __EMSCRIPTEN__
    return BNVal.ToHex().c_str();
#else
    std::unique_ptr<char> returnPtr;
    returnPtr.reset(strdup_func(BNVal.ToHex().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE BNRandomDec (const int size)
{
    BigNumber BNVal = GenerateRand (size); 
#ifdef __EMSCRIPTEN__
    return BNVal.ToDec().c_str();     
#else    
    std::unique_ptr<char> returnPtr;
    returnPtr.reset(strdup_func(BNVal.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE BNRandomHexWithSeed(const char * seed, const int size)
{
    BigNumber BNVal;     
    BNVal.generateRandHexWithSeed(seed, size);
#ifdef __EMSCRIPTEN__    
    return BNVal.ToHex().c_str();
#else
    std::unique_ptr<char> returnPtr;
    returnPtr.reset(strdup_func(BNVal.ToHex().c_str()));
    return (std::move(returnPtr));
#endif    
}

BIGNUM_RETURN_TYPE BNRandomDecWithSeed(const char * seed, const int size)
{
    BigNumber BNVal;     
    BNVal.generateRandDecWithSeed(seed, size);
#ifdef __EMSCRIPTEN__
    return BNVal.ToDec().c_str(); 
#else    
    std::unique_ptr<char> returnPtr;
    returnPtr.reset(strdup_func(BNVal.ToDec().c_str()));
    return (std::move(returnPtr));
#endif    
}
