#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include "ECPoint/ECPoint.h"
#include "BigNumbers/BigNumbers.h"
#include "MessageHash/conversions.h"
#include "MessageHash/MessageHash.h"
#include "AsymKey/AsymKey.h"
#include "BCHAddress/BCHAddress.h"
#include <SecretSplit/KeyShare.h>


int main(int argc,char * argv[]){


    
	
	
	
    AsymKey myKey ; 
    
    
    std::cout << myKey.exportPrivateHEX () << std::endl; 
    
    
    std::string msg("I promise that I'm an invoice ... hahah");
    
    
    AsymKey derivedKey = myKey.derive(msg); 
    
     
    
    std::cout << derivedKey.exportPrivateHEX() << std::endl;
    
    
    
     std::vector<KeyShare> shares = myKey.split(4,5);
     
     
     
     for (std::vector<KeyShare>::const_iterator iter = shares.begin(); iter != shares.end(); ++ iter){
     
        std::cout << keyshare_to_json(*iter) << std::endl;
     }
     
     
     
     // choice 4 of the 5 shares
     
     //choose random 4 out of 5 shares
     
     
     std::cout << "Choose 4 key shares randomly" << std::endl; 
     std::vector<int> numbers; 
     while (numbers.size() < 4){
        int num = rand() % 5; 

        if(std::find(numbers.begin(), numbers.end(), num) == numbers.end()){
            numbers.push_back(num); 
        }
     }
     
     std::vector<KeyShare> randomshares; 
     for(std::vector<int>::const_iterator iter = numbers.begin(); iter != numbers.end(); ++ iter){
        randomshares.push_back(shares[*iter]); 
     }
     
     
     for (std::vector<KeyShare>::const_iterator iter = randomshares.begin(); iter != randomshares.end(); ++ iter){
     
        std::cout << keyshare_to_json(*iter) << std::endl;
     }
     
     AsymKey newKey;
     newKey.recover(randomshares); 
     
     
     std::cout << newKey.exportPrivateHEX() << std::endl;
     
     AsymKey newDerivedKey = newKey.derive(msg) ;
     
     
     std::cout << newDerivedKey.exportPrivateHEX() << std::endl;
     
     std::vector<KeyShare> notEnoughShares;
     for(int i=0;i<3;++i){
        notEnoughShares.push_back(shares[i]);
     }
    
    
    AsymKey newBrokenKey;
    newBrokenKey.recover (notEnoughShares);
    std::cout << newBrokenKey.exportPrivateHEX() << std::endl;
    
    return 0; 
}
