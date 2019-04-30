#ifndef _NCHAIN_TEST_UTILS_H_
#define _NCHAIN_TEST_UTILS_H_

#include <string>
#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>
#include <ctime>
using namespace std;
namespace unitTests
{

    #define NCHAIN_TEST_TIME_START(LABEL) BOOST_TEST_MESSAGE( #LABEL ); std::clock_t c_start = std::clock();
    #define NCHAIN_TEST_TIME_STOP  std::clock_t c_end = std::clock(); BOOST_TEST_MESSAGE(std::to_string(1000.0 * (c_end-c_start) / CLOCKS_PER_SEC));
	
}
#endif  // _NCHAIN_TEST_UTILS_H_
