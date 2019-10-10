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
messagePtr Base64EncDec::decode (const messagePtr& msg, const int& inputlen, size_t& len, int& strict, int* err ){
     std::unique_ptr<unsigned char[]>  tmpBuffer  = m_pImpl->decode (msg, inputlen,len,strict, err);
    return tmpBuffer ; 
}


std::string EncodeBase64 (const std::unique_ptr<unsigned char[]>& msgPtrApi, int msgSize)    
{  
    Base64EncDec encdec;
    int sizeEncodedBuffer(0);
    std::string retVal;
    std::unique_ptr<unsigned char[]> retValPtr =  encdec.encode (msgPtrApi, msgSize, 0, sizeEncodedBuffer);

    for(int i=0;i<sizeEncodedBuffer;++i)
    {
        retVal.push_back(retValPtr.get()[i]);
    }

    return retVal;
}


std::unique_ptr<unsigned char[]>  DecodeBase64 (const std::string& msgPtrApi, int& msgSize )
{
    size_t value = 0;
    int strict = 0 ;
    int * err = new int;

    std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [msgPtrApi.size()+1]);
    std::fill_n(msgPtr.get(), msgPtrApi.size()+1, 0x00);
    int i(0);
    for(std::string::const_iterator iter = msgPtrApi.begin(); iter != msgPtrApi.end(); ++iter)
    {
        msgPtr.get()[i++] = *iter;
    }

    Base64EncDec encdec;
    std::unique_ptr<unsigned char[]> decodedValPtr = encdec.decode(msgPtr, msgPtrApi.size(), value, strict, err);
    msgSize = static_cast<int> (value);

    return decodedValPtr;     
}


