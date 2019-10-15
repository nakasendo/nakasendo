#include <string.h>
#include <iostream>
#include <memory>

#include <MessageHash/MessageHashConfig.h>
#include <MessageHash/MessageHash.h>
#include "MessageHash/Base64EncDec.h"
#include "MessageHash/Base58EncDec.h"


void processMsg (const std::string& msgVal, const std::string& encodedValTest){

    for ( int loop=0; loop<10; ++loop){
        std::unique_ptr<unsigned char[]> msgValptr ( new unsigned char[msgVal.size()]);
        int i(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++iter, ++i){
            msgValptr.get()[i] = *iter; 
        }
        std::string encoded = EncodeBase64(msgValptr,msgVal.size());

        //std::cout << encoded << std::endl;
        if (encoded != encodedValTest){
            std::cout << "API ... encode panic \n" << encoded << "\n" <<encodedValTest << "\n" << encoded.length() << "\t" << encodedValTest.length() << std::endl;
        }


        int retLen (0);
        std::string decodedStr; 
        std::unique_ptr<unsigned char[]> decoded = DecodeBase64(encoded, retLen);

        for (int i = 0; i < retLen; ++i){
                decodedStr.push_back(decoded.get()[i]);
        }
        if (decodedStr.length()!= msgVal.length()){
            std::cout << "first api decode panic \n" << decodedStr.length() << " " << msgVal.length() << std::endl;
        }
        if(decodedStr != msgVal){
            std::cout << "second api decode panic \n" << decodedStr << "\n" << msgVal << std::endl ; 
        }
        //if(decodedStr.length() != 0){
        //    std::cout << decodedStr << std::endl;
        //}

    }
}

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

    //std::cout << ListHashFunc () << std::endl ; 

#if 0 
    std::cout << "trying sha224WithRSAEncryption" << std::endl ; 
    std::cout << HashMsg(argv[1], std::string ("sha224WithRSAEncryption")) << std::endl ;
    std::cout << "and md5WithRSAEncryption" << std::endl;    
    std::cout << HashMsg(argv[1], std::string ("md5WithRSAEncryption")) << std::endl ;
    std::cout << "and BLAKE2b512" << std::endl ; 
    std::cout << HashMsg(argv[1], std::string ("BLAKE2b512")) << std::endl ;
#endif
#if 0
    {
        std::string msgVal ("nChain Limited UK branch is fast growing in FinTech industry"); 
        std::unique_ptr<unsigned char[]> msgPtr ( new unsigned char [msgVal.length()]);
        
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ; 
        int sizeEncoded(0);
        std::unique_ptr<unsigned char[]> retValPtr =  encdec.encode (msgPtr, msgVal.size () , 0, sizeEncoded );
        std::string encodedVal;
        for(int i =0; i < sizeEncoded-1; ++i)
        {
            encodedVal.push_back(retValPtr.get()[i]);
        }
        printf("Output (base64): %s\n", retValPtr.get());
        std::string encodedValTest ("bkNoYWluIExpbWl0ZWQgVUsgYnJhbmNoIGlzIGZhc3QgZ3Jvd2luZyBpbiBGaW5UZWNoIGluZHVzdHJ5");

        std::cout << sizeEncoded << " " << encodedValTest.length() << " " << encodedVal.length() << std::endl;
        if ( encodedValTest.length() != encodedVal.length()){
            std::cout << "first panic" << std::endl;
        }
        if(encodedVal != encodedValTest){
            std::cout << "panic ... \n" << encodedVal << "\n" << "bkNoYWluIExpbWl0ZWQgVUsgYnJhbmNoIGlzIGZhc3QgZ3Jvd2luZyBpbiBGaW5UZWNoIGluZHVzdHJ5"<< std::endl;
        }

         std::cout << "decoding..." << std::endl ;
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int; 
        std::unique_ptr<unsigned char[]> decodedValPtr = encdec.decode(retValPtr,value, strict, err);
        printf ("Output(decoded): %s\n", decodedValPtr.get());
    }
#endif
    {
    
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("nChain Limited UK branch is fast growing in FinTech industry"); 
        std::string encodedValTest ("bkNoYWluIExpbWl0ZWQgVUsgYnJhbmNoIGlzIGZhc3QgZ3Jvd2luZyBpbiBGaW5UZWNoIGluZHVzdHJ5");


        processMsg(msgVal, encodedValTest);
    }

    {
    
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("nChain Limited UK branch is fast growing in FinTech industry"); 
        std::string encodedValTest ("bkNoYWluIExpbWl0ZWQgVUsgYnJhbmNoIGlzIGZhc3QgZ3Jvd2luZyBpbiBGaW5UZWNoIGluZHVzdHJ5");


        processMsg(msgVal, encodedValTest);
        
    
    }
    {
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("Development team"); 
        std::string encodedValTest ("RGV2ZWxvcG1lbnQgdGVhbQ==");
        processMsg(msgVal, encodedValTest);
    }
        
    {
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("team"); 
        std::string encodedValTest ("dGVhbQ==");
        processMsg(msgVal, encodedValTest);
        
    }


    {
        std::cout << "And now via the client" << std::endl ;
        std::string msgVal( "Research team");
        std::string encodedValTest ("UmVzZWFyY2ggdGVhbQ==");
        processMsg(msgVal,encodedValTest);
    }

    {
        std::cout << "And now via the client" << std::endl ;
        std::string msgVal( "Research team");
        std::string encodedValTest ("UmVzZWFyY2ggdGVhbQ==");
        processMsg(msgVal,encodedValTest);
    }
