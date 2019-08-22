#ifndef __BASE64_ENCDEC_IMPL_H__
#define __BASE64_ENCDEC_IMPL_H__

#include <string>
#include <memory>

using messagePtr = std::unique_ptr<unsigned char[]> ; 
using charPtr = std::unique_ptr<char[]>;

class Base64EncDecImpl
{
    public:
        explicit Base64EncDecImpl ();
        ~Base64EncDecImpl(); 
        messagePtr encode (const messagePtr&, const size_t&, const int&, int&);
        messagePtr decode (const messagePtr&, const int&, size_t&, int,  int*) ; 

    private:



        Base64EncDecImpl (const Base64EncDecImpl&);
        Base64EncDecImpl& operator= (const Base64EncDecImpl&);
        Base64EncDecImpl(const Base64EncDecImpl&&);
        Base64EncDecImpl& operator= (const Base64EncDecImpl&&);

        unsigned char * enc (unsigned char *, size_t, int, int&);
        unsigned int rawBase64Decode (unsigned char *, unsigned char *, const int&, const int&, int*);

};


#endif //ifndef __BASE64_ENCDEC_IMPL_H__
