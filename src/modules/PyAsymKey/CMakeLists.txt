#################################################################
#  Date             10/05/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(_ADDITIONAL_CPP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/PyAsymKey.cpp)
build_py_module(AsymKey "${_ADDITIONAL_CPP_FILE}")