#ifndef __BASE58ENCDECIMPL_H__
#define __BASE58ENCDECIMPL_H__

#include <string>
#include <memory>


using messagePtr = std::unique_ptr<unsigned char[]> ;
using messageVec = std::vector<uint8_t> ; 

class Base58EncDecImpl
{
    public:
        explicit Base58EncDecImpl ();
        ~Base58EncDecImpl()=default; 
        std::string encode (const messageVec&);
        std::string encodeCheck (const messageVec&) ; 

        messageVec decode (const std::string&);
        messageVec decodeCheck(const std::string&);

    private:



        Base58EncDecImpl (const Base58EncDecImpl&);
        Base58EncDecImpl& operator= (const Base58EncDecImpl&);
        Base58EncDecImpl(const Base58EncDecImpl&&);
        Base58EncDecImpl& operator= (const Base58EncDecImpl&&);

        std::string EncodeBase58(const uint8_t*, const uint8_t*);
        std::vector<uint8_t> DecodeBase58(const char *);
};


#endif //#ifndef __BASE58ENCDECIMPL_H__
