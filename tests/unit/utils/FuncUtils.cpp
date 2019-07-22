
#include "FuncUtils.h"
#include <random>
#include <chrono>

unsigned  getRandomNumberInRange(int start, int end)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(start, end); // define the range
    return distr(eng);
}
std::string  getRandomString(const unsigned size)
{
    std::string randomString;

    // get the current time in epoch format
    std::default_random_engine randomEngine(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));

    // produce random integer value, with mean 127, distributed according to poisson distribution function
    std::poisson_distribution<int> distP(sizeof(alphanumbers) / 2);

    for (unsigned i = 0; i < size; i++)
    {
        // generates the next random char
        randomString.push_back(alphanumbers[distP(randomEngine) % sizeof(alphanumbers)]);
    }
    return randomString;
}

std::string getRandomNumberString(const unsigned size)
{
    std::string randomString;

    // get the current time in epoch format
    std::default_random_engine randomEngine(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));

    // produce random integer value, with mean 127, distributed according to poisson distribution function
    std::poisson_distribution<int> distP(5);

    for (unsigned i = 0; i < size; i++)
    {
        // generates the next random char
        randomString.push_back(alphanumbers[distP(randomEngine) % 10]);
    }
    return randomString;
}

