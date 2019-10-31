#include <BigNumbers/BigNumbers.h>
#include <BigNumbers/BigNumbersImpl.h>

#include <iostream>
#include <sstream>
#include <stdlib.h>

#include <cassert>

BigNumber::BigNumber() : m_pImpl(new BigNumberImpl){}

BigNumber::~BigNumber()=default;

BigNumber::BigNumber(BigNumber&& obj) noexcept = default;
BigNumber& BigNumber::operator=(BigNumber&& obj) noexcept = default;

BigNumber::BigNumber(const BigNumber& obj)
    : m_pImpl(new BigNumberImpl (*obj.m_pImpl))
{
    return ; 
}

BigNumber& BigNumber::operator=(const BigNumber& obj)
{
    if (this != &obj)
    {
        m_pImpl.reset( new BigNumberImpl (*obj.m_pImpl));
    }
    return *this;
}

void BigNumber::One(){
    this->pImpl()->One () ; 
}

void BigNumber::Zero(){
    this->pImpl()->Zero();
}   

void BigNumber::Negative(){
    this->pImpl()->Negative();
}

BigNumber BigNumber::operator++ (int)
{
    BigNumber num(*this);
    ++ (*this); 
    return num ; 
    
}
    
BigNumber& BigNumber::operator++ ()
{
    ++ *(m_pImpl.get()); 
    return *this ;
}

BigNumber BigNumber::operator-- (const int){
    BigNumber bnVal(*this);
    -- (*this);
    return bnVal;
}

BigNumber& BigNumber::operator-- ()
{
    -- *(m_pImpl.get());
    return (*this);
}

std::string BigNumber::ToHex () const {
    return (this->pImpl()->ToHex ());
}

std::string BigNumber::ToDec () const{
    return (this->pImpl ()->ToDec());
}

int BigNumber::FromHex (const std::string& val){
    return(this->pImpl()->FromHex(val));
}

int BigNumber::FromDec (const std::string& val)
{
    return(this->pImpl()->FromDec(val));
}

int BigNumber::FromBin (unsigned char *val, int size)
{
    return(this->pImpl()->FromBin(val, size));
}

int BigNumber::FromBin (std::vector<uint8_t>& val)
{
    return(this->pImpl()->FromBin(val));
}

std::vector<uint8_t> BigNumber::ToBin () const
{
    return(this->pImpl()->ToBin());
}

std::string BigNumber::generateRandHex(const int& nsize){
    this->pImpl()->generate (nsize) ; 
    return (this->pImpl()->ToHex ());
} 

std::string BigNumber::generateRandDec(const int& nsize){
    this->pImpl()->generate(nsize); 
    return (this->pImpl()->ToDec());
}

std::string BigNumber::generateNegRandHex (const int& nsize){
    this->pImpl()->generateNeg(nsize);
    return (this->pImpl()->ToHex());
}

std::string BigNumber::generateNegRandDec (const int& nsize){
    this->pImpl()->generateNeg(nsize);
    return (this->pImpl()->ToDec());
}

std::string BigNumber::generateRandRange (const BigNumber& max){       
    this->pImpl()->generateRange (max.pImpl());
    return ToHex(); 
}


void BigNumber::seedRNG (const std::string& seed) {
    this->pImpl()->seedRNG (seed);
}

std::string BigNumber::generateRandHexWithSeed(const std::string& seed, const int& nsize){
    this->pImpl()->seedRNG(seed);
    return generateRandHex(nsize);
}

std::string BigNumber::generateRandDecWithSeed(const std::string& seed, const int& nsize){
    this->pImpl()->seedRNG(seed);    
    return generateRandDec (nsize);
}

std::string BigNumber::generateNegRandHexWithSeed (const std::string& seed, const int& nsize){
    this->pImpl()->seedRNG(seed);    
    return generateNegRandHex(nsize);
}

std::string BigNumber::generateNegRandDecWithSeed (const std::string& seed, const int& nsize){
    this->pImpl()->seedRNG(seed);
    return generateNegRandDec(nsize);
}

std::string BigNumber::generateRangRandHexWithSeed (const std::string& seed, const BigNumber& upperLimit){
    this->pImpl()->seedRNG(seed);
    generateRandRange(upperLimit);
    return ToHex(); 
}

std::string BigNumber::generateRangRandDecWithSeed (const std::string& seed, const BigNumber& upperLimit){
    this->pImpl()->seedRNG(seed);
    generateRandRange(upperLimit);
    return ToHex(); 
}

// Generate random prime & return string Representation
std::string BigNumber::generateRandPrimeHex(const int& nsize)
{
    this->pImpl()->generatePrime(nsize);
    return (this->pImpl()->ToHex());
}

