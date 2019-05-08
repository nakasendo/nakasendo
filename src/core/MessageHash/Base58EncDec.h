#ifndef __BASE58ENCDEC_H__
#define __BASE58ENCDEC_H__

#include <memory>
#include <vector>

class Base58EncDecImpl;

using messagePtr = std::unique_ptr<unsigned char> ;
using messageVec = std::vector<uint8_t> ; 

class Base58EncDec
{
    public:
        explicit Base58EncDec();
        ~Base58EncDec();
        std::string encode (const messageVec &);
        std::string encodeCheck (const messageVec&); 

        messageVec decode (const std::string&);
        messageVec decodeCheck(const std::string&);

    private:

        Base58EncDec (const Base58EncDec& ) ; 
        Base58EncDec& operator= (const Base58EncDec& ) ; 

        Base58EncDec (const Base58EncDec&& ) ; 
        Base58EncDec& operator= (const Base58EncDec&& ) ; 

        
        std::unique_ptr<Base58EncDecImpl> m_pImpl ; 
};


#endif //#ifndef __BASE58ENCDEC_H__
