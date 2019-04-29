#define BOOST_TEST_MODULE test_BigNumbers
#include <boost/test/unit_test.hpp>

#include <BigNumbers/BigNumbers.h>
#include <BigNumbers/BigNumAPI.h>
#include <iostream>
#include <memory>
#include "test_BigNumbers.h"
#include "TestUtils.h"

namespace unitTests
{

    void TestAdditionDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberAdditionDecData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(addEntry)});

            // perform addition operation on LHS and RHS
            BigNumber result = lhs + rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
        }
    }

    void TestAdditionHexaDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberAdditionHexData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(addEntry)});

            // perform LHS + RHS
            BigNumber result = lhs + rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
        }
    }

    void TestSubtractionDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberSubtractionData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(addEntry)});

            // perform  LHS - RHS
            BigNumber result = lhs - rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
        }
    }

    void TestSubtractionHexaDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberSubtractionHexaData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(addEntry)});

            // perform LHS - RHS
            BigNumber result = lhs - rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
        }
    }


    void TestMultipleDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberMultipleData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(addEntry)});

            // perform LHS * RHS
            BigNumber result = lhs * rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
        }
    }

    void TestMultipleHexaDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberMultipleDataHexa )
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(addEntry)});

            // perform LHS * RHS
            BigNumber result = lhs * rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
        }
    }

    void TestDivisionDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberDivisionDecimalData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(addEntry)});

            // perform LHS / RHS, expect an exception if the RHS is 0
	    if (rhs.ToDec() == "0")
	    {
                BOOST_CHECK_THROW(lhs / rhs, std::exception);
	    }
	    else
	    {
                BigNumber result = lhs / rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
	    }
        }
    }

    void TestDivisionHexaDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberDivisionHexaDecimalData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(addEntry)});

            // perform LHS / RHS, expect an exception if the RHS is 0
	    if (rhs.ToDec() == "0")
	    {
                BOOST_CHECK_THROW(lhs / rhs, std::exception);
	    }
	    else
	    {
                BigNumber result = lhs / rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
	    }
        }
    }

    void TestLeftShiftDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberLeftShiftData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(addEntry)});

            // perform  LHS << RHS, where expect a exception if the RHS is -ve
	    if (rhs.ToDec().length() > 0 && rhs.ToDec()[0] == '-')
	    {
                BOOST_CHECK_THROW(lhs << rhs, std::exception);
	    }
	    else
	    {
                BigNumber result = lhs << rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
	    }
        }
    }

    void TestLeftShiftHexaDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberLeftShiftHexaData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(addEntry)});

            // perform  LHS << RHS, where expect a exception if the RHS is -ve
	    if (rhs.ToDec().length() > 0 && rhs.ToDec()[0] == '-')
	    {
                BOOST_CHECK_THROW(lhs << rhs, std::exception);
	    }
	    else
	    {
                BigNumber result = lhs << rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
	    }
        }
    }

    void TestRightShiftDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberRightShiftData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(addEntry)});

            // perform  LHS >> RHS, where expect a exception if the RHS is -ve
	    if (rhs.ToDec().length() > 0 && rhs.ToDec()[0] == '-')
	    {
                BOOST_CHECK_THROW(lhs >> rhs, std::exception);
	    }
	    else
	    {
                BigNumber result = lhs >> rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
	    }
        }
    }

    void TestRightShiftHexaDecimal()
    {
        // Iterate over Data for large number addition
        for (const auto & addEntry : testLargeNumberRightShiftHexaData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(addEntry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(addEntry)});

            // perform  LHS >> RHS, where expect a exception if the RHS is -ve
	    if (rhs.ToDec().length() > 0 && rhs.ToDec()[0] == '-')
	    {
                BOOST_CHECK_THROW(lhs >> rhs, std::exception);
	    }
	    else
	    {
                BigNumber result = lhs >> rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(addEntry), result.ToDec());
	    }
        }
    }

    BOOST_AUTO_TEST_SUITE(test_suite_BigNumbers)

    BOOST_AUTO_TEST_CASE(bar_case)
    {
        BOOST_CHECK(true);
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalAddition)
    {
        // Verify if Additions can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersDecimalAddition);
        TestAdditionDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaAddition)
    {
        // Verify if Additions can be performed on the large number in hexadecimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersHexaAddition);
        TestAdditionHexaDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalSubtraction)
    {
        // Verify if Additions can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersDecimalSubtraction);
        TestSubtractionDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaSubtraction)
    {
        // Verify if Additions can be performed on the large number in hexadecimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersHexaSubtraction);
        TestSubtractionHexaDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalMultiple)
    {
        // Verify if multiple  can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersDecimalMultiple);
        TestMultipleDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    
    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaDecimalMultiple)
    {
        // Verify if multiple  can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersHexaDecimalMultiple);
        TestMultipleHexaDecimal();
        NCHAIN_TEST_TIME_STOP;
    }
    

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalDivision)
    {
        // Verify if division  can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersDecimalDivision);
        TestDivisionDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaDecimalDivision)
    {
        // Verify if division  can be performed on the large number in hexa decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersHexaDecimalDivision);
        TestDivisionHexaDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalLeftShift)
    {
        // Verify if division  can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersDecimalLeftShift);
        TestLeftShiftDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexDecimalLeftShift)
    {
        // Verify if division  can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersHexDecimalLeftShift);
        TestLeftShiftHexaDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalRightShift)
    {
        // Verify if division  can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersDecimalRightShift);
        TestRightShiftDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaDecimalRightShift)
    {
        // Verify if division  can be performed on the large number in decimals.
        NCHAIN_TEST_TIME_START(testLargeNumbersHexaDecimalRightShift);
        TestRightShiftHexaDecimal();
        NCHAIN_TEST_TIME_STOP;
    }

    BOOST_AUTO_TEST_SUITE_END();
}