std::string BigNumber::generateRandPrimeDec(const int& nsize)
{
    this->pImpl()->generatePrime(nsize);
    return (this->pImpl()->ToDec());
}

std::string BigNumber::generateRandPrimeHexWithSeed(const std::string& seed, const int& nsize)
{
    this->pImpl()->seedRNG(seed);
    return generateRandPrimeHex(nsize);
}

std::string BigNumber::generateRandPrimeDecWithSeed(const std::string& seed, const int& nsize)
{
    this->pImpl()->seedRNG(seed);
    return generateRandPrimeDec(nsize);
}

bool BigNumber::isPrime() const
{
    return this->pImpl()->isPrime();
}

bool BigNumber::isPrimeFasttest() const
{
    return this->pImpl()->isPrimeFasttest();
}

// friend free functions
BigNumber operator+ (const BigNumber& obj1, const BigNumber& obj2) 
{
    std::unique_ptr<BigNumberImpl> res1 = Add(obj1.pImpl(),obj2.pImpl());
    BigNumber res ; 
    res.m_pImpl.reset (new BigNumberImpl (*res1));
    return res; 
}

BigNumber operator+ ( const BigNumber& obj1, const int& nVal)
{
    // does this work for negative numbers?
    BigNumber obj2; 
    std::stringstream numStr ; 
    numStr << nVal ; 
    obj2.FromDec (numStr.str());
    std::unique_ptr<BigNumberImpl> resImpl = Add (obj1.pImpl(), obj2.pImpl());
    BigNumber res; 
    res.m_pImpl.reset (new BigNumberImpl(*resImpl));
    return res;
}

BigNumber operator- (const BigNumber& obj1, const BigNumber& obj2)
{
    std::unique_ptr<BigNumberImpl> res = Sub(obj1.pImpl(), obj2.pImpl());
    BigNumber bnRes; 
    bnRes.m_pImpl.reset (new BigNumberImpl(*res));
    return bnRes; 
}

BigNumber operator- (const BigNumber& obj1, const int& val)
{
     // does this work for negative numbers?
    BigNumber obj2; 
    std::stringstream numStr ; 
    numStr << val ; 
    obj2.FromDec (numStr.str());
    std::unique_ptr<BigNumberImpl> resImpl = Sub (obj1.pImpl(), obj2.pImpl());
    BigNumber res; 
    res.m_pImpl.reset (new BigNumberImpl(*resImpl));
    return res; 
}

BigNumber operator* (const BigNumber& obj1, const BigNumber& obj2)
{
    std::unique_ptr<BigNumberImpl> res = Mul(obj1.pImpl(), obj2.pImpl());
    BigNumber bnMul; 
    bnMul.m_pImpl.reset(new BigNumberImpl(*res));
    return bnMul; 
}

BigNumber operator/ (const BigNumber& obj1, const BigNumber& obj2)
{
    if (obj2.ToDec() == "0")
    {
        throw  std::runtime_error("Divide by zero exception");
    }
    std::unique_ptr<BigNumberImpl> res = Div(obj1.pImpl(), obj2.pImpl());
    BigNumber bnDiv; 
    bnDiv.m_pImpl.reset(new BigNumberImpl(*res));
    return bnDiv; 
}

BigNumber operator% (const BigNumber& obj1, const BigNumber& obj2)
{
    std::unique_ptr<BigNumberImpl> res = Mod(obj1.pImpl(), obj2.pImpl());
    BigNumber bnMod; 
    bnMod.m_pImpl.reset(new BigNumberImpl(*res));
    return bnMod; 
}

bool operator> (const BigNumber& obj1, const BigNumber& obj2)
{
    return CMPGreater(obj1.m_pImpl.get(), obj2.m_pImpl.get()); 
}

bool operator< (const BigNumber& obj1, const BigNumber& obj2)
{
    return CMPLess (obj1.m_pImpl.get(), obj2.m_pImpl.get());
}

bool operator== (const BigNumber& obj1,  const BigNumber& obj2)
{
    return CMPEqual (obj1.m_pImpl.get(), obj2.m_pImpl.get());
}

BigNumber operator>> (const BigNumber& obj1, const BigNumber& obj2)
{
    if (obj2.ToDec().length() > 0 && obj2.ToDec()[0] == '-')
    {
        throw  std::runtime_error("negative shift count");
    }

    BigNumber _obj2 = obj2, _obj1 = obj1; 
    BigNumber intValBn; 
    const int intVal = 2147483647;
    std::stringstream numStr ; 
    numStr << intVal ; 
    intValBn.FromDec(numStr.str());

    while(_obj2 > intValBn)
    {
        _obj2 = _obj2 - intValBn;   
        _obj1 = _obj1 >> intVal;

    }

    _obj1 = _obj1 >> std::stoi(_obj2.ToDec());
    return _obj1;
}

