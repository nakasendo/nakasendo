#include <BigNumbers/BigNumbersImpl.h>
#include <openssl/rand.h>

//using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)> ; 

inline void help_openssl_free_char(char* p) { OPENSSL_free(p); }
inline void help_openssl_free_uchar(unsigned char* p) { OPENSSL_free(p); }


std::unique_ptr<BigNumberImpl> Add (const BigNumberImpl* obj1, const BigNumberImpl* obj2){
    BN_ptr res(BN_new(), ::BN_free);
    if (!BN_add(res.get(),obj1->m_bn.get(),obj2->m_bn.get()))
        throw std::runtime_error("error");
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) ) ;
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Sub (const BigNumberImpl* obj1, const BigNumberImpl* obj2){
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_sub(res.get(), obj1->m_bn.get(), obj2->m_bn.get()))
        throw std::runtime_error("error");
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl(res));
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Mul (const BigNumberImpl* obj1, const BigNumberImpl* obj2)
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_mul(res.get(), obj1->m_bn.get(), obj2->m_bn.get(), ctxptr.get()))
        throw std::runtime_error("error");
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) );
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Div (const BigNumberImpl* obj1, const BigNumberImpl* obj2)
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_div(res.get(), NULL, obj1->m_bn.get(), obj2->m_bn.get(), ctxptr.get()))
        throw std::runtime_error("error");
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) );
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Mod (const BigNumberImpl* obj1, const BigNumberImpl* obj2)
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_mod(res.get(), obj1->m_bn.get(), obj2->m_bn.get(), ctxptr.get()))
        throw std::runtime_error("error");
    std::unique_ptr<BigNumberImpl> ResImpl(new BigNumberImpl(res));
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Inv_mod (const BigNumberImpl*  pARG, const BigNumberImpl*  pmod)
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_mod_inverse(res.get(), pARG->m_bn.get(), pmod->m_bn.get(), ctxptr.get()))
        throw std::runtime_error("error mod inverse");
    std::unique_ptr<BigNumberImpl> ResImpl(new BigNumberImpl(res));
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Add_mod (const BigNumberImpl*  pLHS, const BigNumberImpl*  pRHS, const BigNumberImpl*  pmod)
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_mod_add(res.get(), pLHS->m_bn.get(), pRHS->m_bn.get(), pmod->m_bn.get(), ctxptr.get()))
        throw std::runtime_error("error mod add");
    std::unique_ptr<BigNumberImpl> ResImpl(new BigNumberImpl(res));
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Sub_mod (const BigNumberImpl*  pLHS, const BigNumberImpl*  pRHS, const BigNumberImpl*  pmod)
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_mod_sub(res.get(), pLHS->m_bn.get(), pRHS->m_bn.get(), pmod->m_bn.get(), ctxptr.get()))
        throw std::runtime_error("error mod sub");
    std::unique_ptr<BigNumberImpl> ResImpl(new BigNumberImpl(res));
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Mul_mod (const BigNumberImpl*  pLHS, const BigNumberImpl*  pRHS, const BigNumberImpl*  pmod)
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    BN_ptr res (BN_new(),::BN_free);
    if (!BN_mod_mul(res.get(), pLHS->m_bn.get(), pRHS->m_bn.get(), pmod->m_bn.get(), ctxptr.get()))
        throw std::runtime_error("error mod mul");
    std::unique_ptr<BigNumberImpl> ResImpl(new BigNumberImpl(res));
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> Div_mod (const BigNumberImpl*  pLHS, const BigNumberImpl*  pRHS, const BigNumberImpl*  pmod)
{
    std::unique_ptr<BigNumberImpl> invRHS = Inv_mod(pRHS, pmod);
    std::unique_ptr<BigNumberImpl> resDIV = Mul_mod(pLHS, invRHS.get(),pmod);
    return resDIV;
}


std::unique_ptr<BigNumberImpl>  LShift(const BigNumberImpl* obj, const int& val)
{
    BN_ptr res(BN_new(), ::BN_free);

    if(!BN_lshift(res.get(), obj->m_bn.get(), val))
        throw std::runtime_error("Unable to left-shift by " + std::to_string(val));
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) ) ;
    return ResImpl;
}

