#define BOOST_TEST_MODULE test_Polynomial

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <BigNumbers/BigNumbers.h>
#include <ThresholdSignatures/Polynomial.h>

#include <string>
#include <iostream>


BOOST_AUTO_TEST_SUITE( test_suite_Polynomial )


/* Create a new polynomial using the string constructor
 *   Check the degree
 *   Check the coefficients
 */
BOOST_AUTO_TEST_CASE( test_polynomial_degree1 )
{
    // 3 + 2x 
    std::vector< std::string>  strCoefficients { "3",  "2" } ;     
    Polynomial poly ( strCoefficients, GenerateZero( ) ) ;

    long degree = poly.getDegree( ) ;
    BOOST_CHECK_EQUAL ( degree, strCoefficients.size() - 1 ) ;

    BigNumber fx_0, fx_1, eval_at_0, eval_at_1 ;
    fx_0.FromDec( "0" ) ;
    fx_1.FromDec( "1" ) ;
    eval_at_0 = poly( fx_0 ) ;
    eval_at_1 = poly( fx_1 ) ;

    BOOST_ASSERT  ( eval_at_0.ToDec() == "3"  ) ;
    BOOST_ASSERT  ( eval_at_1.ToDec() == "5"  ) ;

    //std::cout << "Sanity check:" << std::endl ;
    std::cout << "eval_at_0 = " << eval_at_0.ToDec( ) << std::endl ;
    std::cout << "eval_at_1 = " << eval_at_1.ToDec( ) << std::endl ;

    // <todo> remove this
    std::cout << poly << std::endl ;
}

/* Create a new polynomial using the vector constructor
 *   Check the degree
 *   Check the coefficients
 */
BOOST_AUTO_TEST_CASE( test_polynomial_degree2 )
{
     // 1 + 2x + 3x^2 
    std::vector< std::string >  strCoefficients { "1",  "2", "3" } ;     
    std::vector< BigNumber >    bnCoefficients ;

    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
	    big.FromDec( element ) ;
        bnCoefficients.push_back( std::move( big ) ) ;
    }

    Polynomial poly ( bnCoefficients, GenerateZero( ) ) ;

    long degree = poly.getDegree( ) ;
    BOOST_CHECK_EQUAL ( degree, strCoefficients.size() - 1 ) ;

    BigNumber eval_at_0, eval_at_1, eval_at_2;

    BOOST_ASSERT  ( poly[ 0 ] == eval_at_0.FromDec( "1" )  ) ;
    BOOST_ASSERT  ( poly[ 1 ] == eval_at_1.FromDec( "2" )  ) ;
    BOOST_ASSERT  ( poly[ 2 ] == eval_at_2.FromDec( "3" )  ) ;

    // <todo> remove this
    std::cout << poly << std::endl ;
}

/* Create a new polynomial using the string constructor
 *   Check the mod function
 */
BOOST_AUTO_TEST_CASE( test_polynomial_degree2_mod )
{
    // 6 + 7x + 8x^2  [mod 5]
    std::vector< std::string>  strCoefficients { "6",  "7", "8" } ;     
    BigNumber modulo ;
    modulo.FromDec( "5" ) ;

    Polynomial poly ( strCoefficients, modulo ) ;


    BigNumber eval_at_0, eval_at_1, eval_at_2 ;

    BOOST_ASSERT  ( poly[ 0 ] == eval_at_0.FromDec( "1" )  ) ;
    BOOST_ASSERT  ( poly[ 1 ] == eval_at_1.FromDec( "2" )  ) ;
    BOOST_ASSERT  ( poly[ 2 ] == eval_at_2.FromDec( "3" )  ) ;

    // <todo> remove this
    std::cout << poly << std::endl ;
}

/* Create a new polynomial using the vector constructor
 *   Check the mod function
 */
BOOST_AUTO_TEST_CASE( test_polynomial_degree2_mod2 )
{
    // 16 + 9x^2  [mod 5]
    std::vector< std::string>  strCoefficients { "16",  "0", "9" } ;     
    BigNumber modulo ;
    modulo.FromDec( "5" ) ;

    std::vector< BigNumber >    bnCoefficients ;

    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
	    big.FromDec( element ) ;
        bnCoefficients.push_back( std::move( big ) ) ;
    }

    Polynomial poly ( bnCoefficients, modulo ) ;


    BigNumber eval_at_0, eval_at_1 ;

    BOOST_ASSERT  ( poly[ 0 ] == eval_at_0.FromDec( "1" )  ) ;
    BOOST_ASSERT  ( poly[ 1 ] == eval_at_1.FromDec( "0" )  ) ;
    BOOST_ASSERT  ( poly[ 2 ] == eval_at_2.FromDec( "4" )  ) ;

    // <todo> remove this
    std::cout << poly << std::endl ;
}



BOOST_AUTO_TEST_SUITE_END( ) ;

/* 
    <todo>
    What elses do we need to test?
    - small polynomial with random inputs
    - small polynomial with big number
    - Assignments ?
 */
