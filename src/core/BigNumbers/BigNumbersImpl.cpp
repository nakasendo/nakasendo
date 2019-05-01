#include "BigNumbersImpl.h"
#include <openssl/rand.h>

//using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)> ; 


std::unique_ptr<BigNumberImpl> Add (const BigNumberImpl* obj1, const BigNumberImpl* obj2){
    BN_ptr res(BN_new(), ::BN_free);
    if (!BN_add(res.get(),obj1->m_bn.get(),obj2->m_bn.get())){
        std::cout << "error" << std::endl; 
    }
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) ) ; 
    return std::move(ResImpl);
}

std::unique_ptr<BigNumberImpl> Sub (const BigNumberImpl* obj1, const BigNumberImpl* obj2){
    BN_ptr res (BN_new(),::BN_free);
    if(!BN_sub(res.get(), obj1->m_bn.get(), obj2->m_bn.get())){
        std::cout << "error" << std::endl ; 
    }
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl(res));
    return std::move(ResImpl);
}

std::unique_ptr<BigNumberImpl> Mod (const BigNumberImpl* obj1, const BigNumberImpl* obj2)
{
    BN_CTX* ctxptr = BN_CTX_new(); 
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_mod(res.get(), obj1->m_bn.get(), obj2->m_bn.get(), ctxptr))
    {
        std::cout << "Error" << std::endl ; 
    }
    std::unique_ptr<BigNumberImpl> ResImpl(new BigNumberImpl(res));
    BN_CTX_free(ctxptr);
    return std::move(ResImpl);
}

std::unique_ptr<BigNumberImpl> Mul (const BigNumberImpl* obj1, const BigNumberImpl* obj2)
{
    BN_CTX* ctxptr = BN_CTX_new(); 
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_mul(res.get(), obj1->m_bn.get(), obj2->m_bn.get(), ctxptr)){
        std::cout << "error" << std::endl; 
    }
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) ) ; 
    return std::move(ResImpl);
}

std::unique_ptr<BigNumberImpl> Div (const BigNumberImpl* obj1, const BigNumberImpl* obj2) 
{
    BN_CTX* ctxptr = BN_CTX_new(); 
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_div(res.get(), NULL, obj1->m_bn.get(), obj2->m_bn.get(), ctxptr)){
        std::cout << "error" << std::endl; 
    }
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) ) ; 
    return std::move(ResImpl);

}

std::unique_ptr<BigNumberImpl>  LShift(const BigNumberImpl* obj, const int& val)
{
    BN_ptr res(BN_new(), ::BN_free);

    if(!BN_lshift(res.get(), obj->m_bn.get(), val))
    {
        std::cout << "Unable to left-shift by " << val << std::endl ; 
    }
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) ) ;
    return std::move(ResImpl);
}

std::unique_ptr<BigNumberImpl> RShift(const BigNumberImpl* obj, const int& val)
{
    BN_ptr res(BN_new(), ::BN_free);

    if(!BN_rshift(res.get(), obj->m_bn.get(), val))
    {
        std::cout << "Unable to right-shift by " << val << std::endl ; 
    }
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) ) ;
    return std::move(ResImpl);
}

// BN_cmp() returns -1 if a < b, 0 if a==b, 1 if a > b
bool CMPGreater (const BigNumberImpl* obj1, const BigNumberImpl* obj2){
    if (BN_cmp(obj1->m_bn.get(), obj2->m_bn.get()) == 1 ){
        return true;
    }else{return false;}
}

bool CMPLess (const BigNumberImpl* obj1, const BigNumberImpl* obj2){
    if (BN_cmp(obj1->m_bn.get(), obj2->m_bn.get()) == -1 ){
        return true;
    }else{return false;}
}

bool CMPEqual ( const BigNumberImpl* obj1, const BigNumberImpl* obj2){
    if (BN_cmp(obj1->m_bn.get(), obj2->m_bn.get()) == 0 ){
        return true;
    }else{return false;} 
}


void BigNumberImpl::One()
{
    BN_one (m_bn.get());
}

void BigNumberImpl::Zero()
{
    BN_zero (m_bn.get());
}

void BigNumberImpl::Negative()
{
    BN_set_negative(m_bn.get(), 1);
}

BigNumberImpl BigNumberImpl::operator++(int)
{
    BigNumberImpl bnImpl(*this);
    ++ (*this);
    return bnImpl ;
}

BigNumberImpl& BigNumberImpl::operator++()
{
    BN_ptr res (BN_new(),::BN_free);
    BN_one(res.get());
    if(!BN_add(m_bn.get(),m_bn.get(), res.get())){
        std::cout << "Unable to increment by one" << std::endl ; 
    }
    return *this ; 
}


BigNumberImpl BigNumberImpl::operator--(int)
{
    BigNumberImpl bnImpl(*this);
    -- (*this);
    return bnImpl ;
}

BigNumberImpl& BigNumberImpl::operator--()
{
    BN_ptr res (BN_new(),::BN_free);
    BN_one(res.get());
    if(!BN_sub(m_bn.get(),m_bn.get(), res.get())){
        std::cout << "Unable to decrement by one" << std::endl ; 
    }
    return *this ; 
}


std::string BigNumberImpl::ToHex () const
{
    return (BN_bn2hex(m_bn.get()));    
}

std::string BigNumberImpl::ToDec () const
{
    return (BN_bn2dec(m_bn.get()));
}

int BigNumberImpl::FromHex (const std::string& val)
{
    BIGNUM * ptr = m_bn.get () ; 
    std::string _val;
    unsigned int pos = 0;
     
    // strip off '0x' if the val has it
    if ((val.length() > 0) && (val[pos] == '-' || val[pos] == '+'))
    {
        
        if (val[pos] == '-')
            _val.push_back(val[pos]);
        pos++;
    }

    if ((val.length() >= (pos + 2)) && (val[pos] == '0') && (val[pos+1] == 'x' || val[pos+1] == 'X'))
    {
        pos = pos + 2;
    }

   _val.append(val, pos); 

    return(BN_hex2bn(&ptr, _val.c_str()));
}

int BigNumberImpl::FromDec (const std::string& val)
{
    BIGNUM * ptr = m_bn.get () ; 
    return(BN_dec2bn (&ptr, val.c_str()));
}

void BigNumberImpl::generate(const int& nsize)
{
    // todo seed the PRNG
    if ( !BN_rand(m_bn.get(), nsize, 0,0))
    {
        std::cout << "error generating random number" << std::endl ; 
    }
}

void BigNumberImpl::generateNeg (const int& nsize)
{
    generate(nsize);
    Negative();
}

 void BigNumberImpl::generateRange (const BigNumberImpl* max)
 {
     // seed the PRNG
    if ( !BN_rand_range(m_bn.get(),max->m_bn.get()))
    {
        std::cout << "error generating random number in range 0 - " << max->ToHex ()  << std::endl ; 
    }      
    return ; 
 }

 void BigNumberImpl::seedRNG (const std::string& seed)
 {
     RAND_seed(seed.c_str(), seed.size());
 }

