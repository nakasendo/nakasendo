#include <boost/test/unit_test.hpp>
#include <BigNumbers/BigNumbers.h>
#include <iostream>
#include <memory>

bool check_boundness(const std::vector<BigNumber>& crRandValues, const BigNumber& crLowerBound, const BigNumber& crUpperBound)
{
    for (auto bn : crRandValues) // Check the bound
        if (bn < crLowerBound || bn > crUpperBound)
            return false;
    return true;
}

template<typename T>
bool check_randomness(const std::vector<T>& crRandValues)
{
    // Bruteforce pairwise check
    for (size_t i = 0; i < crRandValues.size(); ++i)
    {
        const T& ran_i = crRandValues[i];
        for (size_t j = i + 1; j < crRandValues.size(); ++j)
        {
            const T& ran_j = crRandValues[j];
            if (ran_i == ran_j)
                return false;
        }
    }
    return true;
}

BOOST_AUTO_TEST_SUITE(test_suite_BigNumbersRandom)

BOOST_AUTO_TEST_CASE(test_random_string_from_Bignumber)
{
    const int nsize = 128;
    std::vector<std::string> vect_hex;
    std::vector<std::string> vect_dec;
    for (size_t i = 0; i < 10; ++i)
    {
        BigNumber bn;
        const std::string rand_hex = bn.generateRandHex(nsize);
        vect_hex.push_back(rand_hex);
        const std::string rand_dec = bn.generateRandDec(nsize);
        vect_dec.push_back(rand_dec);
        BOOST_CHECK(rand_hex.size() == 32);
        BOOST_CHECK(rand_dec.size() > 0);
    }
    BOOST_CHECK(check_randomness(vect_hex));
    BOOST_CHECK(check_randomness(vect_dec));
}

BOOST_AUTO_TEST_CASE(test_basic_BigNumbers_Random_cpp)
{
    const size_t nbGen = 50;
    BigNumber lowerbound; lowerbound.FromDec("0");
    BigNumber upperbound; upperbound.FromDec("10000000000");
    std::vector<BigNumber> randValues;
    for (size_t i = 0; i < nbGen; ++i)
    {
        BigNumber randBN = GenerateRandRange(lowerbound, upperbound);
        randValues.push_back(randBN);
    }

    BOOST_CHECK(check_boundness(randValues, lowerbound, upperbound));
    BOOST_CHECK(check_randomness(randValues));
}

BOOST_AUTO_TEST_CASE(test_basic_BigNumbers_Prime_Random_cpp)
{
    const size_t nbGen = 50;
    std::vector<BigNumber> randPrimeValues;
    for (size_t i = 0; i < nbGen; ++i)
    {
        BigNumber randPrime = GenerateRandPrime();
        BOOST_CHECK(randPrime.isPrime());
        BOOST_CHECK(randPrime.isPrimeFasttest());
        randPrimeValues.push_back(randPrime);
    }
    BOOST_CHECK(check_randomness(randPrimeValues));
}

BOOST_AUTO_TEST_CASE(test_basic_BigNumbers_Prime_Random_API)
{
    //TODO test all the API functionalities
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()