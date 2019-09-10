#################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

## Python requires a file __init__.py to recognize the directory contains py modules
if(CMAKE_CONFIGURATION_TYPES)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/__init__.py")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/__init__.py")
  install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/__init__.py" CONFIGURATIONS Debug DESTINATION lib COMPONENT PyModules)
  install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/__init__.py" CONFIGURATIONS Release DESTINATION lib COMPONENT PyModules)
else()
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/__init__.py.in" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/__init__.py")
  install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/__init__.py" DESTINATION lib COMPONENT PyModules)
endif()

add_subdirectory(PyAsymKey)
add_subdirectory(PyBigNumbers)
add_subdirectory(PyMessageHash)
add_subdirectory(PySymEncDec)
add_subdirectory(PyECPoint)
add_subdirectory(PyPolynomial)
add_subdirectory(PyNakasendo)

#add_subdirectory(JsAsymKey)
#add_subdirectory(JsBigNumbers)
#add_subdirectory(JsMessageHash)
#add_subdirectory(JsSymEncDec)
#add_subdirectory(JsECPoint)
#add_subdirectory(JsPolynomial)
#add_subdirectory(JsNakasendo)