#################################################################
#  Date             09/09/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
## Include guard
if(FindEmsdkHelper_Include)
  return()
endif()
set(FindEmsdkHelper_Include TRUE)

## Emsdk is a tools set helping to build code into webassembly modules. Follow the below guid to install
##   https://emscripten.org/docs/getting_started/downloads.html
## Note that we are using the version 1.38.0, so replace the 'latest' version in the guid by the correct version
## Essential components that are required are (they all need to be compatible of 1.28.0):
##    clang
##    emscripten
##    node (need to install modules mocha, mocha-junit-reporter)
##    binaryen (not required  but usefull tools)
##
##    openssl need to be rebuilt with emsdk tools to make a wasm openssl libraries, then the environment variable
##    OPENSSL_WASM_ROOT_DIR need to be defined pointing to the root of this newly built library
##
## The cmake helper will find the two fundamental tools to compile and link :
##    ${emcc_EXECUTABLE}
##    ${emar_EXECUTABLE}
##    ${emcc_INCLUDE_DIR}
##    ${node_EXECUTABLE}
##    ${mocha_EXECUTABLE}
## emcc will know how to find all other tools  through emsdk environment variables. It find the openssl wasm library
##    ${SDK_OPENSSL_WASM_ROOT_DIR}
##    ${SDK_OPENSSL_WASM_INCLUDE_DIR}
##    ${SDK_OPENSSL_WASM_LIBRARY_DIR}
##    ${SDK_WASM_OPENSSL_LIB_FILE}
##    ${SDK_WASM_CRYPTO_LIB_FILE}
##


#### Help to find Emsdk. It will prefer python3
macro(sdkHelpFindEMSDK)########################################################################################
  if(NOT DEFINED ENV{OPENSSL_WASM_ROOT_DIR})
    message(FATAL_ERROR "SDK ERROR : openssl-wasm binaries not found. Define environment variable OPENSSL_WASM_ROOT_DIR pointing to the installed wasm-openssl.")
  else()
    file(TO_CMAKE_PATH "$ENV{OPENSSL_WASM_ROOT_DIR}" _openssl_wasm_root_dir)## Normalize native path to unix path
    set(SDK_OPENSSL_WASM_ROOT_DIR "${_openssl_wasm_root_dir}" CACHE PATH "Location of openssl compiled by EMSDK")
    set(SDK_OPENSSL_WASM_INCLUDE_DIR "${SDK_OPENSSL_WASM_ROOT_DIR}/include" CACHE PATH "Include directory for openssl wasm")
    set(SDK_OPENSSL_WASM_LIBRARY_DIR "${SDK_OPENSSL_WASM_ROOT_DIR}/lib" CACHE PATH "Library directory for openssl wasm")
  endif()

  find_program(emcc_EXECUTABLE NAMES emcc PATH_SUFFIXES bin)
  find_program(emar_EXECUTABLE NAMES emar PATH_SUFFIXES bin)

  get_filename_component(_emscripten_dir "${emcc_EXECUTABLE}" DIRECTORY)
  set(emcc_INCLUDE_DIR "${_emscripten_dir}/system/include")

  if(WIN32)
    find_program(node_EXECUTABLE NAMES node.exe PATH_SUFFIXES bin)
    find_program(mocha_EXECUTABLE NAMES mocha.cmd PATH_SUFFIXES bin)
   else()
    find_program(node_EXECUTABLE NAMES node PATH_SUFFIXES bin)
    find_program(mocha_EXECUTABLE NAMES mocha PATH_SUFFIXES bin)
   endif()

  #find_library(SDK_WASM_OPENSSL_LIB NAMES libssl.a ssl PATHS ${SDK_OPENSSL_WASM_ROOT_DIR} PATH_SUFFIXES lib)
  #find_library(SDK_WASM_CRYPTO_LIB NAMES libcrypto.a crypto PATHS ${SDK_OPENSSL_WASM_ROOT_DIR} PATH_SUFFIXES lib)
  find_file(SDK_WASM_OPENSSL_LIB_FILE NAMES libssl.a PATHS ${SDK_OPENSSL_WASM_LIBRARY_DIR})
  find_file(SDK_WASM_CRYPTO_LIB_FILE NAMES libcrypto.a PATHS ${SDK_OPENSSL_WASM_LIBRARY_DIR})


  # handle the QUIETLY and REQUIRED arguments and set EMSDK_FOUND to TRUE if
  # all listed variables are TRUE
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args (EMSDK FOUND_VAR EMSDK_FOUND
                                     REQUIRED_VARS
                                     emcc_EXECUTABLE
                                     emcc_INCLUDE_DIR
                                     emar_EXECUTABLE
                                     SDK_OPENSSL_WASM_ROOT_DIR
                                     SDK_OPENSSL_WASM_INCLUDE_DIR
                                     SDK_OPENSSL_WASM_LIBRARY_DIR
                                     SDK_WASM_OPENSSL_LIB_FILE
                                     SDK_WASM_CRYPTO_LIB_FILE
  )

  mark_as_advanced(EMSDK_FOUND emcc_EXECUTABLE emar_EXECUTABLE)
