#include <SymEncDec/SymEncDec.h>
#include <SymEncDec/SymEncDecImpl.h>



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


#if 0 
int SymEncDec::aes_encrypt (const std::string& text, std::unique_ptr<unsigned char[]>& ctext){
    return m_pImpl->aes_encrypt(text, ctext);
}

int SymEncDec::aes_decrypt (const std::string& ctext, std::unique_ptr<unsigned char[]>& text){
    return m_pImpl->aes_decrypt (ctext, text);
}

int SymEncDec::aes_decryptEx (const std::unique_ptr<unsigned char[]>& ctext, const int& lenctext, std::unique_ptr<unsigned char[]>& ptext){
    return m_pImpl->aes_decryptEx(ctext,lenctext,ptext);
}
#endif

int SymEncDec::aes_encrypt(const std::string& ptext, std::unique_ptr<unsigned char[]>& ctext) {
    return m_pImpl->aes_encrypt(ptext, ctext); 
}


int SymEncDec::aes_decrypt(const std::unique_ptr<unsigned char[]>& ctext, const int& ctextlen,std::string& ptext){
    return m_pImpl->aes_decrypt(ctext, ctextlen, ptext);
}
// free functions
std::unique_ptr<unsigned char[]> KeyGen (std::unique_ptr<unsigned char[]>& pw, const unsigned int& pwlen, const std::unique_ptr<unsigned char[]>& salt, const uint64_t& saltlen, const unsigned int& ic, uint64_t& requiredKeyLen )
{
    return (std::move(KeyGenImpl(pw,pwlen,salt,saltlen,ic,requiredKeyLen)));    
}