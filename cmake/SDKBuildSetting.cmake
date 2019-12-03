#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
if(SDKBuildSetting_Include)## Include guard
  return()
endif()
set(SDKBuildSetting_Include TRUE)

macro(sdkSetCompilationOptions)## This has to be macro instead of a function because of add_definitions scope is local
  if(NOT SDK_SET_COMPILATION_OPTIONS_DONE)
    ## Set common compile options
    set(CMAKE_CXX_STANDARD 17 CACHE INTERNAL "Use of C++17" FORCE)#TODO : change to C++17 when visual studio support
    set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "Require C++ standard" FORCE)
    set(CMAKE_CXX_EXTENSIONS OFF CACHE INTERNAL "Do not use C++ extension" FORCE)
    set(CMAKE_DEBUG_POSTFIX d CACHE INTERNAL "Use d as debug postfix for  targets" FORCE)

    if (MSVC)
      sdkAddCompilerFlags(/Gm- /MP /wd4251 /wd4244 /wd4307)## TODO test if DNOMINMAX can move here
      ## Using MSVC STATIC RUNTIME)
      ##foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      ##  if(${flag_var} MATCHES "/MD")
      ##    string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      ##  endif(${flag_var} MATCHES "/MD")
      ##endforeach()
    endif()

    if(UNIX)
      set(CMAKE_CXX_FLAGS "-fPIC -fvisibility=default" CACHE STRING "compilation flags for C++" FORCE)
      set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 " CACHE STRING "flags for C++ debug version" FORCE)
      set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -DNDEBUG" CACHE STRING "flags for C++ release version" FORCE)
      set(CMAKE_C_FLAGS "" CACHE STRING "flags for C" FORCE)
      set(CMAKE_C_FLAGS_DEBUG "-O0 -g3" CACHE STRING "flags for C debug version" FORCE)
      set(CMAKE_C_FLAGS_RELEASE "-O3 -g0 -DNDEBUG" CACHE STRING "flags for C release version" FORCE)
    endif()

    set(SDK_SET_COMPILATION_OPTIONS_DONE TRUE CACHE BOOL "Protect to call twices")
  endif()
endmacro()

#### Define output directories according to ${CMAKE_CONFIGURATION_TYPES} or ${CMAKE_BUILD_TYPE}
#### Based on the defined 
####   ${SDK_SYSTEM_BUILD_ARCHI}
####   ${CMAKE_CONFIGURATION_TYPES}
####   ${CMAKE_BUILD_TYPE}
function(sdkSetOutputDirectories)

  if(SDK_SET_OUTPUT_DIRS_DONE)
    return()
  else()
    set(SDK_SET_OUTPUT_DIRS_DONE TRUE CACHE BOOL "Protect to call twices")
  endif()

  if(NOT DEFINED SDK_SYSTEM_BUILD_ARCHI)
    message("  SetBuildFolders : variable [SDK_SYSTEM_BUILD_ARCHI] is not defined")
    message(FATAL_ERROR "On Windows, [SDK_SYSTEM_BUILD_ARCHI] should be defined x64 or x86")
  endif()

  if(CMAKE_BUILD_TYPE)
    set(_BuildTypeList "" "${CMAKE_BUILD_TYPE}")
  else()
    set(_BuildTypeList "${CMAKE_CONFIGURATION_TYPES}")
  endif()

  set(_LIST_OUTPUT_TYPES RUNTIME LIBRARY ARCHIVE)

  foreach(_BuildType IN LISTS _BuildTypeList)
    if(_BuildType)
      string(TOUPPER ${_BuildType} _BUILD_TYPE)
      string(TOLOWER ${_BuildType} _build_type_dir)
      set(_BUILD_TYPE_TAG "_${_BUILD_TYPE}")
    else()
      set(_BUILD_TYPE_TAG "")
      string(TOLOWER ${CMAKE_BUILD_TYPE} _build_type_dir)
    endif()

    set(_MAIN_OUTPUT_PATH "${CMAKE_BINARY_DIR}/x${SDK_SYSTEM_BUILD_ARCHI}/${_build_type_dir}")
    if(NOT EXISTS "${_MAIN_OUTPUT_PATH}")
      file(MAKE_DIRECTORY "${_MAIN_OUTPUT_PATH}")
    endif()
    foreach(_OUT_TYPE ${_LIST_OUTPUT_TYPES})
      set(CMAKE_${_OUT_TYPE}_OUTPUT_DIRECTORY${_BUILD_TYPE_TAG} "${_MAIN_OUTPUT_PATH}" CACHE PATH "Default path for runtime ouput directory" FORCE)
      sdkAppendToGlobalSet(sdkOUTPUT_DIRECTORIES "CMAKE_${_OUT_TYPE}_OUTPUT_DIRECTORY${_BUILD_TYPE_TAG}=[${CMAKE_${_OUT_TYPE}_OUTPUT_DIRECTORY${_BUILD_TYPE_TAG}}]")
    endforeach()
  endforeach()

  #sdkPrintList("sdkOUTPUT_DIRECTORIES" "${sdkOUTPUT_DIRECTORIES}")## Debug Log