#if 0
    {

        std::string msgVal ("Hello World"); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ; 
        int sizeEncoded(0);
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size () + 1, 0, sizeEncoded );
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
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size () + 1, 0,sizeAllocated);
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
        int sizeAllocated; 
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size () + 1, 0,sizeAllocated);
        printf("Output (base64): %s\n", retValPtr.get());

        std::cout << "decoding..." << std::endl ;
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int; 
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(retValPtr,value, strict, err);
        printf ("Output(decoded): %s\n", decodedValPtr.get());
    }
    std::cout << "Base-58 encodingcheck" << std::endl ; 
    {
        std::string msgVal ("The quick brown fox jumped over the lazy dog"); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        std::vector<uint8_t> vec; 
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            vec.push_back(*iter);
            ++ index; 
        }        
        Base58EncDec encdec ; 
        std::string retValPtr =  encdec.encodeCheck(vec);
        std::cout << "Encoded Output (base58-checked): " <<  retValPtr << std::endl ;

        std::vector<uint8_t> decodedVec = encdec.decodeCheck(retValPtr);
        std::string decodedRes;
        for(std::vector<uint8_t>::const_iterator iter = decodedVec.begin(); iter!=decodedVec.end(); ++iter){
            decodedRes.push_back(*iter); 
        }
        std::cout << "Decoded Output (base58-checked): "<< decodedRes << std::endl ;


    }
    {
        std::cout << "my favourite word ... base64" << std::endl ; 
        std::string msgVal ("programmer"); 

        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ;
        int sizeAllocated = 0 ;  
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size (), 0,sizeAllocated);
        printf("Output (base64): %s\n", retValPtr.get());

        std::cout << "decoding..." << std::endl ;
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int; 
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(retValPtr,value, strict, err);
        std::string reval; 
        for (int i=0;i<value;++i){
            reval.push_back(decodedValPtr.get()[i]);
        }
        printf ("Output(decoded): %s\n", reval.c_str());
    } 
    {
        std::cout << "my favourite word ... base64" << std::endl ; 
        std::string msgVal ("programmers"); 

        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ;
        int sizeAllocated = 0 ;  
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size (), 0,sizeAllocated);
        printf("Output (base64): %s\n", retValPtr.get());

        std::cout << "decoding..." << std::endl ;
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int; 
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(retValPtr,value, strict, err);
        std::string reval; 
        for (int i=0;i<value;++i){
            reval.push_back(decodedValPtr.get()[i]);
        }
        printf ("Output(decoded): %s\n", reval.c_str());
    }
    {
        std::cout << "my favourite word ... base64 ... nChain Limited UK branch is fast growing in FinTech industry" << std::endl ; 
        std::string msgVal ("nChain Limited UK branch is fast growing in FinTech industry"); 

        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            ++ index; 
        }        
        Base64EncDec encdec ;
        int sizeAllocated = 0 ;  
        std::unique_ptr<unsigned char> retValPtr =  encdec.encode (msgPtr, msgVal.size (), 0,sizeAllocated);
        printf("Output (base64): %s\n", retValPtr.get());

        std::cout << "decoding..." << std::endl ;
        size_t value = 0;
        int strict = 0 ; 
        int * err = new int; 
        std::unique_ptr<unsigned char> decodedValPtr = encdec.decode(retValPtr,value, strict, err);
        std::string reval; 
        for (int i=0;i<value;++i){
            reval.push_back(decodedValPtr.get()[i]);
        }
        printf ("Output(decoded): %s\n", reval.c_str());
    }              
