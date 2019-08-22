#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>


#include "MessageHash/Base64EncDecImpl.h"


namespace Base64{
    constexpr char b64table[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d' \
                                ,'e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};

    char b64revt[256] = {
        -3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*0-15*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*16-31*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63, /*32-47*/
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-2,-1,-1, /*46-63*/
        -1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14, /*64-79*/
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1, /*80-95*/
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40, /*96-111*/
        41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1, /*112-127*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*128-143*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*144-159*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*160-175*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*176-191*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*192-207*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*208-223*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*224-239*/
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, /*240-255*/
    };
};

Base64EncDecImpl::Base64EncDecImpl(){
    return ; 
}

Base64EncDecImpl::~Base64EncDecImpl(){
    return ; 
}

unsigned char * Base64EncDecImpl::enc (unsigned char * input, size_t len, int wrap, int& sizeAllocated)
{
    size_t i=0, toalloc=0;
    size_t mod = len % 3; 

    toalloc = (len / 3) * 4 + ( 3 - mod) % 3 + 1 ; 
    if (wrap){
        toalloc += len/57 ; 
        if (len%57) toalloc++ ; 
    }
    unsigned char * p , * output ;
    sizeAllocated = ((len/3) + (mod ? 1 : 0)) * 4 +1;
    p = output = new unsigned char[sizeAllocated];
    
    while ( i < (len-mod)){
        *p++ = Base64::b64table[input[i++] >> 2]; 
        *p++ = Base64::b64table[((input[i-1] << 4) | (input[i] >> 4))& 0x3f];
        *p++ = Base64::b64table[((input[i] << 2) | (input[i+1] >> 6))& 0x3f];
        *p++ = Base64::b64table[input[i+1] & 0x3f];
        i += 2 ; 
        if (wrap && !(i%57)) *p++ = '\n'; 
    }

    if (!mod){
        if(wrap && i % 57) *p++ = '\n'; 
        *p = 0;
        return output; 
    }
    else{        
        
        *p++ = Base64::b64table[input[i++] >> 2]; 
        if(i == len){
            *p++ = Base64::b64table[(input[i-1] << 4)& 0x3f];
        }else{
            *p++ = Base64::b64table[((input[i-1] << 4) | (input[i] >> 4)) & 0x3f]; 
        }
        if(mod == 1){
            *p++ = '='; 
            *p++ = '=';
            if(wrap) *p++ = '\n'; 
            *p = 0 ; 
            return output; 
        }
        else{
            *p++ = Base64::b64table[(input[i] << 2) & 0x3f];
            *p++ = '='; 
            if(wrap) *p++ = '\n';
            *p=0; 
            return output;           
        }
    }

}

unsigned int Base64EncDecImpl::rawBase64Decode (unsigned char * in, unsigned char * out, const int& inputlen, const int& strict, int* err){
    unsigned int result=0;
    int x = 0; 
    unsigned char buf[3],*p = in, pad = 0; 

    *err = 0 ; 
	while(!pad){

        switch ((x = Base64::b64revt[*p++])){
            case -3:    // Null terminator
                if(((p-1) - in) % 4 ) *err = 1 ; 
                return result ; 
            case -2:    // padding character .. invalid here
                if (((p-1) -in) % 4 < 2){
                    *err = 1 ;
                    return result; 
                }else if(((p-1) - in) % 4 ==2){
                    // Make sure there is approproiate padding
                    if(*p != '='){
                        *err = 1;
                        return result;
                    }
                    buf[2] = 0;
                    pad = 2;
                    result++;
                    break;
                }else{
                    pad = 1;
                    result += 2; 
                    break;
                }
                return result;
            case -1:
                if(strict){
                    *err =2 ;
                    return result;
                }
                break;
            default:
                switch (((p-1) - in) %4 ){
                    case 0:
                        buf[0] = x << 2; 
                        break;
                    case 1:
                        buf[0] |= (x >> 4) ;
                        buf[1] = x << 4;
                        break;
                    case 2:
                        buf[1] |= ( x >> 2);
                        buf[2] = x << 6 ; 
                        break;
                    case 3:
                        buf[2] |= x ; 
                        result += 3; 
                        for (x = 0; x < 3-pad; x++) *out++ = buf[x]; 
                        break; 
                } // end of default switch statment
        }// end of switch statement
    }
    for(x = 0; x < 3 - pad; x++) *out++ = buf[x];
    return result;
}

messagePtr Base64EncDecImpl::encode (const messagePtr& buf, const size_t& len, const int& wrap, int& sizeEncoded){    
    int sizeAllocated=0;
    unsigned char * retValPtr =  enc (buf.get(), len, wrap, sizeAllocated);
    if (sizeAllocated>0){
        std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [sizeAllocated]);
        for (int i=0; i<sizeAllocated;++i){
            msgPtr.get()[i]=retValPtr[i];
        }        
        delete [] retValPtr;
        sizeEncoded = sizeAllocated-1; 
        return msgPtr;
    }else{
        sizeEncoded = sizeAllocated; 
        delete [] retValPtr;
        return nullptr;
    }     
}

messagePtr Base64EncDecImpl::decode (const messagePtr& buf, const int& inputlen, size_t& len, int strict, int* err){         
       ///
    unsigned char * outbuf = new unsigned char[3 * inputlen / 4 + 1];
    if (!outbuf){
        *err = 3 ; 
        len = 0;
        return nullptr;
    }

    len = rawBase64Decode(buf.get(), outbuf,inputlen, strict, err);
    if(*err){        
        delete [] outbuf;
        len = 0; 
        return nullptr;
    }

       ///
       if (len == 0){
           delete [] outbuf;
           return nullptr; 
       }else{
           
           messagePtr msgPtr(new unsigned char[len]);
           for (unsigned int i=0;i<len;++i){
               msgPtr.get()[i]= outbuf[i];
           }
           delete [] outbuf;
           return msgPtr;
       }
}


