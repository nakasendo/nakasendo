/// Define test module name with debug postfix
#ifdef NDEBUG 
#  define BOOST_TEST_MODULE test_Protobuf
#else
#  define BOOST_TEST_MODULE test_Protobufd
#endif

#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>

#include <google/protobuf/util/time_util.h>
#include "TestMessageDefinition.pb.h"

//// These test cases was taken from $PROTOBUF_SRC/examples
//// TODO : Modify the file TestMessageDefinition and write test cases appropriatedly
BOOST_AUTO_TEST_SUITE(test_suite_Protobuf)

BOOST_AUTO_TEST_CASE(test_case0)
{
    tutorial::Person person;
    person.set_name("try to set name");
    person.set_id(100);
    person.set_email( "try to set email");
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(test_case1)
{
    tutorial::AddressBook adress_book;
    BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
