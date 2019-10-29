/// Define test module name with debug postfix
#ifdef NDEBUG 
#  define BOOST_TEST_MODULE test_Polynomial
#else
#  define BOOST_TEST_MODULE test_Polynomiald
#endif


#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

#include <BigNumbers/BigNumbers.h>
#include <Polynomial/Polynomial.h>
#include <Polynomial/LGInterpolator.h>

#include <string>
#include <iostream>
#include <cmath>

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

BOOST_AUTO_TEST_SUITE( test_suite_Polynomial )


/* Create a new polynomial from vector of strings
 *   Check the degree
 *   Evaluate the point for x=0,1
 *   Check the coefficients
 */
BOOST_AUTO_TEST_CASE( test_polynomial_degree1 )
{
    // 3 + 2x 
    std::vector< std::string>  strCoefficients { "3",  "2" } ;
    std::vector< BigNumber>  bnCoefficients ;    
    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
        big.FromDec( element ) ;
        bnCoefficients.push_back( std::move( big ) ) ;
    }
    Polynomial poly ( bnCoefficients, GenerateZero( ) ) ;

    long degree = poly.getDegree( ) ;
    BOOST_CHECK_EQUAL ( degree, strCoefficients.size() - 1 ) ;

    BigNumber fx_0, fx_1, eval_at_0, eval_at_1 ;
    fx_0.FromDec( "0" ) ;
    fx_1.FromDec( "1" ) ;
    eval_at_0 = poly( fx_0 ) ;
    eval_at_1 = poly( fx_1 ) ;
    
    BOOST_TEST  ( eval_at_0.ToDec() == "3"  ) ;
    BOOST_TEST  ( eval_at_1.ToDec() == "5"  ) ;

    std::ostringstream ss ;
    ss << poly ;
    BOOST_TEST ( ss.str() == "modulo = 0;    polynomial = 3 + 2x" ) ;
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
    BOOST_TEST ( ss.str( ) == "modulo = 0;    polynomial = 1 + 2x + 3x^2" ) ;
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
    std::vector< BigNumber >   bnCoefficients ;

    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
        big.FromDec( element ) ;
        bnCoefficients.push_back( std::move( big ) ) ;
    }

    BigNumber modulo ;
    modulo.FromDec( "5" ) ;

    Polynomial poly ( bnCoefficients, modulo ) ;

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
    BOOST_TEST ( ss.str( ) == "modulo = 5;    polynomial = 1 + 2x + 3x^2" ) ;
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
    BOOST_TEST ( ss.str( ) == "modulo = 5;    polynomial = 1 + 0x + 4x^2" ) ;    
}


/* Create a new plynomial using the random number constructor
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

    // construct using degree and modulo 0     
    Polynomials.push_back ( Polynomial ( 5, modulo ) ) ;

    BOOST_TEST( Polynomials.back( ).getDegree( ) == 5 ) ;
   
    // construct using degree and modulo 12
    modulo.FromDec( "12" ) ; 
    Polynomials.push_back ( Polynomial ( 5, modulo ) ) ;
    BOOST_TEST( Polynomials.back( ).getDegree( ) == 5 ) ;

    // construct using degree, modulo and fixed a_0
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
        // test a_0 is not zero
        BOOST_CHECK( !( (*poly)[0] == GenerateZero( ) ) ) ;
        // test last term is not zero
        BOOST_CHECK( !( (*poly)[ poly->getDegree( ) ] == GenerateZero( ) ) ) ;

    } 
}

// Check polynomial hide function
BOOST_AUTO_TEST_CASE( test_polynomial_hide )
{
    std::vector< std::string >  strCoefficients { "3", "2", "1", "1" } ;
    std::vector< BigNumber >    bnCoefficients ;

    // create a polynomial from vector of strings
    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
        big.FromDec( element ) ;
        bnCoefficients.push_back( std::move( big ) ) ;
    }   

    Polynomial poly ( bnCoefficients, GenerateZero( ) ) ;

    int curveID ( 714 ) ;

    std::vector< BigNumber > hiddenCoefficients = poly.hide( curveID ) ;
    Polynomial hiddenPoly ( hiddenCoefficients, GenerateZero( ) ) ;   

    BOOST_CHECK( hiddenPoly[2] == hiddenPoly[3]) ;
    BOOST_CHECK( hiddenPoly[0].ToDec() == "344295838914342996903890629191305488808654946672820536623545781571036035167993" ) ;
}


/***********************************************************
 *  Stress testing
 ************************************************************/

