#include <iostream>
#include <vector>
#include <MessageHashAPI.h>

int main(int argc, char* argv[]){
std::cout << "CUNTS!!!!!!" << std::endl;

        std::string msgVal( "Research team");
        std::string encodedValTest ("UmVzZWFyY2ggdGVhbQ==");
        std::vector<uint8_t> input;
        for(std::string::const_iterator iter = msgVal.begin();iter != msgVal.end(); ++iter){
            input.push_back(*iter);
        }
        
        std::vector<uint8_t> output = EncodeBase64Ex(input);
        std::cout << output.size() << std::endl;
        for(std::vector<uint8_t>::const_iterator iter = output.begin(); iter != output.end(); ++ iter){
            std::cout << *iter;
        }
        std::cout << std::endl; 
        std::vector<uint8_t> decodout = DecodeBase64Ex(output);
        
    for(std::vector<uint8_t>::const_iterator iter = decodout.begin(); iter != decodout.end(); ++iter){
        std::cout << *iter ;
    }
    std::cout << std::endl;
}
