#################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

if(NOT Python_FOUND)
  message(STATUS "SDK WARNS : Python was not found on your system, module MessageHash for python will not be built")
  return()
endif()

set(_PY_VERSION_POSTFIX "${Python_VERSION_MAJOR}${Python_VERSION_MINOR}")
set(_PYTHON_MODULE_NAME MessageHashPython${_PY_VERSION_POSTFIX}API)
add_library(${_PYTHON_MODULE_NAME} SHARED MessageHashPythonAPI.cpp)## Python module should be shared library
target_link_libraries(${_PYTHON_MODULE_NAME} MessageHash Python::Python)
set_property(TARGET ${_PYTHON_MODULE_NAME} PROPERTY FOLDER "modules")


