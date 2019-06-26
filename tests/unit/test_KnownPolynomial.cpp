#define BOOST_TEST_MODULE test_ECPoints
#include <boost/test/unit_test.hpp>

#include "utils/TestUtilsThreads.h"
#include "data/TestPolynomialCurveData.h"
#include <BigNumbers/BigNumbers.h>
#include <ThresholdSignatures/polynomial/KnownPolynomial.h>
#include <algorithm>
#include <random>
#include <string>

#include <boost/math/tools/rational.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using TestPolyCurveDataList = std::vector<  std::tuple< std::vector<std::string>, std::string, std::string> >;

unsigned  getRandomNumberInRange(int start, int end)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(start, end); // define the range
    return distr(eng);
}

const char* SecpOrder { "115792089237316195423570985008687907853269984665640564039457584007908834671663" };
const char* BigInt { "115792089237316195423570985008687907853269984665640564039457584007913129639936" };

TestPolyCurveDataList testPolyCurveData = 
{

    { {}  , "1", "0" },
    { {"0"}  , "1", "0" },
    { { "1" } , "1", "1" }, // Constant order, 1 term, 0 degree
    { { "-1" } , "1", "-1" }, // Constant order, 1 term, 0 degree

    // Linear equations - y= mx + c
    { { "2", "3" }, "4", "11" },
    { { "-2", "3" }, "-3", "9" },
    { { "2", "3" }, "0", "3" },
    { { "1", "0" }, largeCryptoPPSizeIntNumber, largeCryptoPPSizeIntNumber },
    { { "1", int1024Max }, int1024Min, "0" },
    { { "2", int1024Max }, int1024Min, int1024Min },
    { { "2", int1024Min }, int1024Max, int1024Max },

    // Quadratic equations -  ax^2 + bx + c
    { { "0", "0", "0" }, "4", "0" },
    { { "-1", "0", "1" }, "3", "-8" },
    { { "1", "0", "0" }, "-1", "1" },
    { { int1024Max, int1024Min, int1024Min}, "2", int1024Max },
    { { largeCryptoPPSizeIntNumber, "-"+largeCryptoPPSizeIntNumber, "-" + largeCryptoPPSizeIntNumber }, "2", largeCryptoPPSizeIntNumber },


    // cubic equations - ax^3 + bx^2 + cx + d
    { { "0", "0", "0", "0" }, "4", "0" },
    { { "1", "1", "1", "1" }, "-1", "0" },
    { { "-1", "1", "0", "2" }, int1024Max, resultint1024MaxCubic },
    { { "1", int1024Max, "0", largeCryptoPPSizeIntNumber }, int1024Min, largeCryptoPPSizeIntNumber },
    { { "-1", largeCryptoPPSizeIntNumber, "0", "0" }, largeCryptoPPSizeIntNumber, "0" },

};

void TestPolynomialData(TestPolyCurveDataList testPolyCurveDataList)
{
    // Get the PolyCurveData, where each item is in the format -{ {coefficients}, X value to be given, Expected Result Y}
    for (auto & testPolyCurveData : testPolyCurveDataList)
    {
        std::vector<BigNumber> coefficients;

        // get the coefficients
        for (auto & coefficient : std::get<0>(testPolyCurveData))
        {
            BigNumber bnVal;
	    bnVal.FromDec(coefficient.c_str());
            coefficients.push_back(std::move(bnVal));
        }

        // create a polynomial with coefficients
        KnownPolynomial polynomial { coefficients };

        // verify the coefficients list
        BOOST_CHECK(polynomial.getCoefficients() == coefficients);

        // Make sure that the degree is equal to the number of coefficients supplied.
        if (coefficients.size() == 0)
        {
            BOOST_CHECK(polynomial.getDegree() == -1);
        }
        else
        {
            BOOST_CHECK(polynomial.getDegree() == coefficients.size() - 1);
        }

        // Make sure that the polynomial curve evaluation result meets the expected result.
	BigNumber bnValA;
	bnValA.FromDec(std::get<1>(testPolyCurveData).c_str());

	BigNumber bnValB;
	bnValB.FromDec( std::get<2>(testPolyCurveData).c_str());

        BOOST_CHECK( polynomial.evaluate(bnValA ) == bnValB);
    }

    return;
}

