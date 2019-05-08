#################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

include(PyTools.cmake)
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
    build_wasm_modules(BigNumbers MessageHash)
  endif()
endif()
