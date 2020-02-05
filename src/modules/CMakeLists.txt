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
add_subdirectory(PyBSVAddress)
add_subdirectory(PyNakasendo)

if(BUILD_WEBASSEMBLY)
## mocha test_Module.js -r path/to/moduleloader.js
if(CMAKE_CONFIGURATION_TYPES)
  set(SDK_JS_MODULE_LOADER_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/JsModuleLoader.js" CACHE PATH "Path to javascript module loader for Debug")
  set(SDK_JS_MODULE_LOADER_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/JsModuleLoader.js" CACHE PATH "Path to javascript module loader for Release")

  string(REGEX REPLACE "/" "\\\\\\\\" JS_MODULE_PATH_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
  string(REGEX REPLACE "/" "\\\\\\\\" JS_MODULE_PATH_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

  file(WRITE  "${SDK_JS_MODULE_LOADER_DEBUG}" "global.SDK_MODULE_PATH = '${JS_MODULE_PATH_DEBUG}';\nglobal.SDK_DEBUG_POSTFIX = 'd';")
  file(WRITE  "${SDK_JS_MODULE_LOADER_RELEASE}" "global.SDK_MODULE_PATH = '${JS_MODULE_PATH_RELEASE}';\nglobal.SDK_DEBUG_POSTFIX = '';")
else()
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(_debug_postfix d)
  endif()
  set(SDK_JS_MODULE_LOADER "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/JsModuleLoader.js" CACHE PATH "Path to javascript module loader")
  file(WRITE  "${SDK_JS_MODULE_LOADER}" "global.SDK_MODULE_PATH = '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}';\nglobal.SDK_DEBUG_POSTFIX = '${_debug_postfix}';")
endif()

add_subdirectory(JsAsymKey)
add_subdirectory(JsBigNumbers)
add_subdirectory(JsMessageHash)
#add_subdirectory(JsSymEncDec)
add_subdirectory(JsECPoint)
add_subdirectory(JsPolynomial)
endif(BUILD_WEBASSEMBLY)