BigNumber operator>> (const BigNumber& obj, const int& val)
{

    if (val < 0)
    {
        throw  std::runtime_error("negative shift count");
    }

    BigNumber obj2; 
    std::unique_ptr<BigNumberImpl> resImpl = RShift (obj.pImpl(), val);
    BigNumber res; 
    res.m_pImpl.reset (new BigNumberImpl(*resImpl));
    return res; 
}

BigNumber operator<< (const BigNumber& obj1, const BigNumber& obj2)
{
    if (obj2.ToDec().length() > 0 && obj2.ToDec()[0] == '-')
    {
        throw  std::runtime_error("negative shift count");
    }

    BigNumber _obj2 = obj2, _obj1 = obj1; 
    BigNumber intValBn; 
    const int intVal = 2147483647;
    std::stringstream numStr ; 
    numStr << intVal; 
    intValBn.FromDec(numStr.str());

    while(_obj2 > intValBn)
    {
        _obj2 = _obj2 - intValBn;   
        _obj1 = _obj1 << intVal;
    }
    _obj1 = _obj1 << std::stoi(_obj2.ToDec());
    return _obj1;

}

BigNumber operator<< (const BigNumber& obj, const int& val)
{
    if (val < 0)
    {
        throw  std::runtime_error("negative shift count");
    }
    BigNumber obj2; 
    std::unique_ptr<BigNumberImpl> resImpl = LShift (obj.pImpl(), val);
    BigNumber res; 
    res.m_pImpl.reset (new BigNumberImpl(*resImpl));
    return res; 
}


BigNumber Inv_mod (const BigNumber& crARG, const BigNumber& crMOD)
{
    std::unique_ptr<BigNumberImpl> impl_ret = Inv_mod(crARG.pImpl(),crMOD.pImpl());
    BigNumber res;
    res.m_pImpl.reset (new BigNumberImpl (*impl_ret));
    return res;
}

BigNumber Add_mod (const BigNumber& crLHS, const BigNumber& crRHS, const BigNumber& crMOD)
{
    std::unique_ptr<BigNumberImpl> impl_ret = Add_mod(crLHS.pImpl(), crRHS.pImpl(),crMOD.pImpl());
    BigNumber res;
    res.m_pImpl.reset (new BigNumberImpl (*impl_ret));
    return res;
}

BigNumber Sub_mod (const BigNumber& crLHS, const BigNumber& crRHS, const BigNumber& crMOD)
{
    std::unique_ptr<BigNumberImpl> impl_ret = Sub_mod(crLHS.pImpl(), crRHS.pImpl(),crMOD.pImpl());
    BigNumber res;
    res.m_pImpl.reset (new BigNumberImpl (*impl_ret));
    return res;
}

BigNumber Mul_mod (const BigNumber& crLHS, const BigNumber& crRHS, const BigNumber& crMOD)
{
    std::unique_ptr<BigNumberImpl> impl_ret = Mul_mod(crLHS.pImpl(), crRHS.pImpl(),crMOD.pImpl());
    BigNumber res;
    res.m_pImpl.reset (new BigNumberImpl (*impl_ret));
    return res;
}

BigNumber Div_mod (const BigNumber& crLHS, const BigNumber& crRHS, const BigNumber& crMOD)
{
    std::unique_ptr<BigNumberImpl> impl_ret = Div_mod(crLHS.pImpl(), crRHS.pImpl(),crMOD.pImpl());
    BigNumber res;
    res.m_pImpl.reset (new BigNumberImpl (*impl_ret));
    return res;
}

// free functions
BigNumber GenerateRand (const int& size )
{
    BigNumber res ;
    res.generateRandHex (size) ;
    return res ; 
}

BigNumber GenerateRandNegative (const int& size)
{
    BigNumber res ; 
    res.generateNegRandHex (size);
    return res; 
}

BigNumber GenerateOne ()
{
    BigNumber bn; 
    bn.One(); 
    return bn; 
}

BigNumber GenerateZero()
{
    BigNumber bn; 
    bn.Zero (); 
    return bn;
}

BigNumber GenerateRandRange(const BigNumber& min, const BigNumber& max ,const int& nsize)
{
    // please note that negative ranges are allowed.
    BigNumber Range = max - min + 1; 
    BigNumber RandomRange;     
    RandomRange.generateRandRange(Range);
    BigNumber Val = min + (RandomRange % Range);
    if (Val < min || Val > max)
        throw std::out_of_range("RANGE VIOLATION MIN VALUE" + min.ToDec() + "\t MAX VALUE " + max.ToDec());
    return Val;
}

BigNumber GenerateRandPrime(const int& size)
{
    BigNumber res;
    res.generateRandPrimeHex(size);
    return res;
}
