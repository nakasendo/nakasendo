#include <stdio.h>
#include <sstream>
#include <numeric>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <Polynomial/Polynomial.h>

#include <BigNumbers/BigNumbers.h>
#include <Polynomial/LGInterpolator.h>
#include <SecretSplit/KeyShare.h>
#include <SecretSplit/SecretSplit.h>




//g++ -o interp -I. -I../../core -I../../../../build/generated_hpp LaGrangeInterp.cpp  -std=c++17 -L/Users/j.murphy/nchain/SDK-NAME-CHANGE/build/x64/release -lBigNumbers -lPolynomial


#if 0 
            std::regex reg("\\s+");

            std::sregex_token_iterator iter(inputval.begin(), inputval.end(), reg, -1);
            std::sregex_token_iterator end;

            std::vector<std::string> vec(iter, end);
           
            std::cout << "value from file: " << inputval << "\tsize of vec " << vec.size() << std::endl;
            if (vec.size() != 2){
                std::cout << "invalid data in the shares file" << std::endl ; 
                return -1;
            }
            BigNumber xval; 
            xval.FromHex (vec.at(0)); 
            BigNumber yval;
            yval.FromHex (vec.at(1)); 
            
            ShareSample.push_back (std::make_pair(xval, yval)); 
            /////
#endif

int main (int argc, char * argv[])
{
    // random args pulled from the python code
    
    
    BigNumber mod; 
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");  
    
    
    if ( argc != 2 ){
        std::cout << "No share file sup[plied ... bombing" << std::endl;
        return -1;
    }
    
    
    std::vector<KeyShare> ShareSample ; 
    std::ifstream inSharesStr; 
    inSharesStr.open (argv[1] ); 
    if (inSharesStr.is_open()){
        while (!inSharesStr.eof()){
            std::string inputval;
            std::getline ( inSharesStr, inputval);
            /////
            if ( inputval.find_first_not_of (" \t\n\f\r" )){
                continue;
            }
            
            KeyShare share = keyshare_from_json (inputval);
            ShareSample.push_back(share);

        }
    }else{
        std::cout << "Unable to open shares file..." << std::endl;
        return -1;
    }
    

    std::cout << "Secret recovered from: " << std::endl; 
    for(std::vector<KeyShare>::const_iterator iter = ShareSample.begin(); iter != ShareSample.end(); ++ iter){
         std::cout << keyshare_to_json(*iter) << std::endl ;
    }
    BigNumber secret; 
    secret.Zero(); 
    secret = RecoverSecret(ShareSample, mod); 
    std::cout << secret.ToDec() << std::endl; 
    return 0 ; 
}

