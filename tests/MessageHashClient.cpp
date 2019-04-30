#include <string.h>
#include <iostream>
#include <memory>

#include <MessageHash/MessageHash.h>
#include <MessageHash/MessageHashAPI.h>
#include "MessageHash/Base64EncDec.h"

int main(int argc,char** argv)
{
    if(!argv[1]){
        std::cout << "No Message supplied" << std::endl; 
        return 1; 
    }


    // call the default sha256 

    MessageHash hashMess; 

    

    if(!argv[2])
    {

        hashMess.HashSha256 (argv[1]);
        std::cout << "Calling printable hash" << std::endl ; 
        hashMess.printHash () ; 
        std::cout << std::endl ;
        std::cout << hashMess.HashHex () << std::endl ; 
                
    }
    else
    {
        //std::unique_ptr<unsigned char> message = HashMsg(argv[1], argv[2]);
        //std::cout << message.get () << std::endl ; 
    }


    std::cout << "And now via the API" << std::endl; 

    std::cout << HashMsgSHA256 (argv[1]) << std::endl ; 

    std::cout << ListHashFunc () << std::endl ; 

#if 0 
    std::cout << "trying sha224WithRSAEncryption" << std::endl ; 
    std::cout << HashMsg(argv[1], std::string ("sha224WithRSAEncryption")) << std::endl ;
    std::cout << "and md5WithRSAEncryption" << std::endl;    
    std::cout << HashMsg(argv[1], std::string ("md5WithRSAEncryption")) << std::endl ;
    std::cout << "and BLAKE2b512" << std::endl ; 
    std::cout << HashMsg(argv[1], std::string ("BLAKE2b512")) << std::endl ;
#endif


    {

        std::string msgVal ("Hello World"); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ; 
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size () + 1, 0 );
        printf("Output (base64): %s\n", retValPtr.get());

        std::cout << "decoding..." << std::endl ;
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int; 
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(retValPtr,value, strict, err);
        printf ("Output(decoded): %s\n", decodedValPtr.get());

    }

    {

        std::string msgVal ("And bradley"); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ;
        int sizeAllocated = 0 ;  
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size () + 1, 0);
        printf("Output (base64): %s\n", retValPtr.get());

        std::cout << "decoding..." << std::endl ;
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int; 
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(retValPtr,value, strict, err);
        printf ("Output(decoded): %s\n", decodedValPtr.get());
    }    

    {

        std::string msgVal ("Murphy the monkey and bradley the kinkajou are friends"); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ; 
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size () + 1, 0 );
        printf("Output (base64): %s\n", retValPtr.get());

        std::cout << "decoding..." << std::endl ;
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int; 
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(retValPtr,value, strict, err);
        printf ("Output(decoded): %s\n", decodedValPtr.get());
    }

    {
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("Murphy the monkey and bradley the kinkajou are friends"); 

        std::string encoded = EncodeBase64(msgVal);
        std::cout << encoded << std::endl ;


        std::cout << "And decoding via the client" << std::endl; 
        std::string decoded = DecodeBase64(encoded);
        std::cout << decoded << std::endl ; 
    }
  
    return 0 ; 
    
}
