#include "MessageHash/Base58EncDec.h"
#include "MessageHash/Base58EncDecImpl.h"

Base58EncDec::Base58EncDec() : m_pImpl(new Base58EncDecImpl){
    return ; 
}

Base58EncDec::~Base58EncDec(){
    return ; 
}

std::string Base58EncDec::encode (const std::vector<uint8_t>& vch){
    return (m_pImpl->encode(vch));
}
std::string Base58EncDec::encodeCheck (const std::vector<uint8_t>& vch){
    return (m_pImpl->encodeCheck(vch));
}

messageVec Base58EncDec::decode (const std::string& msg){
    return (m_pImpl->decode(msg));
}
messageVec Base58EncDec::decodeCheck(const std::string& msg){
    return (m_pImpl->decodeCheck(msg));
}

std::string EncodeBase58 (const std::string& msg)
{
    std::vector<uint8_t> vec;
    for (std::string::const_iterator iter = msg.begin(); iter != msg.end(); ++ iter)
    {
        vec.push_back(*iter);
    }
    Base58EncDec encdec ;
    std::string  encVal =  encdec.encode (vec);
    return encVal ;
}

std::string DecodeBase58 (const std::string& msg)
{       
    std::string nonConstMsg ( msg );
    nonConstMsg = nonConstMsg.erase(nonConstMsg.find_last_not_of("\t\n\v\f\r ")+1);

    std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [nonConstMsg.length()+1]); 
    std::fill_n(msgPtr.get(), msg.length()+1, 0x00);
    std::string::const_iterator iter = nonConstMsg.begin();

    for (unsigned int i = 0; i < nonConstMsg.size();++i)
    {
        msgPtr.get()[i] = *iter ; ++ iter ; 
    }        
    Base58EncDec encdec;
    std::vector<uint8_t> decodedVal = encdec.decode(nonConstMsg);
    std::string retVal;
    for(std::vector<uint8_t>::const_iterator iter = decodedVal.begin();iter != decodedVal.end(); ++ iter)
    {
        retVal.push_back(*iter);
    } 
    return retVal ;
}    


std::string EncodeBase58Checked (const std::string& msg)
{
    std::vector<uint8_t> vec;
    for (std::string::const_iterator iter = msg.begin(); iter != msg.end(); ++ iter)
    {
        vec.push_back(*iter);
    }
    Base58EncDec encdec ;
    std::string  encVal =  encdec.encodeCheck (vec);
    return encVal ;
}

std::string DecodeBase58Checked (const std::string& msg)
{
    std::string nonConstMsg ( msg );
    nonConstMsg = nonConstMsg.erase(nonConstMsg.find_last_not_of("\t\n\v\f\r ")+1);

    std::unique_ptr<unsigned char> msgPtr ( new unsigned char [nonConstMsg.length()+1]);
    std::fill_n(msgPtr.get(), msg.length()+1, 0x00);
    std::string::const_iterator iter = nonConstMsg.begin();
    for (unsigned int i = 0; i < nonConstMsg.size();++i)
    {
        msgPtr.get()[i] = *iter ; ++ iter ; 
    }        
    Base58EncDec encdec;
    std::vector<uint8_t> decodedVal = encdec.decodeCheck(nonConstMsg);
    std::string retVal;
    for(std::vector<uint8_t>::const_iterator iter = decodedVal.begin();iter != decodedVal.end(); ++ iter)
    {
        retVal.push_back(*iter);
    } 
    return retVal ; 
}