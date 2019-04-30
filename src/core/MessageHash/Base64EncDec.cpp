#include "Base64EncDec.h"
#include "Base64EncDecImpl.h"

Base64EncDec::Base64EncDec() : m_pImpl(new Base64EncDecImpl){
    return ; 
}

Base64EncDec::~Base64EncDec(){
    return ; 
}

messagePtr Base64EncDec::encode (messagePtr& msg, const size_t& len, const int& wrap){  
    std::unique_ptr<unsigned char>  tmpBuffer (m_pImpl->encode (msg, len, wrap)); 
    return std::move(tmpBuffer);      
}
messagePtr Base64EncDec::decode (messagePtr& msg, size_t& len, int& strict, int* err ){
     std::unique_ptr<unsigned char>  tmpBuffer (m_pImpl->decode (msg,len,strict, err));
    return std::move (tmpBuffer) ; 
}





