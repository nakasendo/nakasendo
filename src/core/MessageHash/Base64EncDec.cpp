#include "MessageHash/Base64EncDec.h"
#include "MessageHash/Base64EncDecImpl.h"
#include <iostream>
Base64EncDec::Base64EncDec() : m_pImpl(new Base64EncDecImpl){
    return ; 
}

Base64EncDec::~Base64EncDec(){
    return ; 
}

messagePtr Base64EncDec::encode (const messagePtr& msg, const size_t& len, const int& wrap, int& sizeEncoded){  
    std::unique_ptr<unsigned char[]>  tmpBuffer  = m_pImpl->encode (msg, len, wrap,sizeEncoded); 
    return tmpBuffer;      
}
messagePtr Base64EncDec::decode (const messagePtr& msg, size_t& len, int& strict, int* err ){
     std::unique_ptr<unsigned char[]>  tmpBuffer  = m_pImpl->decode (msg,len,strict, err);
    return tmpBuffer ; 
}





