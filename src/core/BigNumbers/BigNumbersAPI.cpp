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

    #define _HELP_RETURN_HEX_EX(bnVar)                 \
        std::vector<uint8_t> retVal;                    \
        std::string retValStr(bnVar.ToHex());           \
        for(std::string::const_iterator iter = retValStr.begin(); iter != retValStr.end(); ++iter){retVal.push_back(*iter);}\
        return (retVal);
    #define _HELP_RETURN_DEC_EX(bnVar)                 \
        std::vector<uint8_t> retVal;                    \
        std::string retValStr(bnVar.ToDec());           \
        for(std::string::const_iterator iter = retValStr.begin(); iter != retValStr.end(); ++iter){retVal.push_back(*iter);}\
        return (retVal);        

 std::string ConvertUint8ArrToStr(const std::vector<uint8_t>& arg){
     std::string strArgA;
     for(std::vector<uint8_t>::const_iterator iter = arg.begin(); iter != arg.end(); ++iter){
         strArgA.push_back(*iter);
     }
     return strArgA; 
 }

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> addFromHexEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb){

    std::string argAStr, argBStr; 
    for(std::vector<uint8_t>::const_iterator iter = arga.begin(); iter != arga.end(); ++iter){
        argAStr.push_back(*iter);
    }

    for(std::vector<uint8_t>::const_iterator iter = argb.begin(); iter != argb.end(); ++iter){
        argBStr.push_back(*iter);
    }

    BigNumber BNValA, BNValB; 
    BNValA.FromHex (argAStr);
    BNValB.FromHex (argBStr);
    BigNumber Sum = BNValA + BNValB;

    std::string result = Sum.ToHex();
    std::vector<uint8_t> retVal;
    for(std::string::const_iterator iter = result.begin(); iter != result.end(); ++ iter){
        retVal.push_back(*iter);
    }    
    return retVal;
}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> BNRandomHexEx(const int size){
    BigNumber BNVal = GenerateRand (size); 
    std::string bnHex = BNVal.ToHex(); 
    std::vector<uint8_t> retVal;
    for(std::string::iterator iter = bnHex.begin(); iter != bnHex.end(); ++iter){
        retVal.push_back(*iter);
    }    
    return retVal; 
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
std::vector<uint8_t> BNRandomDecEx (const int size){
    BigNumber BNVal = GenerateRand (size); 
    std::string bnHex = BNVal.ToDec(); 
    std::vector<uint8_t> retVal;
    for(std::string::iterator iter = bnHex.begin(); iter != bnHex.end(); ++iter){
        retVal.push_back(*iter);
    }    
    return retVal; 
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> BNRandomDecWithSeedEx(const std::vector<uint8_t>& seed, const int size){
    std::string seedVal; 
    for(std::vector<uint8_t>::const_iterator iter = seed.begin(); iter != seed.end(); ++iter){
        seedVal.push_back(*iter);
    }
    BigNumber BNVal;     
    BNVal.generateRandDecWithSeed(seedVal, size);
    std::vector<uint8_t> retval;
    std::string bnstr = BNVal.ToDec();
    for(std::string::iterator iter = bnstr.begin(); iter != bnstr.end(); ++iter){
        retval.push_back(*iter);
    }
    return retval;
}
#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> BNRandomHexWithSeedEx(const std::vector<uint8_t>& seed, const int size){
    std::string seedVal; 
    for(std::vector<uint8_t>::const_iterator iter = seed.begin(); iter != seed.end(); ++iter){
        seedVal.push_back(*iter);
    }
    BigNumber BNVal;     
    BNVal.generateRandHexWithSeed(seedVal, size);
    std::vector<uint8_t> retval;
    std::string bnstr = BNVal.ToDec();
    for(std::string::iterator iter = bnstr.begin(); iter != bnstr.end(); ++iter){
        retval.push_back(*iter);
    }
    return retval;
}
#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> BNRandomPrimeHexEx(const int size)
{
    BigNumber BNVal = GenerateRandPrime(size);    
    _HELP_RETURN_HEX_EX(BNVal);    
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t>  BNRandomPrimeDecEx(const int size)
{
    BigNumber BNVal = GenerateRandPrime(size);
    _HELP_RETURN_DEC_EX (BNVal);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> BNRandomPrimeHexWithSeedEx(const std::vector<uint8_t>& seed, const int size)
{
    std::string seedVal; 
    for(std::vector<uint8_t>::const_iterator iter = seed.begin(); iter != seed.end(); ++iter){
        seedVal.push_back(*iter);
    }
    BigNumber BNVal;
    BNVal.generateRandPrimeHexWithSeed(seedVal, size);    
    _HELP_RETURN_HEX_EX(BNVal);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> BNRandomPrimeDecWithSeedEx(const std::vector<uint8_t>& seed, const int size)
{
    std::string seedVal; 
    for(std::vector<uint8_t>::const_iterator iter = seed.begin(); iter != seed.end(); ++iter){
        seedVal.push_back(*iter);
    }
    BigNumber BNVal;
    BNVal.generateRandPrimeDecWithSeed(seedVal, size);
    _HELP_RETURN_DEC_EX(BNVal);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
bool isPrimeHexEx(const std::vector<uint8_t>&  BNHex)
{
    std::string seedVal; 
    for(std::vector<uint8_t>::const_iterator iter = BNHex.begin(); iter != BNHex.end(); ++iter){
        seedVal.push_back(*iter);
    }
    BigNumber primeBN;
    primeBN.FromHex(seedVal);
    return primeBN.isPrime();
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
bool isPrimeFasttestHexEx(const std::vector<uint8_t>& BNHex)
{
    std::string seedVal; 
    for(std::vector<uint8_t>::const_iterator iter = BNHex.begin(); iter != BNHex.end(); ++iter){
        seedVal.push_back(*iter);
    }    
    BigNumber primeBN;
    primeBN.FromHex(seedVal);
    return primeBN.isPrimeFasttest();
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
bool isPrimeDecEx(const std::vector<uint8_t>& BNDec)
{
    std::string seedVal; 
    for(std::vector<uint8_t>::const_iterator iter = BNDec.begin(); iter != BNDec.end(); ++iter){
        seedVal.push_back(*iter);
    } 
    BigNumber primeBN;
    primeBN.FromDec(seedVal);
    return primeBN.isPrime();
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
bool isPrimeFasttestDecEx(const std::vector<uint8_t>& BNDec)
{
    std::string seedVal; 
    for(std::vector<uint8_t>::const_iterator iter = BNDec.begin(); iter != BNDec.end(); ++iter){
        seedVal.push_back(*iter);
    }
    BigNumber primeBN;
    primeBN.FromDec(seedVal);
    return primeBN.isPrimeFasttest();
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> leftShiftFromHexEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb){
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb); 
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (strArgA);
    BNValB.FromHex (strArgB);
    BigNumber res = BNValA << BNValB;
    _HELP_RETURN_HEX_EX(res);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> leftShiftFromDecEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb){
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string atrArgB = ConvertUint8ArrToStr(argb); 
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (strArgA);
    BNValB.FromDec (atrArgB);
    BigNumber res = BNValA << BNValB;
    _HELP_RETURN_DEC_EX(res);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> rightShiftFromHexEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb){
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb); 
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (strArgA);
    BNValB.FromHex (strArgB);
    BigNumber res = BNValA >> BNValB;
    _HELP_RETURN_HEX_EX(res);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> rightShiftFromDecEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb){
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string atrArgB = ConvertUint8ArrToStr(argb); 
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (strArgA);
    BNValB.FromDec (atrArgB);
    BigNumber res = BNValA >> BNValB;
    _HELP_RETURN_DEC_EX(res);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif  
std::vector<uint8_t> addFromDecEx (const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb)
{
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb); 
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (strArgA);
    BNValB.FromDec (strArgA);
    BigNumber Sum = BNValA + BNValB;
    _HELP_RETURN_DEC_EX(Sum);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> subFromHexEx (const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb)
{
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb);
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (strArgA);
    BNValB.FromHex (strArgB);
    BigNumber Diff = BNValA - BNValB;
    _HELP_RETURN_HEX_EX(Diff);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> subFromDecEx (const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb)
{
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb);
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (strArgA);
    BNValB.FromDec (strArgB);
    BigNumber Diff = BNValA - BNValB;
    _HELP_RETURN_DEC_EX(Diff);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> multiplyFromDecEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb)
{
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb);
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (strArgA);
    BNValB.FromDec (strArgB);
    BigNumber res = BNValA * BNValB;
    _HELP_RETURN_DEC_EX(res);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif 
std::vector<uint8_t> multiplyFromHexEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb)
{
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb);    
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (strArgA);
    BNValB.FromHex (strArgB);
    BigNumber res = BNValA * BNValB;
    _HELP_RETURN_HEX_EX(res);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> divideFromDecEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb)
{
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb);  
    BigNumber BNValA, BNValB; 
    BNValA.FromDec (strArgA);
    BNValB.FromDec (strArgB);
    BigNumber res = BNValA / BNValB;
    _HELP_RETURN_DEC_EX(res);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> divideFromHexEx(const std::vector<uint8_t>& arga, const std::vector<uint8_t>& argb)
{
    std::string strArgA = ConvertUint8ArrToStr(arga);
    std::string strArgB = ConvertUint8ArrToStr(argb);    
    BigNumber BNValA, BNValB; 
    BNValA.FromHex (strArgA);
    BNValB.FromHex (strArgB);
    BigNumber res = BNValA / BNValB;
    _HELP_RETURN_HEX_EX(res);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Mod_HexEx (const std::vector<uint8_t>& ARG, const std::vector<uint8_t>& Mod)
{
    std::string strArg = ConvertUint8ArrToStr(ARG);
    std::string strMod = ConvertUint8ArrToStr(Mod);    
    BigNumber BNValArg, BNValMod;
    BNValArg.FromHex(strArg);
    BNValMod.FromHex(strMod);
    BigNumber ret = BNValArg % BNValMod;
    _HELP_RETURN_HEX_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Inv_mod_HexEx (const std::vector<uint8_t>& ARG, const std::vector<uint8_t>& Mod)
{
    std::string strArg = ConvertUint8ArrToStr(ARG);
    std::string strMod = ConvertUint8ArrToStr(Mod);    
    BigNumber BNValArg, BNValMod;
    BNValArg.FromHex(strArg);
    BNValMod.FromHex(strMod);
    BigNumber ret = Inv_mod(BNValArg, BNValMod);
    _HELP_RETURN_HEX_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Add_mod_HexEx (const std::vector<uint8_t>& LHS, const std::vector<uint8_t>& RHS, const std::vector<uint8_t>& Mod)
{
    std::string strLHS = ConvertUint8ArrToStr(LHS);
    std::string strRHS = ConvertUint8ArrToStr(RHS);
    std::string strMod = ConvertUint8ArrToStr(Mod);  
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromHex(strLHS);
    BNValRHS.FromHex(strRHS);
    BNValMOD.FromHex(strMod);
    BigNumber ret = Add_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_HEX_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Sub_mod_HexEx (const std::vector<uint8_t>& LHS, const std::vector<uint8_t>& RHS, const std::vector<uint8_t>& Mod)
{
    std::string strLHS = ConvertUint8ArrToStr(LHS);
    std::string strRHS = ConvertUint8ArrToStr(RHS);
    std::string strMod = ConvertUint8ArrToStr(Mod);     
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromHex(strLHS);
    BNValRHS.FromHex(strRHS);
    BNValMOD.FromHex(strMod);
    BigNumber ret = Sub_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_HEX_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Mul_mod_HexEx (const std::vector<uint8_t>& LHS, const std::vector<uint8_t>& RHS, const std::vector<uint8_t>& Mod)
{
    std::string strLHS = ConvertUint8ArrToStr(LHS);
    std::string strRHS = ConvertUint8ArrToStr(RHS);
    std::string strMod = ConvertUint8ArrToStr(Mod);     
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromHex(strLHS);
    BNValRHS.FromHex(strRHS);
    BNValMOD.FromHex(strMod);
    BigNumber ret = Mul_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_HEX_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Div_mod_HexEx (const std::vector<uint8_t>& LHS, const std::vector<uint8_t>& RHS, const std::vector<uint8_t>& Mod)
{
    std::string strLHS = ConvertUint8ArrToStr(LHS);
    std::string strRHS = ConvertUint8ArrToStr(RHS);
    std::string strMod = ConvertUint8ArrToStr(Mod);    
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromHex(strLHS);
    BNValRHS.FromHex(strRHS);
    BNValMOD.FromHex(strMod);
    BigNumber ret = Div_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_HEX_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Mod_DecEx (const std::vector<uint8_t>& ARG, const std::vector<uint8_t>& Mod)
{
    std::string strARG = ConvertUint8ArrToStr(ARG);
    std::string strMod = ConvertUint8ArrToStr(Mod);     
    BigNumber BNValArg, BNValMod;
    BNValArg.FromDec(strARG);
    BNValMod.FromDec(strMod);
    BigNumber ret = BNValArg % BNValMod;
    _HELP_RETURN_DEC_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Inv_mod_DecEx (const std::vector<uint8_t>& ARG, const std::vector<uint8_t>& Mod)
{
    std::string strARG = ConvertUint8ArrToStr(ARG);
    std::string strMod = ConvertUint8ArrToStr(Mod);     
    BigNumber BNValArg, BNValMod;
    BNValArg.FromDec(strARG);
    BNValMod.FromDec(strMod);
    BigNumber ret = Inv_mod(BNValArg, BNValMod);
    _HELP_RETURN_DEC_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Add_mod_DecEx (const std::vector<uint8_t>& LHS, const std::vector<uint8_t>& RHS, const std::vector<uint8_t>& Mod)
{
    std::string strLHS = ConvertUint8ArrToStr(LHS);
    std::string strRHS = ConvertUint8ArrToStr(RHS);
    std::string strMod = ConvertUint8ArrToStr(Mod);        
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromDec(strLHS);
    BNValRHS.FromDec(strRHS);
    BNValMOD.FromDec(strMod);
    BigNumber ret = Add_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_DEC_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Sub_mod_DecEx (const std::vector<uint8_t>& LHS, const std::vector<uint8_t>& RHS, const std::vector<uint8_t>& Mod)
{
    std::string strLHS = ConvertUint8ArrToStr(LHS);
    std::string strRHS = ConvertUint8ArrToStr(RHS);
    std::string strMod = ConvertUint8ArrToStr(Mod);    
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromDec(strLHS);
    BNValRHS.FromDec(strRHS);
    BNValMOD.FromDec(strMod);
    BigNumber ret = Sub_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_DEC_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Mul_mod_DecEx (const std::vector<uint8_t>& LHS, const std::vector<uint8_t>& RHS, const std::vector<uint8_t>& Mod)
{
    std::string strLHS = ConvertUint8ArrToStr(LHS);
    std::string strRHS = ConvertUint8ArrToStr(RHS);
    std::string strMod = ConvertUint8ArrToStr(Mod);    
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromDec(strLHS);
    BNValRHS.FromDec(strRHS);
    BNValMOD.FromDec(strMod);
    BigNumber ret = Mul_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_DEC_EX(ret);
}

#ifdef  __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE 
#endif
std::vector<uint8_t> Div_mod_DecEx (const std::vector<uint8_t>& LHS, const std::vector<uint8_t>& RHS, const std::vector<uint8_t>& Mod)
{
    std::string strLHS = ConvertUint8ArrToStr(LHS);
    std::string strRHS = ConvertUint8ArrToStr(RHS);
    std::string strMod = ConvertUint8ArrToStr(Mod);      
    BigNumber BNValLHS, BNValRHS, BNValMOD;
    BNValLHS.FromDec(strLHS);
    BNValRHS.FromDec(strRHS);
    BNValMOD.FromDec(strMod);
    BigNumber ret = Div_mod(BNValLHS, BNValRHS, BNValMOD);
    _HELP_RETURN_DEC_EX(ret);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(module_bignumbers) {
    emscripten::function("BNRandomHexEx", &BNRandomHexEx);  
    emscripten::function("BNRandomDecEx", &BNRandomDecEx); 
    emscripten::function("BNRandomHexWithSeedEx", &BNRandomHexWithSeedEx); 
    emscripten::function("BNRandomDecWithSeedEx", &BNRandomDecWithSeedEx); 
    emscripten::function("BNRandomPrimeHexEx",&BNRandomPrimeHexEx); 
    emscripten::function("BNRandomPrimeDecEx",&BNRandomPrimeDecEx);
    emscripten::function("BNRandomPrimeHexWithSeedEx",&BNRandomPrimeHexWithSeedEx);
    emscripten::function("BNRandomPrimeDecWithSeedEx",&BNRandomPrimeDecWithSeedEx);
    emscripten::function("isPrimeHexEx",&isPrimeHexEx); 
    emscripten::function("isPrimeFasttestHexEx",&isPrimeFasttestHexEx); 
    emscripten::function("isPrimeDecEx",&isPrimeDecEx); 
    emscripten::function("isPrimeFasttestDecEx",&isPrimeFasttestDecEx); 
    emscripten::function("leftShiftFromHexEx",&leftShiftFromHexEx);
    emscripten::function("leftShiftFromDecEx",&leftShiftFromDecEx); 
    emscripten::function("rightShiftFromHexEx",&rightShiftFromHexEx); 
    emscripten::function("rightShiftFromDecEx",&rightShiftFromDecEx); 
    emscripten::function("addFromHexEx", &addFromHexEx);  

    emscripten::function("addFromDecEx",&addFromDecEx);
    emscripten::function("subFromHexEx",&subFromHexEx); 
    emscripten::function("subFromDecEx",&subFromDecEx);
    emscripten::function("multiplyFromHexEx",&multiplyFromHexEx); 
    emscripten::function("multiplyFromDecEx",&multiplyFromDecEx);
    emscripten::function("divideFromHexEx",&divideFromHexEx);
    emscripten::function("divideFromDecEx",&divideFromDecEx); 

    emscripten::function("Mod_HexEx",&Mod_HexEx); 
    emscripten::function("Inv_mod_HexEx",&Inv_mod_HexEx); 
    emscripten::function("Add_mod_HexEx",&Add_mod_HexEx); 
    emscripten::function("Sub_mod_HexEx",&Sub_mod_HexEx);
    emscripten::function("Mul_mod_HexEx",&Mul_mod_HexEx);
    emscripten::function("Div_mod_HexEx",&Div_mod_HexEx);
    emscripten::function("Mod_DecEx",&Mod_DecEx); 
    emscripten::function("Inv_mod_DecEx",&Inv_mod_DecEx);
    emscripten::function("Add_mod_DecEx",&Add_mod_DecEx);
    emscripten::function("Sub_mod_DecEx",&Sub_mod_DecEx); 
    emscripten::function("Mul_mod_DecEx",&Mul_mod_DecEx);
    emscripten::function("Div_mod_DecEx",&Div_mod_DecEx);
}
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