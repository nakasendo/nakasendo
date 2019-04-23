#################################################################
#  Date             15/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(MESSAGE_HASH_HPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashAPI.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHash.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashImpl.h
)

set(MESSAGE_HASH_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashAPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHash.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashImpl.cpp
)

set(MESSAGE_HASH_SOURCE_FILES ${MESSAGE_HASH_HPP_FILES} ${MESSAGE_HASH_CPP_FILES})
add_library(MessageHash ${LIBS_BUILD_TYPE} ${MESSAGE_HASH_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
  target_link_libraries(MessageHash crypt32 ws2_32)
endif()
target_link_libraries(MessageHash OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(MessageHash PRIVATE EXPORT_MessageHash)
set_property(TARGET MessageHash PROPERTY FOLDER "core")
#########################################################################################
#########################################################################################

## Build python module  #########################################################################################
if(Python_FOUND)
  set(_PY_VERSION_POSTFIX "${Python_VERSION_MAJOR}${Python_VERSION_MINOR}")
  set(_PYTHON_MODULE_NAME MessageHashPython${_PY_VERSION_POSTFIX}API)
  add_library(${_PYTHON_MODULE_NAME} SHARED MessageHashPythonAPI.cpp)## Python module should be shared library
  target_link_libraries(${_PYTHON_MODULE_NAME} MessageHash Python::Python)
  set_property(TARGET ${_PYTHON_MODULE_NAME} PROPERTY FOLDER "modules")
else()
  message(STATUS "SDK WARNS : Python was not found on your system, module MessageHash for python will not be built")
endif()


## Add tests. TODO separate to tests directory #########################################################################################
add_executable(MessageHashClient MessageHashClient.cpp)
target_link_libraries(MessageHashClient MessageHash)
set_property(TARGET MessageHashClient PROPERTY FOLDER "tests")
