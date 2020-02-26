#ifndef SYM_ENC_DEC_H
#define SYM_ENC_DEC_H

#include <SymEncDec/SymEncDecConfig.h>

#include <memory>
#include <string>

class SymEncDecImpl;

class SymEncDec_API SymEncDec
{
    public:
        explicit SymEncDec();
        explicit SymEncDec(const int&, const int&);
        ~SymEncDec();
            //moveable
        SymEncDec(SymEncDec&& obj) noexcept;
        SymEncDec& operator=(SymEncDec&& obj) noexcept;
            //copyable
        SymEncDec(const SymEncDec& obj);
        SymEncDec& operator=(const SymEncDec& obj);

        void SetKeySize(const int&) ; 
        void SetBlockSize(const int&);

        void SetParams( const std::unique_ptr<unsigned char[]>&, const std::unique_ptr<unsigned char[]>&, const unsigned int&, const unsigned int& );
        void GetParams(std::unique_ptr<unsigned char[]>&, std::unique_ptr<unsigned char[]>&, unsigned int&, unsigned int&);

        int aes_encrypt(const std::string&, std::unique_ptr<unsigned char[]>& ) ;
        int aes_decrypt(const std::unique_ptr<unsigned char[]>&, const int&,std::string&);

    private:
         std::unique_ptr<SymEncDecImpl> m_pImpl ;

};

// free function interface
SymEncDec_API std::string Encode(const std::string& crMsg, const std::string& crKey, const std::string& crIV, uint64_t keylen=32, uint64_t blocksize=16);
SymEncDec_API std::string Decode(const std::string& crMsg, const std::string& crKey, const std::string& crIV, uint64_t keylen=32, uint64_t blocksize=16);

SymEncDec_API std::string GenerateKey256(const std::string& crKey, const std::string& crIV, uint64_t keylen=32, uint64_t blocksize=16);
SymEncDec_API std::string GenerateNounce(uint64_t blocksize=16);

SymEncDec_API std::unique_ptr<unsigned char[]> KeyGen(std::unique_ptr<unsigned char[]>&, const unsigned int&, const std::unique_ptr<unsigned char[]>& , const uint64_t& , const unsigned int& , uint64_t&) ;
SymEncDec_API void NounceGen(std::unique_ptr<unsigned char[]>&, const int blocksize=16);
#endif /* SYM_ENC_DEC_H */
