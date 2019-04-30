#define BOOST_TEST_MODULE test_BigNumbers
#include <boost/test/unit_test.hpp>

#include <BigNumbers/BigNumbers.h>
#include <BigNumbers/BigNumAPI.h>
#include <iostream>
#include <memory>
#include "test_BigNumbers.h"

namespace unitTests
{

    BOOST_AUTO_TEST_SUITE(test_suite_BigNumbers)

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalAddition)
    {
        // Verify if Additions can be performed on the large number in decimals.

        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberAdditionDecData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(entry)});

            // perform addition operation on LHS and RHS
            BigNumber result = lhs + rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaAddition)
    {
        // Verify if Additions can be performed on the large number in hexadecimals.
        
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberAdditionHexData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(entry)});

            // perform LHS + RHS
            BigNumber result = lhs + rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalSubtraction)
    {
        // Verify if Additions can be performed on the large number in decimals.
	
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberSubtractionData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(entry)});

            // perform  LHS - RHS
            BigNumber result = lhs - rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaSubtraction)
    {
        // Verify if Additions can be performed on the large number in hexadecimals.
	
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberSubtractionHexaData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(entry)});

            // perform LHS - RHS
            BigNumber result = lhs - rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalMultiple)
    {
        // Verify if multiple  can be performed on the large number in decimals.
    
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberMultipleData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(entry)});

            // perform LHS * RHS
            BigNumber result = lhs * rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
        }
    }

    
    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaDecimalMultiple)
    {
        // Verify if multiple  can be performed on the large number in decimals.
	
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberMultipleDataHexa )
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(entry)});

            // perform LHS * RHS
            BigNumber result = lhs * rhs;

            // check if the result is equal to the expected result.
            BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
        }
    }
    

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalDivision)
    {
        // Verify if division  can be performed on the large number in decimals.
    
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberDivisionDecimalData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(entry)});

            // perform LHS / RHS, expect an exception if the RHS is 0
            if (rhs.ToDec() == "0")
            {
                BOOST_CHECK_THROW(lhs / rhs, std::exception);
            }
            else
            {
                BigNumber result = lhs / rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
            }
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaDecimalDivision)
    {
        // Verify if division  can be performed on the large number in hexa decimals.
    
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberDivisionHexaDecimalData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(entry)});

            // perform LHS / RHS, expect an exception if the RHS is 0
            if (rhs.ToDec() == "0")
            {
                BOOST_CHECK_THROW(lhs / rhs, std::exception);
            }
            else
            {
                BigNumber result = lhs / rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
            }
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalLeftShift)
    {
        // Verify if left shift  can be performed on the large number in decimals.
    
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberLeftShiftData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(entry)});

            // perform  LHS << RHS, where expect a exception if the RHS is -ve
            if (rhs.ToDec().length() > 0 && rhs.ToDec()[0] == '-')
            {
                BOOST_CHECK_THROW(lhs << rhs, std::exception);
            }
            else
            {
                BigNumber result = lhs << rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
            }
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexDecimalLeftShift)
    {
        // Verify if left shift  can be performed on the large number in decimals.
    
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberLeftShiftHexaData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(entry)});

            // perform  LHS << RHS, where expect a exception if the RHS is -ve
            if (rhs.ToDec().length() > 0 && rhs.ToDec()[0] == '-')
            {
                BOOST_CHECK_THROW(lhs << rhs, std::exception);
            }
            else
            {
                BigNumber result = lhs << rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
            }
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersDecimalRightShift)
    {
        // Verify if right shift  can be performed on the large number in decimals.
    
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberRightShiftData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromDec({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromDec({std::get<1>(entry)});

            // perform  LHS >> RHS, where expect a exception if the RHS is -ve
            if (rhs.ToDec().length() > 0 && rhs.ToDec()[0] == '-')
            {
                BOOST_CHECK_THROW(lhs >> rhs, std::exception);
            }
            else
            {
                BigNumber result = lhs >> rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
            }
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersHexaDecimalRightShift)
    {
        // Verify if right shift  can be performed on the large number in decimals.
    
        // Iterate over Data for large number addition
        for (const auto & entry : testLargeNumberRightShiftHexaData)
        {
            BigNumber lhs, rhs;
            // get "LefthandSide(LHS)"
            lhs.FromHex({std::get<0>(entry)});
				
            // get "RightHandSide(RHS)"
            rhs.FromHex({std::get<1>(entry)});

            // perform  LHS >> RHS, where expect a exception if the RHS is -ve
            if (rhs.ToDec().length() > 0 && rhs.ToDec()[0] == '-')
            {
                BOOST_CHECK_THROW(lhs >> rhs, std::exception);
            }
            else
            {
                BigNumber result = lhs >> rhs;

                // check if the result is equal to the expected result.
                BOOST_CHECK_EQUAL(std::get<2>(entry), result.ToDec());
            }
        }
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersPreIncrementData)
    {
        // Verify if pre increment can be performed on the large number in decimals.
    
        for(const auto & entry : testPreIncrementData)
        {
            BigNumber val;
            val.FromDec({std::get<0>(entry)});
            BigNumber result = ++val;
            BOOST_CHECK_EQUAL(std::get<1>(entry), result.ToDec());
        }
   
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersPostIncrementData)
    {
        // Verify if post increment can be performed on the large number in decimals.
    
        for(const auto & entry : testPostIncrementData)
        {
            BigNumber val;
            val.FromDec(entry);
            BigNumber result = val++;
            BOOST_CHECK_EQUAL(entry, result.ToDec());
        }
   
    }


    BOOST_AUTO_TEST_CASE(testLargeNumbersPreDecrementData)
    {
        // Verify if pre decrement can be performed on the large number in decimals.
    
        for(const auto & entry : testPreDecrementData)
        {
            BigNumber val;
            val.FromDec({std::get<0>(entry)});
            BigNumber result = --val;
            BOOST_CHECK_EQUAL(std::get<1>(entry), result.ToDec());
        }
   
    }

    BOOST_AUTO_TEST_CASE(testLargeNumbersPostDecrementData)
    {
        // Verify if post decrement can be performed on the large number in decimals.
    
        for(const auto & entry : testPostDecrementData)
        {
            BigNumber val;
            val.FromDec(entry);
            BigNumber result = val--;
            BOOST_CHECK_EQUAL(entry, result.ToDec());
        }
   
    }

    BOOST_AUTO_TEST_SUITE_END();
}
