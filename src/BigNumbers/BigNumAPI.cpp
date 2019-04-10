#include <sstream>
#include <random>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "BigNumAPI.h"
#include "BigNumbers.h"

extern "C" {

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
    const char * 
#else
    std::unique_ptr<char> 
#endif
addFromHex (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber Sum = BNValA + BNValB;
#ifdef __EMSCRIPTEN__
    return Sum.ToHex().c_str(); 
#else    
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset (strndup (Sum.ToHex().c_str(),Sum.ToHex().size()));
    return (std::move(returnPtr)); 
#endif    
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE const char *
#else
std::unique_ptr<char> 
#endif
addFromDec (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber Sum = BNValA + BNValB;
#ifdef __EMSCRIPTEN__
    return Sum.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset (strndup (Sum.ToDec().c_str(),Sum.ToDec().size()));
    return (std::move(returnPtr)); 
#endif    
}

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE
    const char * 
#else
    std::unique_ptr<char> 
#endif
subFromHex (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argA);
    BNValB.FromHex (argB);
    BigNumber Diff = BNValA - BNValB;
#ifdef __EMSCRIPTEN__
    return Diff.ToHex().c_str(); 
#else    
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset (strndup (Diff.ToHex().c_str(),Diff.ToHex().size()));
    return (std::move(returnPtr)); 
#endif    
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE const char *
#else
std::unique_ptr<char> 
#endif
subFromDec (char * argA, char * argB)
{
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (argA);
    BNValB.FromDec (argB);
    BigNumber Diff = BNValA - BNValB;
#ifdef __EMSCRIPTEN__    
    return Diff.ToDec().c_str(); 
#else        
    std::unique_ptr<char> returnPtr ; 
    returnPtr.reset (strndup (Diff.ToDec().c_str(),Diff.ToDec().size()));
    return (std::move(returnPtr)); 
#endif    
}

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE const char *
#else
std::unique_ptr<char>
#endif   
BNRandomHex (const int size)
{    
    BigNumber BNVal = GenerateRand (size); 
#ifdef __EMSCRIPTEN__
    return BNVal.ToHex().c_str();
#else
    std::unique_ptr<char> returnPtr;
    returnPtr.reset(strndup(BNVal.ToHex().c_str(),BNVal.ToHex().size()));
    return (std::move(returnPtr)); 
#endif    
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE const char *
#else
std::unique_ptr<char>  
#endif 
BNRandomDec (const int size)
{
    BigNumber BNVal = GenerateRand (size); 
#ifdef __EMSCRIPTEN__
    return BNVal.ToDec().c_str();     
#else    
    std::unique_ptr<char> returnPtr;
    returnPtr.reset(strndup(BNVal.ToDec().c_str(),BNVal.ToDec().size()));
    return (std::move(returnPtr)); 
#endif    
}

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE const char *
#else
    std::unique_ptr<char>
#endif
    BNRandomHexWithSeed(const char * seed, const int size)
{
    BigNumber BNVal;     
    BNVal.generateRandHexWithSeed(seed, size);
#ifdef __EMSCRIPTEN__    
    return BNVal.ToHex().c_str();
#else
    std::unique_ptr<char> returnPtr;
    returnPtr.reset(strndup(BNVal.ToHex().c_str(),BNVal.ToHex().size()));
    return (std::move(returnPtr));
#endif    
}

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_KEEPALIVE const char *
#else
    std::unique_ptr<char>
#endif
    BNRandomDecWithSeed(const char * seed, const int size)
{
    BigNumber BNVal;     
    BNVal.generateRandDecWithSeed(seed, size);
#ifdef __EMSCRIPTEN__
    return BNVal.ToDec().c_str(); 
#else    
    std::unique_ptr<char> returnPtr;
    returnPtr.reset(strndup(BNVal.ToDec().c_str(),BNVal.ToDec().size()));
    return (std::move(returnPtr));
#endif    
}
};

