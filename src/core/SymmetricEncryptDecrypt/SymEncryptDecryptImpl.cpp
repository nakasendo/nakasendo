#include <string.h>
#ifdef WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

 #include <openssl/err.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

#include "SymEncryptDecryptImpl.h"

using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;
using HMAC_ptr = std::unique_ptr<HMAC_CTX , decltype(&::HMAC_CTX_free)> ; 

SymEncryptionDecryptImpl::~SymEncryptionDecryptImpl(){
  OPENSSL_cleanse(m_key.get(), m_KeySize);
  OPENSSL_cleanse(m_iv.get(), m_BlockSize);
}



void SymEncryptionDecryptImpl::SetKeySize (const int& size){
  m_KeySize = size ; 
}

void SymEncryptionDecryptImpl::SetBlockSize (const int& size){
  m_BlockSize = size ; 
}

void SymEncryptionDecryptImpl::SetParams(const std::unique_ptr<unsigned char>& key, const std::unique_ptr<unsigned char>& iv, const unsigned int& keysize, const unsigned int& blocksize){



  if (keysize != m_KeySize){
    m_KeySize = keysize; 
    m_key.reset ( new unsigned char[m_KeySize]);
  }

  if (blocksize != m_BlockSize){
    m_BlockSize = blocksize;
    m_iv.reset ( new unsigned char[m_BlockSize]);
  }
  for (unsigned int i=0; i<m_KeySize; ++i)
  {
    m_key.get()[i] = key.get()[i];
  }
  for (unsigned int i=0;i<m_BlockSize; ++i){
    m_iv.get()[i]=iv.get()[i];
  }
}

void SymEncryptionDecryptImpl::GetParams(std::unique_ptr<unsigned char>& key, std::unique_ptr<unsigned char>& iv, unsigned int& keysize, unsigned int& blocksize)
{
  // reset to the correct size
  key.reset (new unsigned char[m_KeySize] );
  iv.reset (new unsigned char[m_BlockSize]);
  for (unsigned int i=0; i<m_KeySize; ++i)
  {
    key.get()[i] = m_key.get()[i];
  }
  for (unsigned int i=0;i<m_BlockSize; ++i){
    iv.get()[i]=m_iv.get()[i];
  }

  keysize = m_KeySize ; 
  blocksize = m_BlockSize;  
  return ; 
}

int  SymEncryptionDecryptImpl::aes_encrypt (const std::string& pText, std::unique_ptr<unsigned char>& ctext){
  EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
  int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, m_key.get(), m_iv.get());
  if (rc != 1)
    return -1;

  // Recovered text expands upto BLOCK_SIZE
  std::string ctextInternal; 
  ctextInternal.resize (pText.size()+m_BlockSize);
  
  int out_len1 = (pText.size()+m_BlockSize);

  rc = EVP_EncryptUpdate(ctx.get(), (unsigned char*)&ctextInternal[0], &out_len1, (unsigned char*)&pText[0], (int)pText.size());
  if (rc != 1)
    return -1;

  int out_len2 = ctextInternal.size () - out_len1; 

  rc = EVP_EncryptFinal_ex(ctx.get(),(unsigned char*)&ctextInternal[0]+out_len1, &out_len2); 
  if (rc != 1)
    return -1;

  int cypherlen = out_len1 + out_len2; 
  ctext.reset ( new unsigned char [cypherlen + 1]);
  std::fill_n(ctext.get(), cypherlen + 1, 0x00);
  for(int i=0;i<cypherlen;++i){
    ctext.get()[i]=ctextInternal[i];
  }
  return cypherlen; 
}

int SymEncryptionDecryptImpl::aes_decrypt( const std::string& ctext, std::unique_ptr<unsigned char>& text ){
  EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
  int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, m_key.get(), m_iv.get());
  if (rc != 1)
    return -1;

  std::string rtext ; 
  rtext.resize (ctext.size());
  // Recovered text contracts upto BLOCK_SIZE
  int out_len1 = ctext.size();
  
  rc = EVP_DecryptUpdate(ctx.get(), (unsigned char*)&rtext[0], &out_len1, (unsigned char*)&ctext[0], (int)ctext.size());
  if (rc != 1)
    return -1;

  int out_len2 = rtext.size() - out_len1;
  rc = EVP_DecryptFinal_ex(ctx.get(), (unsigned char*)&rtext[0]+out_len1, &out_len2);
  if (rc != 1){
    ERR_print_errors_fp(stderr);
    return -1;
  }
    

  int cypherlen = out_len1 + out_len2; 
  text.reset (new unsigned char[cypherlen+1]);
  
  std::fill_n(text.get(), cypherlen+1, 0x00);
  for (int i=0;i<cypherlen;++i){
    text.get()[i]=rtext[i];
  }
  return cypherlen;
}
   



