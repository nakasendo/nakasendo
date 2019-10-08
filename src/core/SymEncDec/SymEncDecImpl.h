#ifndef SYM_ENC_DEC_IMPL_H
#define SYM_ENC_DEC_IMPL_H

#include <memory>
#include <iostream>
#include <string>

class SymEncDecImpl
{
    public:

        SymEncDecImpl() : m_KeySize(32), m_BlockSize(16)
        { 
            m_key.reset (new unsigned char[m_KeySize]);
            m_iv.reset (new unsigned char[m_BlockSize]);
            generateParams ();
            return ;
        }

        SymEncDecImpl(const int& keysize, const int& blocksize) : m_KeySize(keysize), m_BlockSize(blocksize)
        { 
            m_key.reset (new unsigned char[m_KeySize]);
            m_iv.reset (new unsigned char[m_BlockSize]);
            generateParams();
            return ;
        }

        ~SymEncDecImpl();


 //
        SymEncDecImpl (const SymEncDecImpl& obj) 
            : m_KeySize(obj.m_KeySize), m_BlockSize(obj.m_BlockSize)
        { 
            m_key.reset (new unsigned char[m_KeySize]); 
            m_iv.reset (new unsigned char[m_BlockSize]);
            for (int i = 0; i<m_KeySize; ++i){
                m_key.get()[i] = obj.m_key.get()[i];
            }
            for (int i = 0; i<m_BlockSize; ++i){
                m_iv.get()[i] = obj.m_iv.get()[i];
            }
        }
        

        SymEncDecImpl& operator= (const SymEncDecImpl& obj)
        { 
            if (this != &obj){
                m_KeySize = obj.m_KeySize; 
                m_BlockSize = obj.m_BlockSize;
                m_key.reset (new unsigned char[m_KeySize]); 
                m_iv.reset (new unsigned char[m_BlockSize]);
                for (int i = 0; i<m_KeySize; ++i){
                    m_key.get()[i] = obj.m_key.get()[i];
                }
                for (int i = 0; i<m_BlockSize; ++i){
                    m_iv.get()[i] = obj.m_iv.get()[i];
                }
            }
            return *this;
        }

        //moveable
        SymEncDecImpl (SymEncDecImpl&& obj) noexcept = default;
        SymEncDecImpl& operator=(SymEncDecImpl&& obj) noexcept = default;

        void SetKeySize(const int&) ; 
        void SetBlockSize(const int&);

        void SetParams(const std::unique_ptr<unsigned char[]>&, const std::unique_ptr<unsigned char[]>&, const unsigned int&, const unsigned int& );
        void GetParams(std::unique_ptr<unsigned char[]>&, std::unique_ptr<unsigned char[]>&, unsigned int&, unsigned int&);
        int aes_encrypt(const std::string&, std::unique_ptr<unsigned char[]>& ) ;
        int aes_decrypt(const std::unique_ptr<unsigned char[]>&, const int&,std::string&);


    private:
        unsigned int m_KeySize ; 
        unsigned int m_BlockSize ; 
        std::unique_ptr<unsigned char[]> m_key; 
        std::unique_ptr<unsigned char[]> m_iv;

        void generateParams() ; 


};

// free functions


void NounceGenImpl(std::unique_ptr<unsigned char[]>&,const int blocksize=16);
std::unique_ptr<unsigned char[]> KeyGenImpl (std::unique_ptr<unsigned char[]>& pw, const unsigned int& pwlen, const std::unique_ptr<unsigned char[]>& salt, const uint64_t& saltlen, const unsigned int& ic, uint64_t& requiredKeyLen ) ;
void pkc5InitialPRF(unsigned char *p, size_t plen, unsigned char *salt,size_t saltlen, size_t i, unsigned char *out, size_t *outlen);
void pkc5SubsequentPRF(unsigned char *p, size_t plen, unsigned char *v,size_t vlen, unsigned char *out, size_t *outlen);
void pkcs5F(unsigned char *p, size_t plen, unsigned char *salt, size_t saltlen, size_t ic, size_t bix, unsigned char * out);

#endif /* SYM_ENC_DEC_IMPL_H */
