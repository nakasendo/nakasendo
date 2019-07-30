#define BOOST_TEST_MODULE test_Polynomial

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <BigNumbers/BigNumbers.h>
#include <Polynomial/Polynomial.h>
#include <Polynomial/LGInterpolator.h>
#include <vector>

#include <string>
#include <iostream>

std::vector<BigNumber> getVectorBNX(const int& nbPoint){
    std::vector<BigNumber> VectorX; 
    for (int i=0;i<nbPoint;++i){
        BigNumber num ; 
        num.One();
        BigNumber secondNum;
        secondNum.FromDec(std::to_string(i));
        num = num + secondNum;
        VectorX.push_back(num);
    }
    return VectorX;
}

BOOST_AUTO_TEST_SUITE( test_suite_Interpolation )


/* Create a new polynomial using the string constructor
 *   Check the degree
 *   Evaluate the point for x=0,1
 *   Check the coefficients
 */
BOOST_AUTO_TEST_CASE( test_Interpolation_degree_3_mod_17 )
{
    int degree = 3;
    BigNumber mod; 
    mod.FromDec("17"); 

    Polynomial poly(degree, mod);
    std::vector<BigNumber> fx; 

    int margin = 2 ;
    int npPoint = degree + 1 + margin;

    std::vector<BigNumber> x = getVectorBNX(npPoint);
    
    for (std::vector<BigNumber>::const_iterator iter = x.begin(); iter !=x.end(); ++ iter){
        fx.push_back(poly(*iter));
    }


    std::vector<std::pair<BigNumber, BigNumber> > xfx; 
    std::vector<BigNumber>::const_iterator xIter = x.begin(), fxIter = fx.begin (); 
    for(; xIter != x.end(); ++xIter, ++fxIter){
        xfx.push_back ( std::pair(*xIter, *fxIter)); 
    }

    // Pick a number with in the Mod range.
    BigNumber zero;
    zero.Zero(); 
    BigNumber xValue = GenerateRandRange(zero, mod); 

    LGInterpolator interpFunc(xfx, mod);

     for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator testIter = xfx.begin(); testIter != xfx.end(); ++ testIter){
         try{
            BigNumber TestVal = interpFunc(testIter->first);
            BOOST_TEST (TestVal.ToDec() == testIter->second.ToDec());
        }
        catch(std::exception& e){
            std::cout << e.what() << std::endl;
        }
     }

}

BOOST_AUTO_TEST_CASE( test_Interpolation_degree_100_mod_104729 )
{
    // degree 100 & a mod of 104729 ( the 10000th prime)
    int degree = 100;
    BigNumber mod; 
    mod.FromDec("104729"); 

    Polynomial poly(degree, mod);
    std::vector<BigNumber> fx; 

    int margin = 2 ;
    int npPoint = degree + 1 + margin;

    std::vector<BigNumber> x = getVectorBNX(npPoint);
    
    for (std::vector<BigNumber>::const_iterator iter = x.begin(); iter !=x.end(); ++ iter){
        fx.push_back(poly(*iter));
    }


    std::vector<std::pair<BigNumber, BigNumber> > xfx; 
    std::vector<BigNumber>::const_iterator xIter = x.begin(), fxIter = fx.begin (); 
    for(; xIter != x.end(); ++xIter, ++fxIter){
        xfx.push_back ( std::pair(*xIter, *fxIter)); 
    }

    // Pick a number with in the Mod range.
    BigNumber zero;
    zero.Zero(); 
    BigNumber xValue = GenerateRandRange(zero, mod); 

    LGInterpolator interpFunc(xfx, mod);

     for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator testIter = xfx.begin(); testIter != xfx.end(); ++ testIter){
         try{
            BigNumber TestVal = interpFunc(testIter->first);
            BOOST_TEST (TestVal.ToDec() == testIter->second.ToDec());
        }
        catch(std::exception& e){
            std::cout << e.what() << std::endl;
        }
     }

}
BOOST_AUTO_TEST_CASE( test_Interpolation_degree_200_mod_SECP256K1MOD )
{
    int degree = 200;
    BigNumber mod; 
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"); 

    Polynomial poly(degree, mod);
    std::vector<BigNumber> fx; 

    int margin = 2 ;
    int npPoint = degree + 1 + margin;

    std::vector<BigNumber> x = getVectorBNX(npPoint);
    
    for (std::vector<BigNumber>::const_iterator iter = x.begin(); iter !=x.end(); ++ iter){
        fx.push_back(poly(*iter));
    }


    std::vector<std::pair<BigNumber, BigNumber> > xfx; 
    std::vector<BigNumber>::const_iterator xIter = x.begin(), fxIter = fx.begin (); 
    for(; xIter != x.end(); ++xIter, ++fxIter){
        xfx.push_back ( std::pair(*xIter, *fxIter)); 
    }

    // Pick a number with in the Mod range.
    BigNumber zero;
    zero.Zero(); 
    BigNumber xValue = GenerateRandRange(zero, mod); 

    LGInterpolator interpFunc(xfx, mod);

     for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator testIter = xfx.begin(); testIter != xfx.end(); ++ testIter){
         try{
            BigNumber TestVal = interpFunc(testIter->first);
            BOOST_TEST (TestVal.ToDec() == testIter->second.ToDec());
        }
        catch(std::exception& e){
            std::cout << e.what() << std::endl;
        }
     }
}

BOOST_AUTO_TEST_CASE( test_Interpolation_empty_coeff )
{

    std::vector< std::pair<BigNumber,BigNumber > >   bnCoefficients ;      
    BigNumber Mod; 
    Mod.Zero(); 

    // Polynomial is empty, returning
    BOOST_CHECK_THROW
        ( 
            LGInterpolator interp ( bnCoefficients, Mod ) ,
            std::runtime_error  
        );
}

BOOST_AUTO_TEST_CASE( test_Interpolation_repeated_coeff )
{
    // Chose a polynomial with repeating co-efficients.
    std::vector< std::pair<BigNumber,BigNumber > >   bnCoefficients ; 
    BigNumber val1; 
    val1.FromDec ("12");
    BigNumber val2 ; 
    val2.FromDec("7");
    BigNumber val3; 
    val3.FromDec ("7");
    BigNumber val4;
    val4.FromDec ("2");   
    BigNumber random; 
    bnCoefficients.push_back(std::make_pair(val1,random));
    bnCoefficients.push_back(std::make_pair(val2,random));
    bnCoefficients.push_back(std::make_pair(val3,random));
    bnCoefficients.push_back(std::make_pair(val4,random));
    BigNumber Mod; 
    Mod.Zero(); 

    // Polynomial has non-unique co-effs, returning
    BOOST_CHECK_THROW
        ( 
            LGInterpolator interp ( bnCoefficients, Mod ) ,
            std::runtime_error  
        );
}

BOOST_AUTO_TEST_SUITE_END( ) ;

/* 
    <todo>
Add large tests (see Chi's comments)
 */
