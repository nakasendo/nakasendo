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

#include <SymEncDec/SymEncDecImpl.h>

using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;
using HMAC_ptr = std::unique_ptr<HMAC_CTX , decltype(&::HMAC_CTX_free)> ; 

SymEncDecImpl::~SymEncDecImpl(){
    //OPENSSL_cleanse(m_key.get(), m_KeySize);
    //OPENSSL_cleanse(m_iv.get(), m_BlockSize);
}



void SymEncDecImpl::SetKeySize (const int& size){
    m_KeySize = size ; 
}

void SymEncDecImpl::SetBlockSize (const int& size){
    m_BlockSize = size ; 
}

void SymEncDecImpl::SetParams(const std::unique_ptr<unsigned char[]>& key, const std::unique_ptr<unsigned char[]>& iv, const unsigned int& keysize, const unsigned int& blocksize){



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
  unsigned int i = 0 ; 
  for (;i<m_BlockSize; ++i){
    m_iv.get()[i]=iv.get()[i];
  }
 
}

void SymEncDecImpl::GetParams(std::unique_ptr<unsigned char[]>& key, std::unique_ptr<unsigned char[]>& iv, unsigned int& keysize, unsigned int& blocksize)
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

// Private parameter-generatiom
void SymEncDecImpl::generateParams (){
  int rc = RAND_bytes(m_key.get(), m_KeySize);
  if (rc != 1)
      throw std::runtime_error("RAND_bytes key failed");

  rc = RAND_bytes(m_iv.get(), m_BlockSize);
  if (rc != 1)
    throw std::runtime_error("RAND_bytes for iv failed");
}


  int SymEncDecImpl::aes_encrypt(const std::string& ptext, std::unique_ptr<unsigned char[]>& ctext) {

    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, m_key.get(), m_iv.get());
    if (rc != 1)
      throw std::runtime_error("EVP_EncryptInit_ex failed");
      // Recovered text expands upto BLOCK_SIZE
    ctext.reset (new unsigned char[ptext.size()+m_BlockSize]);
    int out_len1 = ptext.size()+m_BlockSize;

    rc = EVP_EncryptUpdate(ctx.get(), &(ctext.get()[0]), &out_len1, (const unsigned char*)&ptext[0], (int)ptext.size());
    if (rc != 1)
      throw std::runtime_error("EVP_EncryptUpdate failed");
  
    int out_len2 = (ptext.size()+m_BlockSize) - out_len1;
    rc = EVP_EncryptFinal_ex(ctx.get(), &(ctext.get()[0 + out_len1]), &out_len2);
    if (rc != 1)
      throw std::runtime_error("EVP_EncryptFinal_ex failed");

    int cipherLen(out_len1 + out_len2);
    return cipherLen; 
  }

  int SymEncDecImpl::aes_decrypt(const std::unique_ptr<unsigned char[]>& ctext, const int& ctextLen,std::string& rtext){
    EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
    int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, m_key.get(), m_iv.get());
    if (rc != 1)
      throw std::runtime_error("EVP_DecryptInit_ex failed");

    // Recovered text contracts upto BLOCK_SIZE
    rtext.resize(ctextLen);
    
    int out_len1 = (int)rtext.size();

    rc = EVP_DecryptUpdate(ctx.get(), (unsigned char*)&rtext[0], &out_len1, &(ctext.get()[0]), ctextLen);
    if (rc != 1)
      throw std::runtime_error("EVP_DecryptUpdate failed");
  
    int out_len2 = (int)rtext.size() - out_len1;
    rc = EVP_DecryptFinal_ex(ctx.get(), (unsigned char*)&rtext[0]+out_len1, &out_len2);
    if (rc != 1)
      throw std::runtime_error("EVP_DecryptFinal_ex failed");

    // Set recovered text size now that we know it
    rtext.resize(out_len1 + out_len2);
    return (out_len1 + out_len2); 
  }


// free functions
void  NounceGenImpl(std::unique_ptr<unsigned char[]>& nounce,const int blocksize){
  nounce.reset(new unsigned char[blocksize]);
  int rc = RAND_bytes(nounce.get(), blocksize);
  if (rc != 1)
    throw std::runtime_error("RAND_bytes for iv failed");  
}

std::unique_ptr<unsigned char[]> KeyGenImpl 
(std::unique_ptr<unsigned char[]>& pw, const unsigned int& pwlen, const std::unique_ptr<unsigned char[]>& salt, const uint64_t& saltlen, const unsigned int& ic, uint64_t& requiredKeyLen )
{
  unsigned long i(0), l(0),r(0); 
  std::unique_ptr<unsigned char[]> final (new unsigned char[SHA256_DIGEST_LENGTH]);
  
  std::fill_n (final.get(), SHA256_DIGEST_LENGTH, 0x0);
  if (requiredKeyLen > ((((uint64_t)1) << 32) -1 ) * SHA256_DIGEST_LENGTH){
    return nullptr;
  }
  l = requiredKeyLen / SHA256_DIGEST_LENGTH ;
  r = requiredKeyLen % SHA256_DIGEST_LENGTH ;
  for (int i = 1; i<=l;++i){
    pkcs5F(pw.get(),pwlen, salt.get(),saltlen,ic,i,final.get() + (i-1) * SHA256_DIGEST_LENGTH );
  }
  if(r){
    std::unique_ptr<unsigned char[]> finalr (new unsigned char[SHA256_DIGEST_LENGTH]);
    pkcs5F(pw.get(),pwlen, salt.get(),saltlen,ic,i,finalr.get());
    for(int l=0;l<r;++l){
      *(final.get()+(i-1)*SHA256_DIGEST_LENGTH) = finalr.get()[l];
    }
  }
  return final;
}

void pkc5InitialPRF(unsigned char *p, size_t plen, unsigned char *salt,size_t saltlen, size_t i, unsigned char *out, size_t *outlen){
  size_t swapped_i(0);

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
  std::unique_ptr<unsigned char[]> ulast ( new unsigned char[SHA256_DIGEST_LENGTH]);
  
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

