#ifndef __SYMENCRYPTDECRYPTIMPL_H__
#define __SYMENCRYPTDECRYPTIMPL_H__

#include <memory>
#include <iostream>
#include <string>



class SymEncryptionDecryptImpl
{
    public:

        SymEncryptionDecryptImpl() : m_KeySize(32), m_BlockSize(16)
        { 
            m_key.reset (new unsigned char[m_KeySize]);
            m_iv.reset (new unsigned char[m_BlockSize]);
            generateParams ();
            return ;
        }

        SymEncryptionDecryptImpl(const int& keysize, const int& blocksize) : m_KeySize(keysize), m_BlockSize(blocksize)
        { 
            m_key.reset (new unsigned char[m_KeySize]);
            m_iv.reset (new unsigned char[m_BlockSize]);
            generateParams();
            return ;
        }

        ~SymEncryptionDecryptImpl();


 //
        SymEncryptionDecryptImpl (const SymEncryptionDecryptImpl& obj) 
            : m_KeySize(obj.m_KeySize), m_BlockSize(obj.m_BlockSize)
        { 
            m_key.reset (new unsigned char(*obj.m_key)); 
            m_iv.reset (new unsigned char(*obj.m_iv));
        }
        

        SymEncryptionDecryptImpl& operator= (const SymEncryptionDecryptImpl& obj)
        { 
            if (this != &obj){
                m_KeySize = obj.m_KeySize; 
                m_BlockSize = obj.m_BlockSize;
                m_key.reset (new unsigned char(*obj.m_key)); 
                m_iv.reset (new unsigned char(*obj.m_iv));
            }
            return *this;
        }

        //moveable
        SymEncryptionDecryptImpl (SymEncryptionDecryptImpl&& obj) noexcept = default;
        SymEncryptionDecryptImpl& operator=(SymEncryptionDecryptImpl&& obj) noexcept = default;

        void SetKeySize(const int&) ; 
        void SetBlockSize(const int&);

        void SetParams(const std::unique_ptr<unsigned char>&, const std::unique_ptr<unsigned char>&, const unsigned int&, const unsigned int& );
        void GetParams(std::unique_ptr<unsigned char>&, std::unique_ptr<unsigned char>&, unsigned int&, unsigned int&);

        int aes_encrypt (const std::string&, std::unique_ptr<unsigned char>& ) ; 
        int aes_decrypt (const std::string&, std::unique_ptr<unsigned char>&) ; 
        
        

    private:
        unsigned int m_KeySize ; 
        unsigned int m_BlockSize ; 
        std::unique_ptr<unsigned char> m_key; 
        std::unique_ptr<unsigned char> m_iv;

        void generateParams() ; 


};

// free functions
std::unique_ptr<unsigned char> KeyGenImpl (std::unique_ptr<unsigned char>& pw, const unsigned int& pwlen, const std::unique_ptr<unsigned char>& salt, const uint64_t& saltlen, const unsigned int& ic, uint64_t& requiredKeyLen ) ;
void pkc5InitialPRF(unsigned char *p, size_t plen, unsigned char *salt,size_t saltlen, size_t i, unsigned char *out, size_t *outlen);
void pkc5SubsequentPRF(unsigned char *p, size_t plen, unsigned char *v,size_t vlen, unsigned char *out, size_t *outlen);
void pkcs5F(unsigned char *p, size_t plen, unsigned char *salt, size_t saltlen, size_t ic, size_t bix, unsigned char * out);

#endif //ifndef __SYMENCRYPTDECRYPT_H__