std::unique_ptr<BigNumberImpl> RShift(const BigNumberImpl* obj, const int& val)
{
    BN_ptr res(BN_new(), ::BN_free);

    if(!BN_rshift(res.get(), obj->m_bn.get(), val))
        throw std::runtime_error("Unable to right-shift by " + std::to_string(val));
    std::unique_ptr<BigNumberImpl> ResImpl (new BigNumberImpl (res) ) ;
    return ResImpl;
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
    if(!BN_add(m_bn.get(),m_bn.get(), res.get()))
        throw std::runtime_error("Unable to increment by one");
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
    if(!BN_sub(m_bn.get(),m_bn.get(), res.get()))
        throw std::runtime_error("Unable to decrement by one");
    return *this ; 
}


std::string BigNumberImpl::ToHex () const
{
    using SSL_CharPtr = std::unique_ptr<char, decltype(&help_openssl_free_char)>;
    SSL_CharPtr hex_str(BN_bn2hex(m_bn.get()), &help_openssl_free_char);
    std::string ret_str (hex_str.get());
    return ret_str;
}

std::string BigNumberImpl::ToDec () const
{
    using SSL_CharPtr = std::unique_ptr<char, decltype(&help_openssl_free_char)>;
    SSL_CharPtr dec_str(BN_bn2dec(m_bn.get()), &help_openssl_free_char);
    std::string ret_str(dec_str.get());
    return ret_str;
}

int BigNumberImpl::FromHex (const std::string& val)
{
    BIGNUM * ptr = m_bn.get () ; 
    return(BN_hex2bn(&ptr, val.c_str()));
}

int BigNumberImpl::FromDec (const std::string& val)
{
    BIGNUM * ptr = m_bn.get () ; 
    return(BN_dec2bn (&ptr, val.c_str()));
}

// get bin value to BN
int BigNumberImpl::FromBin(unsigned char *val, int size)
{
    if (val == NULL || size <=0)
	    return -1;

    // convert bin val to Big number
    if (BN_bin2bn(val, size,  m_bn.get()) == nullptr)
        return -1;

    return 1;
}

int BigNumberImpl::FromBin(std::vector<uint8_t>& val)
{
    if (val.size() == 0)
	    return -1;

    // get the raw data from the vector
    unsigned char *valData = val.data();
    
    // get bin value to BN
    return FromBin(valData, (int)val.size());
}

std::vector<uint8_t>  BigNumberImpl::ToBin() const
{
    using SSL_UCharPtr = std::unique_ptr<unsigned char, decltype(&help_openssl_free_uchar)>;
    size_t len = BN_num_bytes(m_bn.get());
    SSL_UCharPtr binBn((unsigned char *)OPENSSL_malloc(len), &help_openssl_free_uchar);
    if (!binBn.get())
        return {};

    size_t ret = BN_bn2bin(m_bn.get(), binBn.get());
    if (ret != len)
        return std::vector<uint8_t>();

    std::vector<uint8_t>  retVec(binBn.get(), binBn.get()+ret);
    return retVec;
}

void BigNumberImpl::generate(const int& nsize)
{
    // todo seed the PRNG
    if ( !BN_rand(m_bn.get(), nsize, 0,0))
        throw std::runtime_error("error generating random number");
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
        throw std::range_error("error generating random number in range 0 - " + max->ToHex());
    return ; 
}

void BigNumberImpl::generatePrime(const int& nsize)
{
    if (!BN_generate_prime_ex(m_bn.get(), nsize, 0, nullptr, nullptr, nullptr))
        throw std::runtime_error("error generating prime number");
}

bool BigNumberImpl::isPrime() const
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    const bool res = (bool) BN_is_prime_ex(m_bn.get(), BN_prime_checks, ctxptr.get(), nullptr);
    return res;
}

bool BigNumberImpl::isPrimeFasttest() const
{
    std::unique_ptr<BN_CTX, decltype(&BN_CTX_free)> ctxptr (BN_CTX_new(), &BN_CTX_free );
    const bool res = (bool)BN_is_prime_fasttest_ex(m_bn.get(), BN_prime_checks, ctxptr.get(), 0, nullptr);
    return res;
}

void BigNumberImpl::seedRNG (const std::string& seed)
{
    RAND_seed(seed.c_str(), (int)seed.size());
}