/* Polynomial Stress Test 1
 *
 * Working with modulo 17 :
 * generate randomly an polynomial of degree 3,
 * generate randomly x mod 17.
 * evaluate the polynomial at x.
 * Test the evaluated value is between zero and 16.
 * Wrap all those steps inside a loop 200 iterations.
*/
BOOST_AUTO_TEST_CASE( test_polynomial_stress_1 )
{

    BigNumber modulo, min, max ;
    modulo.FromDec( "17" ) ;
    
    BigNumber minusOne (GenerateZero( ) ) ;
    --minusOne ;

    for ( auto i = 0 ; i < 200 ; ++i )
    {
        // default min and max value
        Polynomial poly1 ( Polynomial( 3, modulo ) ) ;

        // user defined min and max values
        min.FromDec( "0" ) ;
        max.FromDec( "100" ) ;
        Polynomial poly2 ( Polynomial( 3, modulo, min, max ) ) ;

        BigNumber randomX ( GenerateRandRange( min, max ) ) ;
        randomX = randomX % modulo ;

        // evaluate polynomial at x
        BigNumber eval_randomX1 ( poly1( randomX ) ) ;
        BigNumber eval_randomX2 ( poly2( randomX ) ) ;

        BOOST_CHECK ( eval_randomX1 > minusOne ) ;
        BOOST_CHECK ( eval_randomX1 < modulo ) ;
        BOOST_CHECK ( eval_randomX2 > minusOne ) ;
        BOOST_CHECK ( eval_randomX2 < modulo ) ;        
    }
}

/* Polynomial Stress Test 2
 * 
 * Working with no modulo (a 'normal' polynomial)
 * Generate random polynomial of degree 3, 
 * with coefficients less than 10. 
 * 
 * Choose random x of less than 100. 
 * 
 * Manually calculate the value of polynomial 
 * using normal C++ arithmetic calculation : 
 * a + bx + cx^2 + dx^3
 * 
 * Use Polynomial class to evaluate polynomial, 
 * compare the output to the manual calculation
 * 
 * Repeat for 200 iterations
 */
BOOST_AUTO_TEST_CASE( test_polynomial_stress_2 )
{
    BigNumber min, max, max_x ;
    min = GenerateZero( ) ;
    max.FromDec( "10" ) ;
    int x (3) ;
    max_x.FromDec( "100" ) ;

    for ( auto counter = 0; counter < 200; ++counter )
    {
        // polynomial with degree 3, not using modulo, with defined min and max
        Polynomial poly( 3, GenerateZero( ), min, max ) ; 
        BigNumber randomX ( GenerateRandRange( min, max_x ) ) ;

        // evaluate manually converting to integer
        int result = std::stoi( poly[0].ToDec( ) ) ;  
        int power = 1 ;  

        for ( auto i = 1 ; i <= poly.getDegree() ; ++i, ++power )
        {
            int coefficient (std::stoi( poly[i].ToDec( ) ) ) ;
            result = result + ( coefficient * std::pow(x,power ) ) ;
        }

        // convert to string format
        std::string result_manual ( std::to_string( result ) ) ;

        // evaluate using polynomial()
        BigNumber fx, result_p  ;
        fx.FromDec( std::to_string( x ) ) ; 
        result_p = poly( fx ) ;
    
        // convert ot string format
        std::string result_polynomial ( result_p.ToDec( ) ) ;

        // compare manual with polynomial
        BOOST_TEST( result_manual == result_polynomial ) ;
    }
}

/* Polynomial Stress Test 3
 * use modulo=“FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141” 
 * (this is the order of elliptic curve)
 * 
 * Used degree 3, set max to be modulo + 1
 * Choose random x and evaluate polynomial. 
 * Ensure evaluated x is less than modulo. 
 */
BOOST_AUTO_TEST_CASE( test_polynomial_stress_3 )
{
    BigNumber modulo, min, max ;
    modulo.FromHex( "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141") ;
    min = GenerateZero( ) ;
    max = modulo ;
    ++max ;

    for ( auto i = 0; i < 200; ++i )
    {
        Polynomial poly ( 3, modulo, min, max ) ;
        BigNumber randomX ( GenerateRandRange( min, max ) ) ;

        BigNumber eval_x ;
        eval_x = poly( randomX ) ;

        BOOST_CHECK( eval_x < modulo ) ;
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

    // create from vector of string
    std::vector< std::string>  strCoefficients { "0", "3",  "2" } ;

    // create from vector of BigNumber
    std::vector< BigNumber >    bnCoefficients ;

    for ( auto & element : strCoefficients )
    {
        BigNumber big ;
        big.FromDec( element ) ;
        bnCoefficients.push_back( std::move( big ) ) ;
    }

    BOOST_CHECK_THROW
    (
        Polynomial ( bnCoefficients, GenerateZero( ) ),
        std::range_error
    ) ;        
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
    std::vector< std::string>  strCoefficients { "3",  "2", "1", "0" } ;
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
            std::range_error  
        );         
    
}





/***********************************************************
 *  Interpolation testing
 ************************************************************/

// Test creation of LGInterpolator object of degree 3, modulo 17
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

    LGInterpolator interpFunc(xfx, mod);

    for( auto testIter = xfx.begin(); 
        testIter != xfx.end(); 
        ++ testIter)
    {
        BigNumber TestVal = interpFunc(testIter->first);
        BOOST_TEST (TestVal.ToDec() == testIter->second.ToDec());
    }
}

