#################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

if(NOT Boost_FOUND)
  message(STATUS "SDK WARNS : Boost was not found on your system, unit tests will not be built")
  return()
endif()

macro(sdk_add_unit_test _targetName)
  if(NOT TARGET ${_targetName})
      message(FATAL_ERROR " [${_targetName}] is not a TARGET")
  endif()
  sdkLinkTargetToBoost(${_targetName} Boost::unit_test_framework)
  add_test(NAME ${_targetName} COMMAND $<TARGET_FILE:${_targetName}> --log_level=error)
endmacro()

add_executable(test_BigNumbers test_BigNumbers.cpp)
target_link_libraries(test_BigNumbers BigNumbers)
set_property(TARGET test_BigNumbers PROPERTY FOLDER "tests/unit")
sdk_add_unit_test(test_BigNumbers)

add_executable(test_MessageHash test_MessageHash.cpp)
target_link_libraries(test_MessageHash MessageHash)
set_property(TARGET test_MessageHash PROPERTY FOLDER "tests/unit")
sdk_add_unit_test(test_MessageHash)