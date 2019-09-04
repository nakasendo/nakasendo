#################################################################
#  Date             03/09/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################


## To build wasm modules, it is required to install emscripten and build openssl to wasm
##   - To setup emscripten, follow the documentation in https://emscripten.org/docs/getting_started/downloads.html
##     After completing installation, add path to emcc to the PATH environment variable
##   - To install openssl build in wasm, download from https://bitbucket.org/nch-atlassian/sdklibraries/downloads/openssl-1.1.1b_wasm.tar.gz
##     Extract it somewhere, and set the environment variable OPENSSL_WASM_ROOT_DIR to this directory
##
## Example of linking with WASMBigNumbers
##  emcc BigNumbersClient.cpp -lWASMBigNumbers -L /path/to/INSTALLATION/lib -I /path/to/INSTALLATION/include -std=c++11 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s WASM=1 -o index.html
##
#### Main function helping to find emcc compiler
macro(sdkHelpFindEMCC)########################################################################################
  find_program(emcc_EXECUTABLE NAMES emcc PATH_SUFFIXES bin)
  find_program(emar_EXECUTABLE NAMES emar PATH_SUFFIXES bin)

  # handle the QUIETLY and REQUIRED arguments and set emcc_FOUND to TRUE if
  # all listed variables are TRUE
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args (emcc FOUND_VAR emcc_FOUND REQUIRED_VARS emcc_EXECUTABLE emar_EXECUTABLE)

  mark_as_advanced(emcc_FOUND emcc_EXECUTABLE emar_EXECUTABLE)
endmacro()

function(_wasm_check_list_cpp_file ModuleTargetName)
  if(NOT DEFINED ${ModuleTargetName}_CPP_FILES)
    message("SDK ERROR : Unable to track list of ${ModuleTargetName} cpp files when building WASM module for ${ModuleTargetName}")
    message(FATAL_ERROR "SDK ERROR : From CMakelists.txt of ${ModuleTargetName}, need to set the cache variable ${ModuleTargetName}_CPP_FILES to hold the list of all its cpp files")
  endif()
endfunction()

function(_wasm_get_debug_postfix DebugPostFix)
  if(CMAKE_CONFIGURATION_TYPES)
    set(_debug_postfix "\$<IF:\$<CONFIG:Debug>,d,>")
  else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      set(_debug_postfix ${CMAKE_DEBUG_POSTFIX})
    endif()
  endif()
  set(${DebugPostFix} "${_debug_postfix}" PARENT_SCOPE)
endfunction()

