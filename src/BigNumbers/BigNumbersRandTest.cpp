#include "BigNumbers.h"
#include "BigNumAPI.h"
#include <iostream>
#include <memory>


#if 0 
Sum = c590e57ee64fcef321395bba088ca0a867e1e85a1ea77478f8783e6a6cf8f3e582bff83cb2d7d9fd549fcbb40dea22ac140351007030059500bdca81413600e9
A = c590e57ee64fced18aff6e2f0c6ac05625b1e94f394f42470cae14d12cadea4f5ab6b9d77225fe3b4903825966c78752ae51b6a0a2caca555fd0ffcbd9704b01
B = 219639ed8afc21e052422fff0ae5583231ebca2999404b099628093e6540b1dbc20b9c495aa7229b5965b19a5fcd653b3fa0eccab567c5b5e8
#endif

int main (int argc, char**argv)
{
  
	if (argc != 4)
	{
		std::cout << "<BigNumRandTest <numofrandomNumbers> <lowerLimit> <upperlimit>"
				<< std::endl ; 
		return 1; 
	}	
	int nNumberOfRand=atoi(argv[1]);
	std::string lowerLimit (argv[2]);
	std::string upperLimit(argv[3]); 
	
	BigNumber valLower; 
	BigNumber valUpper; 

	valLower.FromDec(lowerLimit); 
	valUpper.FromDec(upperLimit); 

	for (int i=0; i<nNumberOfRand; ++i)
	{
		BigNumber randRange = GenerateRandRange (valLower, valUpper);
        std::cout << i << "," << randRange.ToDec() << std::endl;


        // check 
        if ( randRange < valLower || randRange > valUpper)
        {
            std::cout << "success on random range" << std::endl ;
        }
	}


   
    return 0; 
}
