#define BOOST_TEST_MODULE test_Polynomial

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <BigNumbers/BigNumbers.h>
#include <Polynomial/Polynomial.h>

#include <string>
#include <iostream>


BOOST_AUTO_TEST_SUITE( test_suite_Polynomial )


/* Create a new polynomial using the string constructor
 *   Check the degree
 *   Evaluate the point for x=0,1
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
    
    BOOST_TEST  ( eval_at_0.ToDec() == "3"  ) ;
    BOOST_TEST  ( eval_at_1.ToDec() == "5"  ) ;

    //std::cout << "Sanity check:" << std::endl ;
    //std::cout << "eval_at_0 = " << eval_at_0.ToDec( ) << std::endl ;
    //std::cout << "eval_at_1 = " << eval_at_1.ToDec( ) << std::endl ;

    std::ostringstream ss ;
    ss << poly ;
    BOOST_TEST ( ss.str( ) == "modulo = 0;	polynomial = 3 + 2x" ) ;
}


/* Create a new polynomial using the vector constr
 *   Evaluate the point for x=0,1,2
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

    BigNumber fx_0, fx_1, fx_2, eval_at_0, eval_at_1, eval_at_2;
    fx_0.FromDec( "0" ) ;
    fx_1.FromDec( "1" ) ;
    fx_2.FromDec( "2" ) ;

    eval_at_0 = poly( fx_0 ) ;
    eval_at_1 = poly( fx_1 ) ;
    eval_at_2 = poly( fx_2 ) ;

    BOOST_TEST  ( eval_at_0.ToDec( ) == "1" ) ;
    BOOST_TEST  ( eval_at_1.ToDec( ) == "6" ) ;
    BOOST_TEST  ( eval_at_2.ToDec( ) == "17" ) ;

    std::ostringstream ss ;
    ss << poly ;
    BOOST_TEST ( ss.str( ) == "modulo = 0;	polynomial = 1 + 2x + 3x^2" ) ;
}

/* Create a new polynomial using the string constructor
 *   Check the mod function
 *   Check the [] operator
 *   Check the f(x) for a vector
 */
BOOST_AUTO_TEST_CASE( test_polynomial_degree2_mod )
{
    // 6 + 7x + 8x^2  [mod 5]
    std::vector< std::string>  strCoefficients { "6",  "7", "8" } ;     
    BigNumber modulo ;
    modulo.FromDec( "5" ) ;

    Polynomial poly ( strCoefficients, modulo ) ;

    BigNumber array_0, array_1, array_2 ;
    array_0.FromDec( "1" ) ;
    array_1.FromDec( "2" ) ;
    array_2.FromDec( "3" ) ;

    BOOST_CHECK  ( ( poly[ 0 ] ) ==  array_0 ) ;
    BOOST_CHECK  ( ( poly[ 1 ] ) ==  array_1 ) ;
    BOOST_CHECK  ( ( poly[ 2 ] ) ==  array_2 ) ;


    BigNumber fx_0, fx_1, fx_2, eval_at_0, eval_at_1, eval_at_2;
    fx_0.FromDec( "0" ) ;
    fx_1.FromDec( "1" ) ;
    fx_2.FromDec( "2" ) ;

    eval_at_0 = poly( fx_0 ) ;
    eval_at_1 = poly( fx_1 ) ;
    eval_at_2 = poly( fx_2 ) ;

    BOOST_TEST  ( eval_at_0.ToDec( ) == "1" ) ;
    BOOST_TEST  ( eval_at_1.ToDec( ) == "1" ) ;
    BOOST_TEST  ( eval_at_2.ToDec( ) == "2" ) ;

   
    std::ostringstream ss ;
    ss << poly ;
    BOOST_TEST ( ss.str( ) == "modulo = 5;	polynomial = 1 + 2x + 3x^2" ) ;
}

/* Create a new polynomial using the vector constructor
 *   Check the mod function
 *   Check the f(x) for x = 0,1,2
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


    BigNumber array_0, array_1, array_2;

    array_0.FromDec( "1" ) ;
    array_1.FromDec( "0" ) ;
    array_2.FromDec( "4" ) ;

    BOOST_CHECK  ( ( poly[ 0 ] ) ==  array_0 ) ;
    BOOST_CHECK  ( ( poly[ 1 ] ) ==  array_1 ) ;
    BOOST_CHECK  ( ( poly[ 2 ] ) ==  array_2 ) ;

    BigNumber fx_0, fx_1, fx_2, eval_at_0, eval_at_1, eval_at_2;
    fx_0.FromDec( "0" ) ;
    fx_1.FromDec( "1" ) ;
    fx_2.FromDec( "2" ) ;

    eval_at_0 = poly( fx_0 ) ;
    eval_at_1 = poly( fx_1 ) ;
    eval_at_2 = poly( fx_2 ) ;

    BOOST_TEST  ( eval_at_0.ToDec( ) == "1" ) ;
    BOOST_TEST  ( eval_at_1.ToDec( ) == "0" ) ;
    BOOST_TEST  ( eval_at_2.ToDec( ) == "2" ) ;

    std::ostringstream ss ;
    ss << poly ;
    BOOST_TEST ( ss.str( ) == "modulo = 5;	polynomial = 1 + 0x + 4x^2" ) ;    
}


/* Create a new plynomial using the random number constructor
 * <todo> put these tests into BOOST_CHECK | BOOST_TEST
 *   test for - a_0 is not zero
 *            - last coefficient is not zero
 *            - degree of coefficient is as expected
 *            - in case of fixed a_0, it is as expected
 */
