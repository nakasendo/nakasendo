################################################################
#  Date             15/08/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

file(GLOB py_example_files "${CMAKE_CURRENT_SOURCE_DIR}/*.py")

foreach(_py_file ${py_example_files})
  install(FILES ${_py_file} DESTINATION "example/python" COMPONENT "Examples python")
endforeach()