// Test creation of LGInterpolator object of degree 100, modulo 104729
BOOST_AUTO_TEST_CASE( test_Interpolation_degree_100_mod_104729 )
{
    // degree 100 & a mod of 104729 ( the 10000th prime)
    int degree = 50;
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

    LGInterpolator interpFunc(xfx, mod);

     for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator testIter = xfx.begin(); testIter != xfx.end(); ++ testIter){
        BigNumber TestVal = interpFunc(testIter->first);
        BOOST_TEST (TestVal.ToDec() == testIter->second.ToDec());
     }

}

// Test creation of LGInterpolator object of degree 200, modulo SECP256K1MOD
BOOST_AUTO_TEST_CASE( test_Interpolation_degree_8_mod_SECP256K1MOD )
{
    int degree = 7;
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

    LGInterpolator interpFunc(xfx, mod);

     for(std::vector<std::pair<BigNumber, BigNumber> >::const_iterator testIter = xfx.begin(); testIter != xfx.end(); ++ testIter){
        BigNumber TestVal = interpFunc(testIter->first);
        BOOST_TEST (TestVal.ToDec() == testIter->second.ToDec());
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

BOOST_AUTO_TEST_CASE( test_Interpolation_eval_at_invalid_basis )
{
    int degree = 7;
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
    BigNumber Val = interpFunc(xValue);
    BOOST_CHECK_THROW(
        BigNumber valBasis = interpFunc(55, xValue),
        std::runtime_error
    );

    BOOST_CHECK_THROW(
        BigNumber valBasis = interpFunc(-1, xValue),
        std::runtime_error
    );
}
////
BOOST_AUTO_TEST_CASE( test_Interpolation_eval_at_basis )
{
    int degree = 3;
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

    // Please refer to - https://en.wikipedia.org/wiki/Lagrange_polynomial
    // These should all evaluate to 1
    int index = 0;
    LGInterpolator interpFunc(xfx, mod);
    BigNumber one ; 
    one.One(); 
    for (std::vector<std::pair<BigNumber, BigNumber> >::const_iterator iter = xfx.begin (); iter != xfx.end(); ++ iter){
        BigNumber ithVal = interpFunc(index++, iter->first);
        BOOST_TEST ( ithVal.ToDec () == one.ToDec() );
    }
    
    // These should all evaluate to 0
    index = 5;
    BigNumber zero; 
    zero.Zero(); 
    for (std::vector<std::pair<BigNumber, BigNumber> >::const_iterator iter = xfx.begin (); iter != xfx.end(); ++ iter){
        BigNumber ithVal = interpFunc(index--, iter->first);
        BOOST_TEST ( ithVal.ToDec() == zero.ToDec());
    }

}

BOOST_AUTO_TEST_CASE( test_ECPoint_Interpolation_degree_7_mod_SECP256K1CURVE )
{
    std::vector<std::pair<BigNumber, ECPoint> > curve; 
    for (int i = 0; i< 8; ++ i){
        BigNumber index; 
        index.FromHex (std::to_string(i));
        ECPoint pt; 
        pt.SetRandom (); 
        curve.push_back ( std::make_pair(index, pt)); 
    }

    BigNumber mod;
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
    LGECInterpolator interpolator ( curve, mod); 

    for(std::vector<std::pair<BigNumber, ECPoint> >::iterator testIter = curve.begin(); testIter != curve.end(); ++ testIter){
        ECPoint TestVal = interpolator(testIter->first);
        BOOST_TEST (TestVal.ToHex() == testIter->second.ToHex());
     }
}



BOOST_AUTO_TEST_CASE( test_ECPoint_Interpolation_BNInterpolattionMuLG_EQUAL )
{

    int degree = 10;
    BigNumber mod; 
    mod.FromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"); 

    Polynomial poly(degree, mod);
    std::vector<std::pair<BigNumber,BigNumber> > xfx; 

    int margin = 2 ;
    int npPoint = degree + 1 + margin;
    
    for (int i=0;i<npPoint;++i){
        BigNumber bn;
        bn.generateRandHex(); 
        xfx.push_back(std::pair(bn, poly(bn)));
    }
    
    BigNumber xValueAtZero;
    xValueAtZero.Zero();
    
    LGInterpolator interpFunc(xfx, mod);
    BigNumber InterpolatedAtZero = interpFunc(xValueAtZero);
    
    
    
    // convert the BNs to ECPoints by multiplying by G
    
    std::vector<std::pair<BigNumber,ECPoint> > curve;
    
    ECPoint GenPoint; 
    ECPoint G = GenPoint.getGenerator();
    
    for(std::vector<std::pair<BigNumber,BigNumber> >::iterator iter = xfx.begin(); iter != xfx.end(); ++ iter){
        
       ECPoint hidden = G.MulHex(iter->second.ToHex(), std::string()); 
       curve.push_back(std::pair(iter->first, hidden));
    
    }
    
    LGECInterpolator interpolator ( curve, mod);
    
    ECPoint ecPointAtZero = interpolator(xValueAtZero);

    
    // multiply bn*G
    
    ECPoint bnAtZero = G.MulHex(InterpolatedAtZero.ToHex(), std::string());
    
    
     BOOST_TEST (bnAtZero.ToHex() == ecPointAtZero.ToHex());
    
}


BOOST_AUTO_TEST_SUITE_END( ) ;