BOOST_AUTO_TEST_CASE( test_polynomial_random1 )
{
    std::vector< Polynomial > Polynomials ;

    BigNumber modulo ;
    BigNumber a_0 ;

    modulo.FromDec( "5" ) ; 

    // constuct using degree and modulo 0     
    Polynomials.push_back ( Polynomial ( 5, modulo ) ) ;

    BOOST_TEST( Polynomials.back( ).getDegree( ) == 5 ) ;
   
    // constuct using degree and modulo 12
    modulo.FromDec( "12" ) ; 
    Polynomials.push_back ( Polynomial ( 5, modulo ) ) ;
    BOOST_TEST( Polynomials.back( ).getDegree( ) == 5 ) ;

    // constuct using degree, modulo and fixed a_0
    a_0.FromDec( "10" ) ;
    Polynomials.push_back  ( Polynomial( 2, modulo, a_0 ) ) ;
    BOOST_TEST( Polynomials.back( ).getDegree( ) == 2 ) ;
    
    // test the a_0 is as expected
    BOOST_CHECK( a_0 == Polynomials.back( )[ 0 ] ) ;

    for ( 
            auto poly = Polynomials.begin( ) ; 
            poly != Polynomials.end( ) ; 
            ++poly 
        )
    {
        //std::cout << "Poly:\t" << *poly << "\tdegree = " << poly->getDegree( ) << std::endl ;

        // test a_0 is not zero
        BOOST_CHECK( !( (*poly)[0] == GenerateZero( ) ) ) ;
        // test last term is not zero
        BOOST_CHECK( !( (*poly)[ poly->getDegree( ) ] == GenerateZero( ) ) ) ;

    } 
}


/***********************************************************
 *  Error testing
 ************************************************************/

// Check modulo is zero
BOOST_AUTO_TEST_CASE( test_polynomial_min_max )
{
    // Invalid range, min == max == 0
    BOOST_CHECK_THROW
        ( 
            Polynomial( 3, GenerateZero( ) ), 
            std::range_error 
        );

    BigNumber modulo, min, max ;
    modulo.FromDec( "0" ) ;
    min.FromDec( "0" ) ;
    max.FromDec( "0" ) ;
    
    // Invalid range, min == max == 0
    BOOST_CHECK_THROW
        ( 
            Polynomial( 3, modulo, min, max ) , 
            std::range_error 
        );        
}

// Check a_0 is zero
BOOST_AUTO_TEST_CASE( test_polynomial_a_0_zero )
{
    // a_0 coefficient should be strictly positive, a_0 = 0
    BOOST_CHECK_THROW
        ( 
            Polynomial( 3, GenerateZero( ), GenerateZero( ) ),
            std::range_error 
        );
}

// Check a_0 > modulo
BOOST_AUTO_TEST_CASE( test_polynomial_a_0_modulo )
{
    BigNumber modulo, a_0 ;
    modulo.FromDec( "5" ) ;
    a_0.FromDec( "6" ) ;

    // a_0 coefficient should be lower than modulo, but a_0=6, modulo=5
    BOOST_CHECK_THROW
        ( 
            Polynomial( 3, modulo, a_0  ) ,
            std::range_error 
        );
}

// Check min >= 0 
BOOST_AUTO_TEST_CASE( test_polynomial_min_zero )
{
    BigNumber min, max ;
    min.FromDec( "-1" ) ;
    max.FromDec( "1" ) ;

    // min should be zero or greater, min = -1
    BOOST_CHECK_THROW
        ( 
            Polynomial( 3, GenerateZero( ), min, max ) ,
            std::range_error 
        );
}

// Check min >= 0 
BOOST_AUTO_TEST_CASE( test_polynomial_max_modulo )
{
    BigNumber modulo, min, max ;
    modulo.FromDec( "5" ) ;
    min.FromDec( "0" ) ;
    max.FromDec( "4" ) ;

    // max should greater than or equal to modulo, max = 5, modulo = 6
    BOOST_CHECK_THROW
        ( 
            Polynomial( 3, modulo, min, max ) ,
            std::range_error 
        );
}

// Check empty coefficients
BOOST_AUTO_TEST_CASE( test_polynomial_empty_coeff )
{
    std::vector< std::string>  strCoefficients ;     

    // Polynomial is empty, returning
    BOOST_CHECK_THROW
        ( 
            Polynomial poly ( strCoefficients, GenerateZero( ) ) ,
            std::runtime_error  
        );

   std::vector< BigNumber >    bnCoefficients ; ;     

    // Polynomial is empty, returning
    BOOST_CHECK_THROW
        ( 
            Polynomial poly ( bnCoefficients, GenerateZero( ) ) ,
            std::runtime_error  
        );
}


// Check zero coefficients at highest degree
BOOST_AUTO_TEST_CASE( test_polynomial_zero_high )
{
    std::vector< std::string>  strCoefficients { "3",  "0" } ;     
    
    // Polynomial has zero coefficient at the highest degree, returning
    BOOST_CHECK_THROW
        ( 
            Polynomial poly ( strCoefficients, GenerateZero( ) ) ,
            std::runtime_error  
        );

    strCoefficients.push_back( "0" ) ;
    std::vector< BigNumber >    bnCoefficients ;

    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
	    big.FromDec( element ) ;
        bnCoefficients.push_back( std::move( big ) ) ;
    }   

    // Polynomial has zero coefficient at the highest degree, returning
    BOOST_CHECK_THROW
        ( 
            Polynomial poly ( bnCoefficients, GenerateZero( ) ) ,
            std::runtime_error  
        );         
    
}
BOOST_AUTO_TEST_SUITE_END( ) ;

/* 
    <todo>
Add large tests (see Chi's comments)
 */
