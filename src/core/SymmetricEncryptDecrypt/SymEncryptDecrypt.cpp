#include "SymEncryptDecrypt.h"
#include "SymEncryptDecryptImpl.h"



SymEncryptDecrypt::SymEncryptDecrypt() : m_pImpl(new SymEncryptionDecryptImpl){return;}
SymEncryptDecrypt::SymEncryptDecrypt(const int& keysize, const int& blocksize)
    : m_pImpl (new SymEncryptionDecryptImpl (keysize,blocksize)) { return ; }

SymEncryptDecrypt::~SymEncryptDecrypt()=default;

SymEncryptDecrypt::SymEncryptDecrypt(SymEncryptDecrypt&& obj) noexcept = default;
SymEncryptDecrypt& SymEncryptDecrypt::operator=(SymEncryptDecrypt&& obj) noexcept = default;

SymEncryptDecrypt::SymEncryptDecrypt(const SymEncryptDecrypt& obj)
    : m_pImpl(new SymEncryptionDecryptImpl (*obj.m_pImpl))
{
    return ; 
}
SymEncryptDecrypt& SymEncryptDecrypt::operator=(const SymEncryptDecrypt& obj){
    if (this != &obj)
    {
        m_pImpl.reset( new SymEncryptionDecryptImpl (*obj.m_pImpl));
    }
    return *this;
}


void SymEncryptDecrypt::SetKeySize(const int& size){
    m_pImpl->SetKeySize(size);
}
void SymEncryptDecrypt::SetBlockSize(const int& size){
    m_pImpl->SetBlockSize(size);
}

void SymEncryptDecrypt::SetParams( const std::unique_ptr<unsigned char>& key, const std::unique_ptr<unsigned char>& iv, const unsigned int& keysize, const unsigned int& blocksize){
    m_pImpl->SetParams(key, iv, keysize, blocksize);
}

void SymEncryptDecrypt::GetParams( std::unique_ptr<unsigned char>& key, std::unique_ptr<unsigned char>& iv, unsigned int& keysize, unsigned int& blocksize){
    m_pImpl->GetParams(key, iv, keysize, blocksize);
}



int SymEncryptDecrypt::aes_encrypt (const std::string& text, std::unique_ptr<unsigned char>& ctext){
    return m_pImpl->aes_encrypt(text, ctext);
}

int SymEncryptDecrypt::aes_decrypt (const std::string& ctext, std::unique_ptr<unsigned char>& text){
    return m_pImpl->aes_decrypt (ctext, text);
}

// free functions
std::unique_ptr<unsigned char> KeyGen (std::unique_ptr<unsigned char>& pw, const unsigned int& pwlen, const std::unique_ptr<unsigned char>& salt, const uint64_t& saltlen, const unsigned int& ic, uint64_t& requiredKeyLen )
{
    return (std::move(KeyGenImpl(pw,pwlen,salt,saltlen,ic,requiredKeyLen)));    
}