endfunction()

function(sdkSetBuildVersion)
  if(SDK_SET_BUILD_VERSION_DONE)
    return()
  else()
    set(SDK_SET_BUILD_VERSION_DONE TRUE CACHE BOOL "Protect to call twices")
  endif()

  if(NOT DEFINED SDK_VERSION_MAJOR)
    set(SDK_VERSION_MAJOR "1" CACHE INTERNAL "framework major version")
  endif()
  if(NOT DEFINED SDK_VERSION_MINOR)
    set(SDK_VERSION_MINOR "0" CACHE INTERNAL "framework minor version")
  endif()
  if(NOT DEFINED SDK_VERSION_PATCH)
    set(SDK_VERSION_PATCH "0" CACHE INTERNAL "framework patch version")
  endif()
  set(SDK_VERSION_STRING "${SDK_VERSION_MAJOR}.${SDK_VERSION_MINOR}.${SDK_VERSION_PATCH}" CACHE INTERNAL "sdk version string")

  # Get branch
  sdkGetGitBranch(_SOURCE_GIT_COMMIT_BRANCH)
  set(SOURCE_GIT_COMMIT_BRANCH ${_SOURCE_GIT_COMMIT_BRANCH} CACHE INTERNAL "Source commit hash")

  sdkGetGitCommitHash(_SOURCE_GIT_COMMIT_HASH)
  set(SOURCE_GIT_COMMIT_HASH ${_SOURCE_GIT_COMMIT_HASH} CACHE INTERNAL "Source commit hash")

  # Get the latest commit datetime
  sdkGetGitCommitDateTime(_SOURCE_GIT_COMMIT_DATETIME)
  set(SOURCE_GIT_COMMIT_DATETIME ${_SOURCE_GIT_COMMIT_DATETIME} CACHE INTERNAL "Source commit datetime")

  # Get the build date time
  sdkGetBuildDateTime(_SDK_BUILD_DATETIME_UTC)
  set(SDK_BUILD_DATETIME_UTC ${_SDK_BUILD_DATETIME_UTC} CACHE INTERNAL "Build datetime UTC")

  #### Generate version C++  #####
  set(SDK_VERSION_HPP_IN ${SDK_ROOT_CMAKE_MODULE_PATH}/SDKVersion.hpp.in CACHE INTERNAL "Template File for framework version config")
  set(SDK_VERSION_HPP ${SDK_GENERATED_HPP_DIR}/SDKVersion.hpp CACHE INTERNAL "HPP File for framework version config")
  configure_file(${SDK_VERSION_HPP_IN} ${SDK_VERSION_HPP})
endfunction()
