#################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(_ADDITIONAL_CPP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/PyBigNumbers.cpp)
build_py_module(BigNumbers "${_ADDITIONAL_CPP_FILE}")