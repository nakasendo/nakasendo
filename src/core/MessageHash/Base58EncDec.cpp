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