endmacro()

function(sdkPrintEmsdkInfo)
  message(" --")
  message(" ---------- EMSDK_FOUND [${EMSDK_FOUND}]")
  message(" ---------- emcc_EXECUTABLE [${emcc_EXECUTABLE}]")
  message(" ---------- emar_EXECUTABLE [${emar_EXECUTABLE}]")
  message(" ---------- emcc_INCLUDE_DIR [${emcc_INCLUDE_DIR}]")
  message(" ---------- node_EXECUTABLE [${node_EXECUTABLE}]")
  message(" ---------- mocha_EXECUTABLE [${mocha_EXECUTABLE}]")
  message(" ---------- SDK_OPENSSL_WASM_ROOT_DIR [${SDK_OPENSSL_WASM_ROOT_DIR}]")
  message(" ---------- SDK_OPENSSL_WASM_INCLUDE_DIR [${SDK_OPENSSL_WASM_INCLUDE_DIR}]")
  message(" ---------- SDK_OPENSSL_WASM_LIBRARY_DIR [${SDK_OPENSSL_WASM_LIBRARY_DIR}]")
  message(" ---------- SDK_WASM_OPENSSL_LIB_FILE [${SDK_WASM_OPENSSL_LIB_FILE}]")
  message(" ---------- SDK_WASM_CRYPTO_LIB_FILE [${SDK_WASM_CRYPTO_LIB_FILE}]")
  ##message(" ---------- EMSDK_VERSION [${EMSDK_VERSION}]") ## todo
endfunction()

#################################################################################################################################
##### A few helper functions when building wasm targets
function(wasm_get_debug_postfix DebugPostFix)
  if(CMAKE_CONFIGURATION_TYPES)
    set(_debug_postfix "\$<IF:\$<CONFIG:Debug>,d,>")
  else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      set(_debug_postfix ${CMAKE_DEBUG_POSTFIX})
    endif()
  endif()
  set(${DebugPostFix} "${_debug_postfix}" PARENT_SCOPE)
endfunction()

function(wasm_get_output_dir OutputFileDir)
  if(CMAKE_CONFIGURATION_TYPES)
    set(_outputfiledir "\$<IF:\$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>")
  else()
    set(_outputfiledir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  endif()
  set(${OutputFileDir} "${_outputfiledir}" PARENT_SCOPE)
endfunction()

function(wasm_get_js_file_name OutputFileName ModuleTargetName)
  wasm_get_debug_postfix(_debug_postfix)
  set(_outputfilename "Js${ModuleTargetName}${_debug_postfix}.js")
  set(${OutputFileName} "${_outputfilename}" PARENT_SCOPE)
endfunction()

function(wasm_get_emslib_file_name OutputFileName ModuleTargetName)
  wasm_get_debug_postfix(_debug_postfix)
  set(_outputfilename "libWASM${ModuleTargetName}${_debug_postfix}.a")
  set(${OutputFileName} "${_outputfilename}" PARENT_SCOPE)
endfunction()

function(wasm_get_bitcode_file_name OutputFileName ModuleTargetName)
  wasm_get_debug_postfix(_debug_postfix)
  set(_outputfilename "WASM${ModuleTargetName}${_debug_postfix}.bc")
  set(${OutputFileName} "${_outputfilename}" PARENT_SCOPE)
endfunction()

function(wasm_get_bitcode_file OutputFile ModuleTargetName)
  wasm_get_output_dir(OutputFileDir)
  wasm_get_bitcode_file_name(BitcodeFileName ${ModuleTargetName})
  set(_bitcodefile "${OutputFileDir}/${BitcodeFileName}")
  set(${OutputFile} "${_bitcodefile}" PARENT_SCOPE)
endfunction()

function(wasm_get_js_file OutputFile ModuleTargetName)
  wasm_get_output_dir(OutputFileDir)
  wasm_get_js_file_name(OutputFileName ${ModuleTargetName})
  set(_outputfile "${OutputFileDir}/${OutputFileName}")
  set(${OutputFile} "${_outputfile}" PARENT_SCOPE)
endfunction()

function(wasm_get_emslib_file OutputFile ModuleTargetName)
  wasm_get_output_dir(OutputFileDir)
  wasm_get_emslib_file_name(OutputFileName ${ModuleTargetName})
  set(_outputfile "${OutputFileDir}/${OutputFileName}")
  set(${OutputFile} "${_outputfile}" PARENT_SCOPE)
endfunction()