void TestBiggerPolynomial(unsigned times)
{

    std::vector<BigNumber> coefficients;

    for (unsigned i = 0; i < times; i++)
    {
        coefficients.clear();
	
        // Get random coefficients's size
        for (unsigned j = 1; j <= getRandomNumberInRange(1, 65536); j++)
        {
            BigNumber bnVal;
	    bnVal.FromDec(std::to_string(i));
            coefficients.push_back(std::move(bnVal));
        }

        KnownPolynomial polyRandDegree{ coefficients };

        // Make sure that the degree is equal to the number of coefficients supplied.
        if (coefficients.size() == 0)
        {
            BOOST_CHECK_EQUAL(polyRandDegree.getDegree(), -1);
        }
        else
        {
            BOOST_CHECK_EQUAL(polyRandDegree.getDegree(), coefficients.size() - 1);
        }
	
        // Make sure that the polynomial can be evaluated.
        BOOST_CHECK_NO_THROW( { 
			          BigNumber bnVal;
				  bnVal.FromDec(std::to_string(getRandomNumberInRange(1, 65536)));
                                  polyRandDegree.evaluate(std::move(bnVal));
                              }
                            );
    }
 
}

void TestPolynomialConWithRndcoefficientsAndDegree(unsigned times)
{		
		
    for (unsigned i = 0; i < times; i++)
    {
        // get the random degree
        size_t rngDegree = getRandomNumberInRange(1, 655365466) % 200;

        // Generate the Polynomial with random degree, with random coefficients values.
	BigNumber bnValA, bnValB;
	bnValA.FromDec(int1024Min);
	bnValB.FromDec(int1024Max);
        KnownPolynomial polyRandomCoeAndDegree{ rngDegree, std::move(bnValA), std::move(bnValB) };

        // check the polynomial degree
        BOOST_CHECK_EQUAL(polyRandomCoeAndDegree.getDegree(), rngDegree);

        // check coefficients are in the range 
        for (auto & cofficient : polyRandomCoeAndDegree.getCoefficients())
        {
            BigNumber bnValA, bnValB;
	    bnValA.FromDec(int1024Min);
	    bnValB.FromDec(int1024Max);
            BOOST_CHECK(cofficient > bnValA || cofficient == bnValA);
            BOOST_CHECK(cofficient < bnValB || cofficient == bnValA);
        }			
    }
		
    return;
}


BOOST_AUTO_TEST_SUITE(test_suite_KnownPolynomial)

// Test random polynomial evaluation compared to boost
BOOST_AUTO_TEST_CASE(TestRandomKnownPolynomialEvaluation)
{
    RunNTimesThreaded<50>([](size_t i, std::stringstream& log) {
        
        BigNumber  bnValB, bnValC; 

	bnValB.FromDec("1");
	bnValC.FromDec(SecpOrder);

        // Generate random polynomial
        KnownPolynomial poly {i, bnValB, bnValC} ; 

        // Build equivalent boost polynomial
        using BoostInt = boost::multiprecision::cpp_int;
        const std::vector<BigNumber>& coeffs { poly.getCoefficients() };
        std::vector<BoostInt> boostCoeffs {};
        for(auto it = coeffs.rbegin(); it != coeffs.rend(); ++it)
        {
            //boostCoeffs.emplace_back(it->str().c_str());
            boostCoeffs.emplace_back(it->ToDec().c_str());
        }

        // Check both evaluate to the same result
        //LargeNumber x { RandutilsSeed{}, LargeNumber{0}, LargeNumber{BigInt} };
	BigNumber min, max;
	min.FromDec("0");
	max.FromDec(BigInt);
	BigNumber x = GenerateRandRange(min, max);
         
        BigNumber y { poly.evaluate(x) };
        BoostInt boostx { x.ToDec() };
        BoostInt boosty { boost::math::tools::evaluate_polynomial(boostCoeffs.data(), boostx, boostCoeffs.size()) };

        NCHAIN_THREAD_CHECK(y.ToDec() == boosty.convert_to<std::string>());
    });

}

/*test case : test the addition of ECPoints, and compare the result*/
BOOST_AUTO_TEST_CASE(testKnownPolynomialData)
{
    TestPolynomialData(testPolyCurveData);
}

BOOST_AUTO_TEST_CASE(testBiggerPolynomial)
{
    // Verify that the polynomial construction with random coefficients' size
    TestBiggerPolynomial(10);
}

BOOST_AUTO_TEST_CASE(testPolynomialConWithRndcoefficientsAndDegree)
{
    // Verify that the polynomial construction with random coefficients and degree
    TestPolynomialConWithRndcoefficientsAndDegree(10);
}

BOOST_AUTO_TEST_SUITE_END();
