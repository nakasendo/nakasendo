#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(BIG_NUMBERS_HPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumAPI.h
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbersImpl.h
)

set(BIG_NUMBERS_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumAPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbersImpl.cpp
)

set(BIG_NUMBERS_SOURCE_FILES ${BIG_NUMBERS_HPP_FILES} ${BIG_NUMBERS_CPP_FILES})
add_library(BigNumbers SHARED ${BIG_NUMBERS_SOURCE_FILES})
target_link_libraries(BigNumbers OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS})
set_property(TARGET BigNumbers PROPERTY FOLDER "core")

## Add tests. TODO separate to tests directory
add_executable(BigNumbersClient BigNumbersClient.cpp)
target_link_libraries(BigNumbersClient BigNumbers)
set_property(TARGET BigNumbersClient PROPERTY FOLDER "tests")

## Add tests. TODO separate to tests directory
add_executable(BigNumbersRandTest BigNumbersRandTest.cpp)
target_link_libraries(BigNumbersRandTest BigNumbers)
set_property(TARGET BigNumbersRandTest PROPERTY FOLDER "tests")