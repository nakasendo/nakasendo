#ifndef __SYMENCRYPTDECRYPT_H__
#define __SYMENCRYPTDECRYPT_H__


#include <DYNAMIC_LIBRARY_API.hpp>
#ifdef EXPORT_SymEncDec
#    define SymEncDec_API EXPORT_DYNAMIC_LIBRARY
#else
#    define SymEncDec_API IMPORT_DYNAMIC_LIBRARY
#endif

#include <memory>

class SymEncryptionDecryptImpl;

class SymEncDec_API SymEncryptDecrypt
{
    public:
        explicit SymEncryptDecrypt();
        explicit SymEncryptDecrypt(const int&, const int&);
        ~SymEncryptDecrypt();
            //moveable
        SymEncryptDecrypt(SymEncryptDecrypt&& obj) noexcept;
        SymEncryptDecrypt& operator=(SymEncryptDecrypt&& obj) noexcept;
            //copyable
        SymEncryptDecrypt(const SymEncryptDecrypt& obj);
        SymEncryptDecrypt& operator=(const SymEncryptDecrypt& obj);


        void SetKeySize(const int&) ; 
        void SetBlockSize(const int&);        

        void SetParams( const std::unique_ptr<unsigned char>&, const std::unique_ptr<unsigned char>&, const unsigned int&, const unsigned int& );
        void GetParams(std::unique_ptr<unsigned char>&, std::unique_ptr<unsigned char>&, unsigned int&, unsigned int&);


        int aes_encrypt (const std::string&, std::unique_ptr<unsigned char>& ) ; 
        int aes_decrypt (const std::string&, std::unique_ptr<unsigned char>& ) ;         

        

    private:
         std::unique_ptr<SymEncryptionDecryptImpl> m_pImpl ; 

};

// free function interface
std::unique_ptr<unsigned char> KeyGen(std::unique_ptr<unsigned char>&, const unsigned int&, const std::unique_ptr<unsigned char>& , const uint64_t& , const unsigned int& , uint64_t&) ; 

#endif //ifndef __SYMENCRYPTDECRYPT_H__