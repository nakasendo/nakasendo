#################################################################
#  Date             25/04/2019                                  #
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
## Example of linking with WASM-BigNumbers
##  emcc BigNumbersClient.cpp -lWASM-BigNumbers -L /path/to/INSTALLATION/lib -I /path/to/INSTALLATION/include -std=c++11 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s WASM=1 -o index.html
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

function(_wasm_check_list_cpp_file ModuleBaseName)
  if(NOT DEFINED ${ModuleBaseName}_CPP_FILES)
    message("SDK ERROR : Unable to track list of ${ModuleBaseName} cpp files when building WASM module for ${ModuleBaseName}")
    message(FATAL_ERROR "SDK ERROR : From CMakelists.txt of ${ModuleBaseName}, need to set the cache variable ${ModuleBaseName}_CPP_FILES to hold the list of all its cpp files")
  endif()
endfunction()

function(_wasm_get_output_file_dir OutputFileDir ModuleBaseName)
  if(CMAKE_CONFIGURATION_TYPES)
    set(_outputfiledir "\$<TARGET_FILE_DIR:${ModuleBaseName}>")
  else()
    set(_outputfiledir "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  endif()
  set(${OutputFileDir} "${_outputfiledir}" PARENT_SCOPE)
endfunction()

function(_wasm_get_output_file_name OutputFileName ModuleBaseName)
  if(CMAKE_CONFIGURATION_TYPES)
    set(_outputfilename "libWASM-${ModuleBaseName}\$<IF:\$<CONFIG:Debug>,d,>.a")
  else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      set(_wasm_debug_postfix ${CMAKE_DEBUG_POSTFIX})
    endif()
    set(_outputfilename "libWASM-${ModuleBaseName}${_wasm_debug_postfix}.a")
  endif()
  set(${OutputFileName} "${_outputfilename}" PARENT_SCOPE)
endfunction()

function(_wasm_get_bitcode_file_name BitcodeFileName ModuleBaseName)
  if(CMAKE_CONFIGURATION_TYPES)
    set(_bitcodefilename "WASM-${ModuleBaseName}\$<IF:\$<CONFIG:Debug>,d,>.bc")
  else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
      set(_wasm_debug_postfix ${CMAKE_DEBUG_POSTFIX})
    endif()
    set(_bitcodefilename "WASM-${ModuleBaseName}${_wasm_debug_postfix}.bc")
  endif()
  set(${BitcodeFileName} "${_bitcodefilename}" PARENT_SCOPE)
endfunction()

function(_wasm_get_bitcode_file BitCodeFile ModuleBaseName)
  _wasm_get_output_file_dir(OutputFileDir ${ModuleBaseName})
  _wasm_get_bitcode_file_name(BitcodeFileName ${ModuleBaseName})
  set(_bitcodefile "${OutputFileDir}/${BitcodeFileName}")
  set(${BitCodeFile} "${_bitcodefile}" PARENT_SCOPE)
endfunction()

function(_wasm_get_output_file OutputFile ModuleBaseName)
  _wasm_get_output_file_dir(OutputFileDir ${ModuleBaseName})
  _wasm_get_output_file_name(OutputFileName ${ModuleBaseName})
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

#### Build wasm modules corespongding to the cpp tools
####     build_wasm_modules(BigNumbers MessageHash)
macro(build_wasm_modules)##################################################
  if(${ARGC} LESS 2)
    message(FATAL_ERROR "Error calling function build_wasm_modules(BigNumbers MessageHash)")
  endif()
  set(module_list "${ARGN}")

  foreach(_module IN LISTS module_list)
    if(NOT TARGET ${_module})
      message(FATAL_ERROR "SDK ERRO : ${_module} is has not been built as cpp module")
    endif()
    message(STATUS "SDK INFO : build WASM module for ${_module}")

    _wasm_check_list_cpp_file(${_module})
    set(WASM_TARGET_NAME WASM-${_module})
    add_custom_target(${WASM_TARGET_NAME} ALL)
    add_dependencies(${WASM_TARGET_NAME} ${_module})
    set_property(TARGET ${WASM_TARGET_NAME} PROPERTY FOLDER "modules") ## Set Folder for MSVC

    _wasm_get_bitcode_file(WASM_BITCODE_FILE ${_module})
    _wasm_get_output_file(WASM_OUTPUT_FILE ${_module})
    string(REPLACE ";" " " WASM_LIST_CPP_FILES "${${_module}_CPP_FILES}")
    file(TO_CMAKE_PATH "$ENV{OPENSSL_WASM_ROOT_DIR}" OPENSSL_WASM_ROOT_CMAKE_DIR)

    if(WIN32)
      set(builderextension bat)
    else()
      set(builderextension sh)
    endif()
    set(BuilderScriptName WASM-${_module}-Builder.${builderextension})
    if(CMAKE_CONFIGURATION_TYPES)
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wasm_module_builder.bat.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${BuilderScriptName})
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wasm_module_builder.bat.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${BuilderScriptName})
      file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${BuilderScriptName} _BUILDER_DEBUG)
      file(TO_NATIVE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${BuilderScriptName} _BUILDER_RELEASE)
      add_custom_command(TARGET ${WASM_TARGET_NAME} POST_BUILD COMMAND cmd /c "$<IF:$<CONFIG:Debug>,${_BUILDER_DEBUG},${_BUILDER_RELEASE}> ${WASM_BITCODE_FILE} ${WASM_OUTPUT_FILE}")
    else()
      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wasm_module_builder.sh.in ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BuilderScriptName})
      add_custom_command(TARGET ${WASM_TARGET_NAME} POST_BUILD COMMAND /bin/sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BuilderScriptName} ${WASM_BITCODE_FILE} ${WASM_OUTPUT_FILE})
    endif()
    install(FILES "${WASM_OUTPUT_FILE}" DESTINATION "lib" COMPONENT WASM)
  endforeach()
endmacro()
