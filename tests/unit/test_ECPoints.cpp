/// Define test module name with debug postfix
#ifdef NDEBUG 
#  define BOOST_TEST_MODULE test_ECPoints
#else
#  define BOOST_TEST_MODULE test_ECPointsd
#endif

#include <boost/test/unit_test.hpp>

#include <ECPoint/ECPoint.h>
#include <BigNumbers/BigNumbers.h>
#include <iostream>
#include <memory>
#include <tuple>
BOOST_AUTO_TEST_SUITE(test_suite_ECPoints)

/*test case : test the addition of ECPoints, and compare the result*/
BOOST_AUTO_TEST_CASE(testECPointAdditionAndComparasion)
{
    
    ECPoint ec1, ec2, ec3;
    ec1.SetRandom();
    ec2.SetRandom();
    ec3.SetRandom();
    BOOST_CHECK((( ec1 + ec2) + ec3) == (ec1 + (ec2 + ec3)));
}

/* test case : test the != operator of ECPoint */
BOOST_AUTO_TEST_CASE(testECPointComparasion1)
{
    ECPoint ec1, ec2;
    ec1.FromHex("03A519004B8A4222CF06E5BFA825266E0DCC6A9C99E408D043A2D0D1C33AE8895F");
    ec2.FromHex("025F0F2C3C4A73FA39DA5E444481BC43CAB388E174517C2F63E1E8BFC1036D66B1");
    BOOST_CHECK(ec1 != ec2);
    BOOST_CHECK(ec1+ec2 == ec2+ec1);
}

/* test case : test the point on curve */
BOOST_AUTO_TEST_CASE(testECPointCheckPointOnCurve)
{
    ECPoint ec1;
    ec1.SetRandom();
    BOOST_CHECK(ec1.CheckOnCurve());
}

/* test case : test the point is at infinity */
BOOST_AUTO_TEST_CASE(testECPointCheckInfinity)
{
    ECPoint ec1;
    ec1.SetRandom();
    BOOST_CHECK(!ec1.CheckInfinity());
}


/* test case : test NID constructors */
BOOST_AUTO_TEST_CASE(testECPointNIDConstructors)
{
    std::vector<std::tuple<int, std::string, std::string>> curveList = getCurveList();
    for (auto nidTuple : curveList)
    {
        ECPoint ec1(std::get<0>(nidTuple));
        ECPoint ec2(std::get<1>(nidTuple));
    }
}

BOOST_AUTO_TEST_CASE(testECPointGeneratorForAllCurves)
{
    std::vector<std::tuple<int, std::string, std::string>> curveList = getCurveList();
    for (auto nidTuple : curveList)
    {
        ECPoint ec1(std::get<0>(nidTuple));
        ec1.getGenerator();
    }
}

BOOST_AUTO_TEST_CASE(testECPointGroupDegreeForAllCurves)
{
    std::vector<std::tuple<int, std::string, std::string>> curveList = getCurveList();
    for (auto nidTuple : curveList)
    {
        ECPoint ec1(std::get<0>(nidTuple));
        ec1.getECGroupDegree();
    }
}


BOOST_AUTO_TEST_CASE(testECPointGroupOrderForAllCurves)
{
    std::vector<std::tuple<int, std::string, std::string>> curveList = getCurveList();
    for (auto nidTuple : curveList)
    {
        ECPoint ec1(std::get<0>(nidTuple));
        ec1.getECGroupOrder().ToDec();
    }
}

/* test case : to test the copy Constructor of ECPoint */
BOOST_AUTO_TEST_CASE(testECPointCopyConstructor)
{
    ECPoint ec1;
    ec1.SetRandom();
    ECPoint ec2(ec1);
    BOOST_CHECK(ec1 == ec2);
}

/*test case : to test the copy assignment operator of ECPoint*/
BOOST_AUTO_TEST_CASE(testECPointCopyAssignment)
{
    ECPoint ec1;
    ECPoint ec2;
    ec1.SetRandom();
    ec2.SetRandom();
    ec2 = ec1;
    BOOST_CHECK(ec1 == ec2);
}

/*test case : to test the double of ECPoint*/
BOOST_AUTO_TEST_CASE(testECPointDouble)
{
    ECPoint ec1;
    ec1.SetRandom();
    ECPoint ec2 = ec1;
    BOOST_CHECK(ec1+ec1 == ec2.Double());
    BOOST_CHECK(ec2.Double().CheckOnCurve());
}

/*test case : to test the invert of ECPoint*/
BOOST_AUTO_TEST_CASE(testECPointInvert)
{
    ECPoint ec1;
    ec1.SetRandom();
    ECPoint ec2 = ec1;
    ec1.Invert();
    BOOST_CHECK(ec1 != ec2);
    BOOST_CHECK(ec1.ToHex().compare(2, std::string::npos, ec2.ToHex(), 2, std::string::npos) == 0);
    BOOST_CHECK(ec1.CheckOnCurve());
}

/* test case : to test getCurveList and getNidForString  */
BOOST_AUTO_TEST_CASE(testECPointGetCurveList)
{
    std::vector<std::tuple<int, std::string, std::string>> curveList = getCurveList();

    // verify the test data
    for (auto nidTuple : curveList)
    {
	BOOST_CHECK(getNidForString(std::get<1>(nidTuple)) == std::get<0>(nidTuple));
    }
}

/*test case : to test the Multiply Hex of ECPoint*/
BOOST_AUTO_TEST_CASE(testECPointMulHex)
{
    ECPoint ec1;
    ec1.SetRandom();
    
    BigNumber bnm , bnn;
    bnm.generateRandHex(1024);
    bnn.generateRandHex(1024);

    ECPoint ec2 = ec1.MulHex(bnm.ToHex(), bnn.ToHex());
    BOOST_CHECK(ec2.CheckOnCurve());

    std::string es;
    ECPoint ec3 = ec1.MulHex(bnm.ToHex(), es);
    BOOST_CHECK(ec3.CheckOnCurve());
}


/*test case : to test the Multiply Dec of ECPoint*/
BOOST_AUTO_TEST_CASE(testECPointMulDec)
{
    ECPoint ec1;
    ec1.SetRandom();
    
    BigNumber bnm , bnn;
    bnm.generateRandDec(1024);
    bnn.generateRandDec(1024);

    ECPoint ec2 = ec1.MulDec(bnm.ToHex(), bnn.ToHex());
    BOOST_CHECK(ec1.CheckOnCurve());
}

BOOST_AUTO_TEST_CASE(testECPointSetAffineCoordinatesConstructor)
{
    ECPoint ec1;
    ec1.SetRandom();
    std::pair<std::string, std::string> coord = ec1.GetAffineCoords_GFp();
    BigNumber bnX, bnY;
    bnX.FromHex(coord.first);
    bnY.FromHex(coord.second);
    ECPoint ec2(bnX, bnY);

    BOOST_CHECK(ec1 == ec2);
}


BOOST_AUTO_TEST_SUITE_END();
