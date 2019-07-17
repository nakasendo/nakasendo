#ifndef __BASE64ENCDEC_H__
#define __BASE64ENCDEC_H__

#include <MessageHash/MessageHash.h>
#include <memory>

class Base64EncDecImpl ; 
using messagePtr = std::unique_ptr<unsigned char[]> ;


class MessageHash_API Base64EncDec
{
    public: 

        explicit Base64EncDec();
        ~Base64EncDec();     

        messagePtr encode (const messagePtr&, const size_t&, const int&,int& ); 
        messagePtr decode (const messagePtr&, size_t&, int&, int* ) ;
    private:


        Base64EncDec (const Base64EncDec& ) ; 
        Base64EncDec& operator= (const Base64EncDec& ) ; 

        Base64EncDec (const Base64EncDec&& ) ; 
        Base64EncDec& operator= (const Base64EncDec&& ) ; 
        std::unique_ptr<Base64EncDecImpl> m_pImpl ; 
};


#endif //ifndef __BASE64ENCDEC_H__
