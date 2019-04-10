#include <string.h>
#include <iostream>
#include <memory>
#include "MessageHash.h"
#include "MessageHashAPI.h"

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


    std::cout << "trying sha224WithRSAEncryption" << std::endl ; 
    std::cout << HashMsg(argv[1], std::string ("sha224WithRSAEncryption")) << std::endl ;
    std::cout << "and md5WithRSAEncryption" << std::endl;    
    std::cout << HashMsg(argv[1], std::string ("md5WithRSAEncryption")) << std::endl ;
    std::cout << "and BLAKE2b512" << std::endl ; 
    std::cout << HashMsg(argv[1], std::string ("BLAKE2b512")) << std::endl ;
    return 0 ; 
    
}