#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
if(SDKInit_Include) ## Include guard
  return()
endif()
set(SDKInit_Include TRUE)

#####################################################
#### Initialize everything related to cmake here ####
#

#### Hold the directory containing this current script ####
set(SDK_ROOT_CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}" CACHE PATH "Root directory of cmake modules")

#### Set all directories/subdirectories in this root to CMAKE_MODULE_PATH
macro(sdkInitModulePaths)
  list(APPEND CMAKE_MODULE_PATH "${SDK_ROOT_CMAKE_MODULE_PATH}")
  sdkListSubDir(dirlist ${SDK_ROOT_CMAKE_MODULE_PATH})
  list(APPEND CMAKE_MODULE_PATH ${dirlist})
endmacro()

#### Force the CMAKE_BUILD_TYPE on UNIX like (not visual studio) system AND user doesn't specify the build type
#### TODO add option build production vs dev. If prod --> default to release build, if dev --> default to debug build
function(sdkSetCMakeBuildType)
  if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "CMAKE_BUILD_TYPE default to Release on Unix system" FORCE)
    message(WARNING "CMAKE_BUILD_TYPE was not defined, forced to [${CMAKE_BUILD_TYPE}]")
  endif()
endfunction()

#### Set some technical value of the OS
macro(sdkGetOSInfo)
  ## Set x64/x86
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SDK_SYSTEM_X64 ON)
    set(SDK_SYSTEM_BUILD_ARCHI 64 CACHE STRING "System archi")
  else()
    set(SDK_SYSTEM_X86 ON)
    set(SDK_SYSTEM_BUILD_ARCHI 32 CACHE STRING "System archi")
  endif()

  ## SDK_SYSTEM_OS used for package name.
  ## Users can define -DCUSTOM_SYSTEM_OS_NAME=Ubuntu for example to have the customized os name on the installer
  if(WIN32) #Windows
    set(SDK_SYSTEM_OS Windows CACHE STRING "System OS type")
  else()    # UNIX
    if(APPLE)
        set(SDK_SYSTEM_OS MacOS CACHE STRING "System OS type")
    else()
        set(SDK_SYSTEM_OS Linux CACHE STRING "System OS type")
    endif()
  endif()
endmacro()

#### Create directory in binary tree to put all generated files
macro(sdkCreateGeneratedDir)
  set(SDK_GENERATED_DIR "${CMAKE_BINARY_DIR}/generated" CACHE STRING "Directory containing all generated files")
  file(MAKE_DIRECTORY "${SDK_GENERATED_DIR}")
endmacro()

#### Create directory in binary tree to put all generated hpp files
macro(sdkCreateGeneratedHppDir)
  set(SDK_GENERATED_HPP_DIR "${CMAKE_BINARY_DIR}/generated/hpp" CACHE STRING "Directory containing all generated hpp files")
  file(MAKE_DIRECTORY "${SDK_GENERATED_HPP_DIR}")
  include_directories("${SDK_GENERATED_HPP_DIR}")
endmacro()

#### Create directory in binary tree to put all generated tools files
macro(sdkCreateGeneratedToolsDir)
  set(SDK_GENERATED_TOOLS_DIR "${CMAKE_BINARY_DIR}/generated/tools" CACHE STRING "Directory containing all generated tools")
  file(MAKE_DIRECTORY "${SDK_GENERATED_TOOLS_DIR}")
endmacro()

#### Check consistency of CMAKE_BUILD_TYPE vs CMAKE_CONFIGURATION_TYPES
####   On build system like vs or xcode, CMAKE_CONFIGURATION_TYPES is defined for both Debug and Release
####   On build system like Unix, CMAKE_BUILD_TYPE is defined indicating the build type
####   Not both of them are defined
macro(sdkTestBuildType)
  ## Set x64/x86
  if(CMAKE_BUILD_TYPE AND CMAKE_CONFIGURATION_TYPES)
    message(FATAL_ERROR "Both CMAKE_BUILD_TYPE and CMAKE_CONFIGURATION_TYPES are defined, which is not expected")
  endif()
endmacro()

