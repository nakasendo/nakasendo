#ifndef __BASE58ENCDEC_H__
#define __BASE58ENCDEC_H__

#include <MessageHash/MessageHashConfig.h>
#include <MessageHash/MessageHash.h>

#include <memory>
#include <vector>

class Base58EncDecImpl;

using messagePtr = std::unique_ptr<unsigned char[]> ;
using messageVec = std::vector<uint8_t> ; 

class MessageHash_API Base58EncDec
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

MessageHash_API std::string EncodeBase58(const std::string& msg);

MessageHash_API std::string DecodeBase58(const std::string& msg);

MessageHash_API std::string EncodeBase58Checked(const std::string& msg);

MessageHash_API std::string DecodeBase58Checked(const std::string& msg);

#endif //#ifndef __BASE58ENCDEC_H__
