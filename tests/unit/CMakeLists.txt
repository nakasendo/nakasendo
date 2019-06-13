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

add_executable(test_AsymKey test_AsymKey.cpp)
target_link_libraries(test_AsymKey AsymKey)
set_property(TARGET test_AsymKey PROPERTY FOLDER "tests/unit")
sdk_add_unit_test(test_AsymKey)

add_executable(test_BigNumbers test_BigNumbers.cpp test_BigNumbersRandom.cpp)
target_link_libraries(test_BigNumbers BigNumbers)
set_property(TARGET test_BigNumbers PROPERTY FOLDER "tests/unit")
sdk_add_unit_test(test_BigNumbers)

add_executable(test_MessageHash test_MessageHash.cpp)
target_link_libraries(test_MessageHash MessageHash)
set_property(TARGET test_MessageHash PROPERTY FOLDER "tests/unit")
sdk_add_unit_test(test_MessageHash)

add_executable(test_SymEncDec test_SymEncDec.cpp)
target_link_libraries(test_SymEncDec SymEncDec)
set_property(TARGET test_SymEncDec PROPERTY FOLDER "tests/unit")
sdk_add_unit_test(test_SymEncDec)

add_executable(test_ECPoint test_ECPoints.cpp)
target_link_libraries(test_ECPoint ECPoint)
target_link_libraries(test_ECPoint BigNumbers)
set_property(TARGET test_ECPoint PROPERTY FOLDER "tests/unit")
sdk_add_unit_test(test_ECPoint)

add_executable(test_GroupPlayers test_GroupPlayer.cpp)
target_link_libraries(test_GroupPlayers ThresholdSignatures)
set_property(TARGET test_GroupPlayers PROPERTY FOLDER "tests/unit")
sdk_add_unit_test(test_GroupPlayers)
