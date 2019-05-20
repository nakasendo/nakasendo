#################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

## Python requires a file __init__.py to recognize the directory contains py modules
if(CMAKE_CONFIGURATION_TYPES)
  file(TOUCH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/__init__.py")
  file(TOUCH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/__init__.py")
  install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/__init__.py" CONFIGURATIONS Debug DESTINATION lib COMPONENT Files)
  install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/__init__.py" CONFIGURATIONS Release DESTINATION lib COMPONENT Files)

  set(PY_MODULES_RUNTIME_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PyModuleLoadHelper.py.in" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/PyModuleLoadHelper.py")
  set(PY_MODULES_RUNTIME_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PyModuleLoadHelper.py.in" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/PyModuleLoadHelper.py")
else()
  file(TOUCH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/__init__.py")
  install(FILES "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/__init__.py" DESTINATION lib COMPONENT Files)

  set(PY_MODULES_RUNTIME_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PyModuleLoadHelper.py.in" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/PyModuleLoadHelper.py")
endif()

add_subdirectory(PyAsymKey)
add_subdirectory(PyBigNumbers)
add_subdirectory(PyMessageHash)
add_subdirectory(PySymEncDec)

## build wasm modules
## See ${CMAKE_CURRENT_SOURCE_DIR}/WasmTools.cmake header to see how to install/setup for WASM
if(NOT DEFINED ENV{OPENSSL_WASM_ROOT_DIR})
  message(STATUS "SDK WARNS : emsdk and openssl-wasm is not installed, modules for WASM will not be built")
else()
  include(WasmTools.cmake)
  sdkHelpFindEMCC()
  if(NOT emcc_FOUND)
    message(STATUS "SDK WARNS : emcc not found, modules for WASM will not be built. Try to add the location of emcc to environment variable PATH")
  else()
    build_wasm_modules(${SDK_CORE_COMPONENT_LIST})
  endif()
endif()
