#define BOOST_TEST_MODULE test_ECPoints
#include <boost/test/unit_test.hpp>

#include <ECPoint/ECPoint.h>
#include <iostream>
#include <memory>
#include <tuple>
BOOST_AUTO_TEST_SUITE(test_suite_ECPoints)

/*test case : test the addition of ECPoints, and compare the result*/
BOOST_AUTO_TEST_CASE(testECPointAdditionAndComparasion)
{
    
    ECPoint ec1, ec2, ec3;
	
    if ((( ec1 + ec2) + ec3) == (ec1 + (ec2 + ec3)))
    {
        BOOST_TEST_MESSAGE("ECPoint Addition passed");
    }
    else
    {
        BOOST_FAIL("ECPoint Addition failed");
    }
}

/* test case : test the != operator of ECPoint */
BOOST_AUTO_TEST_CASE(testECPointComparasion1)
{
    ECPoint ec1, ec2;

	
    if ( ec1 != ec2)
    {
        BOOST_FAIL("ECPoint Addition failed");
    }
    else
    {
        BOOST_TEST_MESSAGE("ECPoint Addition passed");
    }
}

/* test case : test NID constructors */
BOOST_AUTO_TEST_CASE(testECPointNIDConstructors)
{
    std::string currentNIDString;
    std::vector<std::tuple<int, std::string, std::string>> curveList = getCurveList();
    try 
    {
        for (auto nidTuple : curveList)
        {
            ECPoint ec1(std::get<0>(nidTuple));
            ECPoint ec2(std::get<1>(nidTuple));
	    currentNIDString = std::get<1>(nidTuple);
        }
    }
    catch (std::exception & e)
    {
        BOOST_FAIL("Unable to create ECPoint(\""<< currentNIDString << "\")" );
    }
}

/* test case : to test the copy Constructor of ECPoint */
BOOST_AUTO_TEST_CASE(testECPointCopyConstructor)
{
    ECPoint ec1;
    ECPoint ec2(ec1);

	
    if (ec1 == ec2)
    {
        BOOST_TEST_MESSAGE("ECPoint Addition passed");
    }
    else
    {
        BOOST_FAIL("ECPoint Addition failed");
    }
}

/*test case : to test the copy assignment operator of ECPoint*/
BOOST_AUTO_TEST_CASE(testECPointCopyAssignment)
{
    ECPoint ec1;
    ECPoint ec2;

    ec2 = ec1;
	
    if (ec1 == ec2)
    {
        BOOST_TEST_MESSAGE("ECPoint Addition passed");
    }
    else
    {
        BOOST_FAIL("ECPoint Addition failed");
    }
}

/* test case : to test getCurveList and getNidForString  */
BOOST_AUTO_TEST_CASE(testECPointGetCurveList)
{
    std::vector<std::tuple<int, std::string, std::string>> curveList = getCurveList();

    // verify the test data
    for (auto nidTuple : curveList)
    {
        if (getNidForString(std::get<1>(nidTuple)) != std::get<0>(nidTuple))
	{
            BOOST_FAIL("NID(" << std::get<0>(nidTuple) << ") doesn't match "<< std::get<1>(nidTuple));
	}
    }

}

BOOST_AUTO_TEST_SUITE_END();
