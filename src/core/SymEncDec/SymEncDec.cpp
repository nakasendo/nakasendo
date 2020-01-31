#include <SymEncDec/SymEncDec.h>
#include <SymEncDec/SymEncDecImpl.h>
#include <MessageHash/conversions.h>


SymEncDec::SymEncDec() : m_pImpl(new SymEncDecImpl){return;}
SymEncDec::SymEncDec(const int& keysize, const int& blocksize)
    : m_pImpl (new SymEncDecImpl (keysize,blocksize)) { return ; }

SymEncDec::~SymEncDec()=default;

SymEncDec::SymEncDec(SymEncDec&& obj) noexcept = default;
SymEncDec& SymEncDec::operator=(SymEncDec&& obj) noexcept = default;

SymEncDec::SymEncDec(const SymEncDec& obj)
    : m_pImpl(new SymEncDecImpl (*obj.m_pImpl))
{
    return ; 
}
SymEncDec& SymEncDec::operator=(const SymEncDec& obj){
    if (this != &obj)
    {
        m_pImpl.reset( new SymEncDecImpl (*obj.m_pImpl));
    }
    return *this;
}


void SymEncDec::SetKeySize(const int& size){
    m_pImpl->SetKeySize(size);
}
void SymEncDec::SetBlockSize(const int& size){
    m_pImpl->SetBlockSize(size);
}

void SymEncDec::SetParams( const std::unique_ptr<unsigned char[]>& key, const std::unique_ptr<unsigned char[]>& iv, const unsigned int& keysize, const unsigned int& blocksize){
    m_pImpl->SetParams(key, iv, keysize, blocksize);
}

void SymEncDec::GetParams( std::unique_ptr<unsigned char[]>& key, std::unique_ptr<unsigned char[]>& iv, unsigned int& keysize, unsigned int& blocksize){
    m_pImpl->GetParams(key, iv, keysize, blocksize);
}



int SymEncDec::aes_encrypt(const std::string& ptext, std::unique_ptr<unsigned char[]>& ctext) {
    return m_pImpl->aes_encrypt(ptext, ctext); 
}


int SymEncDec::aes_decrypt(const std::unique_ptr<unsigned char[]>& ctext, const int& ctextlen,std::string& ptext){
    return m_pImpl->aes_decrypt(ctext, ctextlen, ptext);
}
// free functions

std::string Encode(const std::string& crMsg, const std::string& crKey, const std::string& crIV, uint64_t keylen, uint64_t blocksize)
{
    size_t keyBufferLen(0);
    std::unique_ptr<unsigned char[]> myKey = HexStrToBin(crKey, &keyBufferLen);
    size_t nounceBufferLen(0);
    std::unique_ptr<unsigned char[]> mySalt = HexStrToBin(crIV, &nounceBufferLen);

    if (keyBufferLen != keylen){
        throw std::runtime_error ("Incorrect Key size");
    } 
    if(nounceBufferLen != blocksize){
        throw std::runtime_error ("Incorrect nounce size");
    }
    SymEncDec encdec;

    encdec.SetParams(myKey, mySalt, keylen,blocksize);
    std::unique_ptr<unsigned char[]> encMsg;
    int encMsgLen = encdec.aes_encrypt(crMsg, encMsg);

    const std::string hexvals = binTohexStr(encMsg, encMsgLen);

    return hexvals;
}

std::string Decode(const std::string& crMsg, const std::string& crKey, const std::string& crIV, uint64_t keylen, uint64_t blocksize)
{
    size_t keyBufferLen(0);
    std::unique_ptr<unsigned char[]> myKey = HexStrToBin(crKey, &keyBufferLen);
    size_t nounceBufferLen(0);
    std::unique_ptr<unsigned char[]> mySalt = HexStrToBin(crIV, &nounceBufferLen);

    if (keyBufferLen != keylen){
        throw std::runtime_error ("Incorrect Key size");
    } 
    if(nounceBufferLen != blocksize){
        throw std::runtime_error ("Incorrect nounce size");
    }

    SymEncDec encdec;
    encdec.SetParams(myKey, mySalt, keylen, blocksize);
    size_t bufferLen(0);
    std::unique_ptr<unsigned char[]> recoveredBuf =  HexStrToBin(crMsg, &bufferLen);

    std::string retval;  
    encdec.aes_decrypt(recoveredBuf, bufferLen, retval);
    return retval;
}

std::string GenerateKey256(const std::string& crKey, const std::string& crIV, uint64_t keylen, uint64_t blocksize)
{
    // This assumes that the IV is a hex encoded 16 byte number

    std::unique_ptr<unsigned char[]> myKey (new unsigned char [crKey.length() + 1 ]);
    std::fill_n(myKey.get(), crKey.length()+1, 0x00);
    int index(0);

    for(std::string::const_iterator iter = crKey.begin(); iter != crKey.end(); ++ iter, ++index){
        myKey.get()[index]=*iter;
    }

    size_t ivbufferLen;
    std::unique_ptr<unsigned char[]> mySalt = HexStrToBin(crIV, &ivbufferLen);


    if (ivbufferLen != blocksize){
        throw std::runtime_error ("Nounce size is not equal to the block size") ; 
    }

    int iterCount(10000);
    std::unique_ptr<unsigned char[]> encodingKey = KeyGen(myKey,crKey.size(),mySalt, blocksize,iterCount, keylen);

    const std::string hexBuilder = binTohexStr(encodingKey, keylen);

    return hexBuilder;
}

std::string GenerateNounce(uint64_t blocksize)
{
    std::unique_ptr<unsigned char[]> nounce;
    try{
        //change to return the nouce size. 
        NounceGen(nounce);
    }
    catch(const std::exception& e){
        std::string errorMsg{"Failed to generate a nounce value "} ; errorMsg += e.what();
        throw std::runtime_error(errorMsg);
    }

    const std::string returnNounce = binTohexStr(nounce, 16);
    return returnNounce;
}

std::unique_ptr<unsigned char[]> KeyGen (std::unique_ptr<unsigned char[]>& pw, const unsigned int& pwlen, const std::unique_ptr<unsigned char[]>& salt, const uint64_t& saltlen, const unsigned int& ic, uint64_t& requiredKeyLen )
{
    return (KeyGenImpl(pw,pwlen,salt,saltlen,ic,requiredKeyLen));    
}

void NounceGen(std::unique_ptr<unsigned char[]>& nounce,const int blocksize){
    NounceGenImpl(nounce,blocksize);
}
