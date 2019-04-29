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

#define UC(c)   ((unsigned char)c)
/*
  input : Given String
  output : true if the given string is found to be hexa decimal, else return false.
  description : if the string has a char that is not one of 0123456789abcdef or 0123456789ABCDEF, we return it as false.
                else, if the string starts with 0x or 0X or the string has one of char from the following char set -  abcdef or ABCDEF, 
		the return true. Return false in all other cases.

  */
bool IsHex(const std::string& val) 
{
    unsigned int pos = 0;
    bool notationFound = false;
    bool atofCharFound = false;

    if (val.length() == 0)
            return false;

    // if the given string starts with + -, Ignore the +/- char
    if (val[pos] == '-' || val[pos] == '+')
    {
        pos++;
    }
    
    // if the rest of string starts with 0x or 0X - mark it as notation(0x||0X) found
    if ((val.length() >= (pos + 2)) && val[pos] == '0' && (val[pos+1] == 'x' || val[pos+1] == 'X'))
    {
        pos = pos + 2;
        notationFound = true;
    }

    for (unsigned char ch : val.substr(pos))
    {
        if (ch >= UC('0') && ch <= UC('9'))
        {
            continue;
        }
        else if (( ch >= UC('a') && ch <= UC('f')) || ( ch >= UC('A') && ch <= UC('F')))
        {
            atofCharFound = true;
            continue;
        }
        else
        {
            return false;
        }
    }

    // if the rest of the string has all the chars are in between 0 to 9, retun as true if notation(ox||0X) is found else return false
    // if the rest of the string has any char in a..f or A..F, return as true irrespective of notation(ox||0X) being true or false
    if (notationFound || atofCharFound)
            return true;

    // return false
    return false; 
}

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

BIGNUM_RETURN_TYPE multiply(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    if (IsHex(argA))
    {
        BNValA.FromHex (argA);
    }
    else
    {
        BNValA.FromDec (argA);
    }

    if (IsHex(argB))
    {
        BNValB.FromHex (argB);
    }
    else
    {
        BNValB.FromDec (argB);
    }
    BigNumber Sum = BNValA * BNValB;
#ifdef __EMSCRIPTEN__
    return Sum.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(Sum.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE divide(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    if (IsHex(argA))
    {
        BNValA.FromHex (argA);
    }
    else
    {
        BNValA.FromDec (argA);
    }

    if (IsHex(argB))
    {
        BNValB.FromHex (argB);
    }
    else
    {
        BNValB.FromDec (argB);
    }
    BigNumber Sum = BNValA / BNValB;
#ifdef __EMSCRIPTEN__
    return Sum.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(Sum.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE leftShift(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    if (IsHex(argA))
    {
        BNValA.FromHex (argA);
    }
    else
    {
        BNValA.FromDec (argA);
    }

    if (IsHex(argB))
    {
        BNValB.FromHex (argB);
    }
    else
    {
        BNValB.FromDec (argB);
    }
    BigNumber res = BNValA << BNValB;
#ifdef __EMSCRIPTEN__
    return res.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset(strdup_func(res.ToDec().c_str()));
    return (std::move(returnPtr)); 
#endif    
}

BIGNUM_RETURN_TYPE rightShift(const char *argA, const char *argB)
{
    BigNumber BNValA, BNValB; 
    if (IsHex(argA))
    {
        BNValA.FromHex (argA);
    }
    else
    {
        BNValA.FromDec (argA);
    }

    if (IsHex(argB))
    {
        BNValB.FromHex (argB);
    }
    else
    {
        BNValB.FromDec (argB);
    }
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