function(_wasm_get_output_dir OutputFileDir)
  if(CMAKE_CONFIGURATION_TYPES)
    set(_outputfiledir "\$<IF:\$<CONFIG:Debug>,${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG},${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}>")
  else()
    set(_outputfiledir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  endif()
  set(${OutputFileDir} "${_outputfiledir}" PARENT_SCOPE)
endfunction()

function(_wasm_get_js_file_name OutputFileName ModuleTargetName)
  _wasm_get_debug_postfix(_debug_postfix)
  set(_outputfilename "WASM${ModuleTargetName}${_debug_postfix}.js")
  set(${OutputFileName} "${_outputfilename}" PARENT_SCOPE)
endfunction()

function(_wasm_get_emslib_file_name OutputFileName ModuleTargetName)
  _wasm_get_debug_postfix(_debug_postfix)
  set(_outputfilename "libWASM${ModuleTargetName}${_debug_postfix}.a")
  set(${OutputFileName} "${_outputfilename}" PARENT_SCOPE)
endfunction()

function(_wasm_get_bitcode_file_name OutputFileName ModuleTargetName)
  _wasm_get_debug_postfix(_debug_postfix)
  set(_outputfilename "WASM${ModuleTargetName}${_debug_postfix}.bc")
  set(${OutputFileName} "${_outputfilename}" PARENT_SCOPE)
endfunction()

function(_wasm_get_bitcode_file OutputFile ModuleTargetName)
  _wasm_get_output_dir(OutputFileDir)
  _wasm_get_bitcode_file_name(BitcodeFileName ${ModuleTargetName})
  set(_bitcodefile "${OutputFileDir}/${BitcodeFileName}")
  set(${OutputFile} "${_bitcodefile}" PARENT_SCOPE)
endfunction()

function(_wasm_get_js_file OutputFile ModuleTargetName)
  _wasm_get_output_dir(OutputFileDir)
  _wasm_get_js_file_name(OutputFileName ${ModuleTargetName})
  set(_outputfile "${OutputFileDir}/${OutputFileName}")
  set(${OutputFile} "${_outputfile}" PARENT_SCOPE)
endfunction()

function(_wasm_get_emslib_file OutputFile ModuleTargetName)
  _wasm_get_output_dir(OutputFileDir)
  _wasm_get_emslib_file_name(OutputFileName ${ModuleTargetName})
  set(_outputfile "${OutputFileDir}/${OutputFileName}")
  set(${OutputFile} "${_outputfile}" PARENT_SCOPE)
endfunction()


function(_wasm_cmd_emsdk_env SetEnvCMD)
  if(WIN32)
    set(${SetEnvCMD} "$ENV{EMSDK_ROOT}\\emsdk_env.bat" PARENT_SCOPE)
  else()
    set(${SetEnvCMD} "source $ENV{EMSDK_ROOT}/emsdk_env.sh" PARENT_SCOPE)
  endif()
endfunction()
##############################################################################################################################
##############################################################################################################################

if(NOT DEFINED ENV{OPENSSL_WASM_ROOT_DIR})
  message(FATAL_ERROR "SDK ERROR : openssl-wasm binaries not found. Define environment variable OPENSSL_WASM_ROOT_DIR pointing to the installed wasm-openssl.")
else()
  sdkHelpFindEMCC()
  if(NOT emcc_FOUND)
    message(FATAL_ERROR "SDK ERROR : emsdk 1.38.30 is required to build WASM modules. Try to add the location of emcc to environment variable PATH")
  endif()
endif()
file(TO_CMAKE_PATH "$ENV{OPENSSL_WASM_ROOT_DIR}" OPENSSL_WASM_ROOT_CMAKE_DIR)## TODO set it at cache level

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(MODULE_BASE_NAME CoreModule) ## Bignumber, MessageHash ...

_wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
_wasm_get_output_dir(WASM_OUTPUT_DIR)
_wasm_get_bitcode_file(BITCODE_OUTPUT_FILE ${MODULE_BASE_NAME})
_wasm_get_emslib_file(LIBRARY_OUTPUT_FILE ${MODULE_BASE_NAME})
_wasm_get_js_file(JS_OUTPUT_FILE ${MODULE_BASE_NAME})

message("-----------------------------  WASM_DEBUG_POSTFIX[${WASM_DEBUG_POSTFIX}]")
message("-----------------------------  WASM_OUTPUT_DIR[${WASM_OUTPUT_DIR}]")
message("-----------------------------  BITCODE_OUTPUT_FILE[${BITCODE_OUTPUT_FILE}]")
message("-----------------------------  LIBRARY_OUTPUT_FILE[${LIBRARY_OUTPUT_FILE}]")
message("-----------------------------  JS_OUTPUT_FILE[${JS_OUTPUT_FILE}]")
message("-----------------------------  emcc_EXECUTABLE[${emcc_EXECUTABLE}]")
message("-----------------------------  emar_EXECUTABLE[${emar_EXECUTABLE}]")

##################### BUILD MODULE LIBRARY
set(CORE_MODULE_HPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CoreModuleFile1.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/CoreModuleFile2.hpp")
set(CORE_MODULE_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CoreModuleFile1.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/CoreModuleFile2.cpp")
set(CORE_MODULE_SOURCE_FILES ${CORE_MODULE_HPP_FILES} ${CORE_MODULE_CPP_FILES})

set(WASM_LIBRARY_TARGET_NAME WASM${MODULE_BASE_NAME})
add_custom_target(${WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${CORE_MODULE_SOURCE_FILES})

add_custom_command(TARGET ${WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${CORE_MODULE_CPP_FILES}
                   -std=c++11 -Wall
                   -lssl -lcrypto -L${OPENSSL_WASM_ROOT_CMAKE_DIR}/lib
                   -I${CMAKE_CURRENT_SOURCE_DIR} -I${OPENSSL_WASM_ROOT_CMAKE_DIR}/include -I${CMAKE_SOURCE_DIR}/depends/json -I${CMAKE_SOURCE_DIR}/src/core -I${SDK_GENERATED_HPP_DIR}
                   -o ${BITCODE_OUTPUT_FILE}
)
add_custom_command(TARGET ${WASM_LIBRARY_TARGET_NAME} POST_BUILD
                   COMMAND ${emar_EXECUTABLE}
                   ARGS
                   r ${LIBRARY_OUTPUT_FILE} ${BITCODE_OUTPUT_FILE}
)

##################### BUILD MODULE JAVASCRIPT
set(JS_MODULE_HPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/JsModule.hpp")
set(JS_MODULE_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/JsModule.cpp")
set(JS_MODULE_SOURCE_FILES ${JS_MODULE_CPP_FILES})

set(WASM_JS_TARGET_NAME Js${MODULE_BASE_NAME})
add_custom_target(${WASM_JS_TARGET_NAME} ALL SOURCES ${JS_MODULE_SOURCE_FILES})
add_dependencies(${WASM_JS_TARGET_NAME} ${WASM_LIBRARY_TARGET_NAME})

add_custom_command(TARGET ${WASM_JS_TARGET_NAME} POST_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${JS_MODULE_CPP_FILES}
                   -std=c++11 -Wall --bind -s WASM=0 -s EXPORT_ALL=1 -s SINGLE_FILE=1
                   -l${WASM_LIBRARY_TARGET_NAME}${WASM_DEBUG_POSTFIX} -L${WASM_OUTPUT_DIR}
                   -lssl -lcrypto -L${OPENSSL_WASM_ROOT_CMAKE_DIR}/lib
                   -I${CMAKE_CURRENT_SOURCE_DIR} -I${OPENSSL_WASM_ROOT_CMAKE_DIR}/include -I${CMAKE_SOURCE_DIR}/depends/json -I${CMAKE_SOURCE_DIR}/src/core -I${SDK_GENERATED_HPP_DIR}
                   -o ${JS_OUTPUT_FILE}
)

#install(FILES "${BITCODE_OUTPUT_FILE}" DESTINATION "lib" COMPONENT WASMModules)
#install(FILES "${JS_OUTPUT_FILE}" DESTINATION "lib" COMPONENT WASMModules)
#install(FILES "${BITCODE_OUTPUT_FILE}" DESTINATION "example/javascript" COMPONENT "Examples")
#install(FILES "${JS_OUTPUT_FILE}" DESTINATION "example/javascript" COMPONENT "Examples")