#### Force CMAKE_INSTALL_PREFIX if not defined
macro(sdkForceInstallDir)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/INSTALLATION" CACHE PATH "Cmake prefix" FORCE)
  message(STATUS "SDK WARNING: Forced CMAKE_INSTALL_PREFIX[${CMAKE_INSTALL_PREFIX}]")
endmacro()

#### Create API definition file
function(sdkDefineDynamicAPI)
  #Copy the API definition file
  set(DYNAMIC_LIBRARY_API_HPP_IN ${SDK_ROOT_CMAKE_MODULE_PATH}/DYNAMIC_LIBRARY_API.hpp.in)
  set(DYNAMIC_LIBRARY_API_HPP ${SDK_GENERATED_HPP_DIR}/DYNAMIC_LIBRARY_API.hpp)
  configure_file(${DYNAMIC_LIBRARY_API_HPP_IN} ${DYNAMIC_LIBRARY_API_HPP})

  include_directories("${SDK_GENERATED_HPP_DIR}")
  if(BUILD_ALL_STATIC_LIBS)
    set(LIBS_BUILD_TYPE STATIC CACHE INTERNAL "Build type for all libraries")
    add_definitions(-DBUILD_ALL_STATIC_LIBS)
  else()
    set(LIBS_BUILD_TYPE SHARED CACHE INTERNAL "Build type for all libraries")
  endif()

  message(STATUS "API definitions file created [${DYNAMIC_LIBRARY_API_HPP}]")
  message(STATUS "  Use \${LIBS_BUILD_TYPE} instead of SHARED or STATIC in add_library for general propose")
  message(STATUS "  Run cmake with option -DBUILD_ALL_STATIC_LIBS=ON to build all libraries in STATIC. Otherwise all libraries will be built in SHARED")
  install(FILES "${DYNAMIC_LIBRARY_API_HPP}" DESTINATION "include" COMPONENT Files)
endfunction()

#### Initialize all setting for using CMake
macro(sdkInitCMake)

  sdkTestBuildType()
  sdkSetCMakeBuildType()
  sdkForceInstallDir()
  cmake_policy(SET CMP0057 NEW)# USE IN_LIST functionality, example: if(${var} IN_LIST MYVAR_LIST)
  cmake_policy(SET CMP0074 NEW)# Avoid warning when find_package
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)#
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMakeTargets")

  include(${SDK_ROOT_CMAKE_MODULE_PATH}/SDKTools.cmake)
  sdkInitModulePaths()
  #sdkPrintList("CMAKE_MODULE_PATH" "${CMAKE_MODULE_PATH}") ## Debug Log
  sdkGetOSInfo()
  sdkCreateGeneratedDir()
  sdkCreateGeneratedHppDir()
  sdkCreateGeneratedToolsDir()
  #sdkPrintOSInfo()#Debug Log

  include(SDKBuildSetting)
  sdkSetCompilationOptions()
  sdkSetOutputDirectories()
  sdkSetBuildVersion()
  sdkDefineDynamicAPI()
  install(FILES "${SDK_VERSION_HPP}" DESTINATION "include" COMPONENT Files)

  ## Precalculate variable for installation
  sdkGetInstallRootDir(_install_root_dir)
  set(SDK_COMMON_INSTALL_PREFIX "${_install_root_dir}" CACHE PATH "Common directory used for installation")

  include(FindOpenSSLHelper)
  HelpFindOpenSSL()
  #sdkPrintOpenSSLInfo()#Debug Log

  include(FindPythonHelper)
  HelpFindPython()
  #sdkPrintPythonInfo()#Debug Log
  if(BUILD_WEBASSEMBLY)
  	include(FindEmsdkHelper)
  	sdkHelpFindEMSDK()
  	sdkPrintEmsdkInfo()#Debug Log
  endif(BUILD_WEBASSEMBLY)
  if(BUILD_TESTS OR INCLUDE_SECP256K1_LIB)
  	include(FindBoostHelper)
  	HelpFindBoost()
  	sdkPrintProperties(Boost::random)
  endif(BUILD_TESTS OR INCLUDE_SECP256K1_LIB)

  include(FindProtobufHelper)
  sdkHelpFindProtobuf()
  #sdkPrintProtobufInfo()

  enable_testing()
endmacro()