#if 0 
    {
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("Murphy the monkey and bradley the kinkajou are friends"); 

        std::unique_ptr<unsigned char> msgValptr ( new unsigned char[msgVal.size()]);
        int i(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++iter, ++i){
            msgValptr.get()[i] = *iter; 
        }
        std::string encoded = EncodeBase64(msgValptr.get(),msgVal.size());
        std::cout << encoded << std::endl ;


        std::cout << "And decoding via the client" << std::endl; 
        std::string decoded = DecodeBase64(encoded.c_str());
        std::cout << decoded << std::endl ; 
    }
  

    {
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("the quick brown fox jumped over the lazy dog"); 

        std::unique_ptr<unsigned char> msgValptr ( new unsigned char[msgVal.size()]);
        int i(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++iter, ++i){
            msgValptr.get()[i] = *iter; 
        }
        std::string encoded = EncodeBase64(msgValptr.get(),msgVal.size());
        std::cout << encoded << std::endl ;



        std::cout << "And decoding via the client" << std::endl; 
        std::string decoded = DecodeBase64(encoded.c_str());
        std::cout << decoded << std::endl ; 
    }

    {
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("list"); 

        std::unique_ptr<unsigned char> msgValptr ( new unsigned char[msgVal.size()]);
        int i(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++iter, ++i){
            msgValptr.get()[i] = *iter; 
        }
        std::string encoded = EncodeBase64(msgValptr.get(),msgVal.size());
        std::cout << encoded << std::endl ;


        std::cout << "And decoding via the client" << std::endl; 
        std::string decoded = DecodeBase64(encoded.c_str());
        std::cout << decoded << std::endl ; 
    }

    {
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("Programmer"); 

        std::unique_ptr<unsigned char> msgValptr ( new unsigned char[msgVal.size()]);
        int i(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++iter, ++i){
            msgValptr.get()[i] = *iter; 
        }
        std::string encoded = EncodeBase64(msgValptr.get(),msgVal.size());
        std::cout << encoded << std::endl ;


        std::cout << "And decoding via the client" << std::endl; 
        std::string decoded = DecodeBase64(encoded.c_str());
        std::cout << decoded << std::endl ; 
    }

    std::cout << "And now for base-58" << std::endl ; 
    {
        std::string msgVal ("Murphy the monkey and bradley the kinkajou are friends"); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        std::vector<uint8_t> vec; 
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            vec.push_back(*iter);
            ++ index; 
        }        
        Base58EncDec encdec ; 
        std::string retValPtr =  encdec.encode (vec);
        
        std::cout << "Output (base58): " <<  retValPtr << std::endl ;
        std::cout << "And now decoding..." << std::endl ; 
        std::vector<uint8_t> decodedVec = encdec.decode(retValPtr);
        std::string decodedRes;
        for(std::vector<uint8_t>::const_iterator iter = decodedVec.begin(); iter!=decodedVec.end(); ++iter){
            decodedRes.push_back(*iter); 
        }
        std::cout << decodedRes << std::endl ; 
    }

    {
        std::cout << "And now via the client" << std::endl ; 
        std::string msgVal ("Programmer"); 

        std::string encoded = EncodeBase58(msgVal);
        std::cout << encoded << std::endl ;


        std::cout << "And decoding via the client" << std::endl; 
        std::string decoded = DecodeBase58(encoded);
        std::cout << decoded << std::endl ; 
    }

    std::cout << "Base-58 encodingcheck" << std::endl ; 
    {
        std::string msgVal ("Programmer"); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        std::vector<uint8_t> vec; 
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            vec.push_back(*iter);
            ++ index; 
        }        
        Base58EncDec encdec ; 
        std::string retValPtr =  encdec.encodeCheck(vec);
        std::cout << "Encoded Output (base58-checked): " <<  retValPtr << std::endl ;

        std::vector<uint8_t> decodedVec = encdec.decodeCheck(retValPtr);
        std::string decodedRes;
        for(std::vector<uint8_t>::const_iterator iter = decodedVec.begin(); iter!=decodedVec.end(); ++iter){
            decodedRes.push_back(*iter); 
        }
        std::cout << "Decoded Output (base58-checked): "<< decodedRes << std::endl ;


    }

       std::cout << "Base-58 encodingcheck" << std::endl ; 
    {
        std::string msgVal ("The quick brown fox jumped over the lazy dog"); 
        std::unique_ptr<unsigned char> msgPtr ( new unsigned char [msgVal.size() + 1 ]);
        std::vector<uint8_t> vec; 
        int index(0);
        for (std::string::const_iterator iter = msgVal.begin(); iter != msgVal.end(); ++ iter){
            msgPtr.get()[index] = *iter ; 
            vec.push_back(*iter);
            ++ index; 
        }        
        Base58EncDec encdec ; 
        std::string retValPtr =  encdec.encodeCheck(vec);
        std::cout << "Encoded Output (base58-checked): " <<  retValPtr << std::endl ;

        std::vector<uint8_t> decodedVec = encdec.decodeCheck(retValPtr);
        std::string decodedRes;
        for(std::vector<uint8_t>::const_iterator iter = decodedVec.begin(); iter!=decodedVec.end(); ++iter){
            decodedRes.push_back(*iter); 
        }
        std::cout << "Decoded Output (base58-checked): "<< decodedRes << std::endl ;


    }
    {
        std::string msgVal ("Hello World");  

        std::string encoded = EncodeBase64(msgVal.c_str());
        std::cout << encoded << std::endl ;


        std::cout << "And decoding via the client" << std::endl; 
        std::string decoded = DecodeBase64(encoded.c_str());
        std::cout << decoded << std::endl ; 


    }
#endif    
#endif
    return 0 ; 
    
}
