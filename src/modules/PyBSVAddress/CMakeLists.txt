#################################################################
#  Date             06/08/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(_PY_MODULE_NAME PyBSVAddress)
set(_PY_MODULE_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/PyBSVAddress.cpp)

Python_add_library(${_PY_MODULE_NAME} ${_PY_MODULE_SOURCE_FILES})
target_link_libraries(${_PY_MODULE_NAME} PRIVATE BSVAddress)
if(CMAKE_CONFIGURATION_TYPES)
  set_target_properties(${_PY_MODULE_NAME} PROPERTIES FOLDER "modules" DEBUG_POSTFIX "_d")# On dual build mode system, need to add postfix _d
else()
  set_target_properties(${_PY_MODULE_NAME} PROPERTIES DEBUG_POSTFIX "")# Works well on Linux debug mode
endif()

install(TARGETS ${_PY_MODULE_NAME} DESTINATION "lib" COMPONENT PyModules)
