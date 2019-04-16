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
add_library(BigNumbers ${LIBS_BUILD_TYPE} ${BIG_NUMBERS_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
  target_link_libraries(BigNumbers crypt32 ws2_32)
endif()
target_link_libraries(BigNumbers OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(BigNumbers PRIVATE EXPORT_BigNumbers)
set_property(TARGET BigNumbers PROPERTY FOLDER "core")
#########################################################################################
#########################################################################################

## Build python module  #########################################################################################
if(Python_FOUND)
  set(_PY_VERSION_POSTFIX "${Python_VERSION_MAJOR}${Python_VERSION_MINOR}")
  set(_PYTHON_MODULE_NAME BigNumPython${_PY_VERSION_POSTFIX}API)
  add_library(${_PYTHON_MODULE_NAME} SHARED BigNumPythonAPI.cpp)## Python module should be shared library
  target_link_libraries(${_PYTHON_MODULE_NAME} BigNumbers Python::Python)
  set_property(TARGET ${_PYTHON_MODULE_NAME} PROPERTY FOLDER "modules")
else()
  message(STATUS "SDK WARNS : Python was not found on your system, module BigNumbers for python will not be built")
endif()


## Add tests. TODO separate to tests directory #########################################################################################
add_executable(BigNumbersClient BigNumbersClient.cpp)
target_link_libraries(BigNumbersClient BigNumbers)
set_property(TARGET BigNumbersClient PROPERTY FOLDER "tests")

## Add tests. TODO separate to tests directory #########################################################################################
add_executable(BigNumbersRandTest BigNumbersRandTest.cpp)
target_link_libraries(BigNumbersRandTest BigNumbers)
set_property(TARGET BigNumbersRandTest PROPERTY FOLDER "tests")