// Private parameter-generatiom
void SymEncryptionDecryptImpl::generateParams (){
  int rc = RAND_bytes(m_key.get(), m_KeySize);
  if (rc != 1)
      throw std::runtime_error("RAND_bytes key failed");

  rc = RAND_bytes(m_iv.get(), m_BlockSize);
  if (rc != 1)
    throw std::runtime_error("RAND_bytes for iv failed");
}




// free functions
std::unique_ptr<unsigned char> KeyGenImpl 
(std::unique_ptr<unsigned char>& pw, const unsigned int& pwlen, const std::unique_ptr<unsigned char>& salt, const uint64_t& saltlen, const unsigned int& ic, uint64_t& requiredKeyLen )
{
  unsigned long i, l,r; 
  std::unique_ptr<unsigned char> final (new unsigned char[SHA256_DIGEST_LENGTH]);
  
  std::fill_n (final.get(), SHA256_DIGEST_LENGTH, 0x00);
  if (requiredKeyLen > ((((uint64_t)1) << 32) -1 ) * SHA256_DIGEST_LENGTH){
    return nullptr;
  }
  l = requiredKeyLen / SHA256_DIGEST_LENGTH ;
  r = requiredKeyLen % SHA256_DIGEST_LENGTH ;
  for (int i = 1; i<=l;++i){
    pkcs5F(pw.get(),pwlen, salt.get(),saltlen,ic,i,final.get() + (i-1) * SHA256_DIGEST_LENGTH );
  }
  if(r){
    std::unique_ptr<unsigned char> finalr (new unsigned char[SHA256_DIGEST_LENGTH]);
    pkcs5F(pw.get(),pwlen, salt.get(),saltlen,ic,i,finalr.get());
    for(int l=0;l<r;++l){
      *(final.get()+(i-1)*SHA256_DIGEST_LENGTH) = finalr.get()[l];
    }
  }
  return std::move(final);
}

void pkc5InitialPRF(unsigned char *p, size_t plen, unsigned char *salt,size_t saltlen, size_t i, unsigned char *out, size_t *outlen){
  size_t swapped_i;

  HMAC_ptr ctx ( HMAC_CTX_new(),::HMAC_CTX_free); 
  HMAC_Init_ex(ctx.get(), p, plen, EVP_sha256(), 0);                                  
  HMAC_Update(ctx.get(),salt,saltlen) ; 
  swapped_i = htonl(i); 
  HMAC_Update(ctx.get(), (unsigned char *)&swapped_i, 4); 
  HMAC_Final(ctx.get(), out, (unsigned int *)outlen); 
}

void pkc5SubsequentPRF(unsigned char *p, size_t plen, unsigned char *v,size_t vlen, unsigned char *out, size_t *outlen){
  HMAC_ptr ctx ( HMAC_CTX_new(),::HMAC_CTX_free); 
  HMAC_Init_ex(ctx.get(), p, plen, EVP_sha256(), 0);                                  
  HMAC_Update(ctx.get(),v,vlen) ; 
  HMAC_Final(ctx.get(), out, (unsigned int *)outlen); 
}

void pkcs5F(unsigned char *p, size_t plen, unsigned char *salt, size_t saltlen, size_t ic, size_t bix, unsigned char * out){
  size_t i=1,j,outlen;
  std::unique_ptr<unsigned char> ulast ( new unsigned char[SHA256_DIGEST_LENGTH]);
  
  pkc5InitialPRF(p,plen,salt,saltlen, bix, ulast.get(),&outlen);
  while(i++ <= ic){
    for(j=0;j< SHA256_DIGEST_LENGTH;j++){
      out[j]^= ulast.get()[j];
    }
    pkc5SubsequentPRF(p,plen,ulast.get(),SHA256_DIGEST_LENGTH,ulast.get(), &outlen );
  }

  for(j=0;j<SHA256_DIGEST_LENGTH;++j){
    out[j] = ulast.get()[j];
  }
}

