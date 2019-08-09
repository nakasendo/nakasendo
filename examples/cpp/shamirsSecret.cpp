#include <stdio.h>
#include <sstream>
#include <numeric>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <Polynomial/Polynomial.h>

#include <BigNumbers/BigNumbers.h>
#include <MessageHash/MessageHash.h>
#include <Polynomial/LGInterpolator.h>
#include <SecretSplit/KeyShare.h>
#include <SecretSplit/SecretSplit.h>



//g++ -o interp -I. -I../../core -I../../../../build/generated_hpp LaGrangeInterp.cpp  -std=c++17 -L/Users/j.murphy/nchain/SDK-NAME-CHANGE/build/x64/release -lBigNumbers -lPolynomial


// check out bitslicing for hiding the index in the number. (look at the sss implementation)
// the size of the polynomial in our implementation must be 1 less than the minium number required 
// to recover the shares

// Adapt to use the KeyShare class
// put in the examples directory

int main (int argc, char * argv[])
{
    // random args pulled from the python code
    int degree = 2;
    int threshold = 3;
    int maxshares = 6;
    
    int numberShareCombinations = 1 ; 
    std::string outfileName ("outshares" ) ; 
    
    BigNumber mod; 
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");  
    
    
    if (argc ==2 ){
        threshold = atoi(argv[1]);
        degree = threshold-1;
    }
    
    if (argc == 3){
        threshold = atoi(argv[1]);
        degree = threshold-1;
        maxshares = atoi(argv[2]);
    }
    
    if (argc == 4){
        threshold = atoi(argv[1]);
        degree = threshold-1;
        maxshares = atoi(argv[2]);
        numberShareCombinations = atoi(argv[3]);
    }
    
    // a randomly chosen large number for our secret
    
    BigNumber zero ;
    zero.Zero(); 
    BigNumber theSecret = GenerateRandRange (zero, mod, 512);
    if (argc == 5){
        threshold = atoi(argv[1]);
        degree = threshold-1;
        maxshares = atoi(argv[2]);
        numberShareCombinations = atoi(argv[3]);
        theSecret.FromDec(argv[4]);
    }
    
    if ( threshold > maxshares ){
        std::cout << "pool secret would be recoverable ... muppet" << std::endl; 
        return -1;
    }
    
    
    
    Polynomial poly(degree, mod, theSecret);
    std::cout << poly << std::endl ;
    std::cout << "The Secret before splitting: " << theSecret.ToDec() << std::endl;
    
    std::vector<KeyShare> shares = make_shared_secret (poly, threshold, maxshares);
    
    for (int i=0; i < numberShareCombinations; ++i){
        std::vector<KeyShare> shareSample;
        std::set<int> chosenNums; 
        
        while (shareSample.size () < threshold ){
            int index = rand() % (shares.size()-1) ; 
            if ( chosenNums.find(index) == chosenNums.end()){
                chosenNums.insert(index);
                shareSample.push_back(shares.at(index)); 
            }
        }
        
        //write to a file
        std::string filename (outfileName); 
        filename += "_"; 
        filename += std::to_string(i); 
        filename += ".txt";
        std::ofstream filenameStream;
        filenameStream.open(filename);
        if ( filenameStream.is_open()){
            for(std::vector<KeyShare>::const_iterator iter = shareSample.begin(); iter != shareSample.end(); ++iter){
                filenameStream << keyshare_to_json(*iter) << std::endl;
            }
            filenameStream.close(); 
        }else{
            std::cout << "Unable to open file: " << filename << std::endl; 
        }
        
    }
    
   
    return 0 ; 
}
