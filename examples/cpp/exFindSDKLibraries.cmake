#################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

## Include guard
if(FindSDKLibrariesHelper_Include)
  return()
endif()
set(FindSDKLibrariesHelper_Include TRUE)

set(DEBUG_FIND_SDKLibraries_HELPER OFF)# Set to ON if want to debug

################################################################################################
################################################################################################
## User need to set environment variable SDKLIBRARIES_ROOT pointing to the root of SDKLibraries library.
## Or in cmake command, add option -DSDKLIBRARIES_ROOT=/absolute/path/to/installed/sdklibraries
## All header files should be in       $SDKLIBRARIES_ROOT/include
## All static lib files should be in   $SDKLIBRARIES_ROOT/lib

## Make sure to use the file SDKLibraries.CMakeLists.txt in $ROOT/cmake/findpackage to build and install SDKLibraries

#.rst:
# FindSDKLibraries
# ---------
#
# Try to find SDKLibraries
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# TODO :
# This module defines :prop_tgt:`IMPORTED` target ``SDKLibraries::SDKLibraries``, if
# SDKLibraries has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   SDKLibraries_FOUND - system has SDKLibraries
#   SDKLibraries_INCLUDE_DIR - the SDKLibraries include directory
#   SDKLibraries_LIBRARIES - Link these to use SDKLibraries
#   SDKLibraries_VERSION_STRING - the version of SDKLibraries found (since CMake 2.8.8)

if(DEFINED SDKLIBRARIES_ROOT)
  message("SDKLIBRARIES_ROOT is defined as [${SDKLIBRARIES_ROOT}]")
  set(SDKLIBRARIES_ROOT ${SDKLIBRARIES_ROOT})
elseif(DEFINED ENV{SDKLIBRARIES_ROOT})
  set(SDKLIBRARIES_ROOT $ENV{SDKLIBRARIES_ROOT})
  message("SDKLIBRARIES_ROOT is defined as [${SDKLIBRARIES_ROOT}]")
else()
  message(FATAL_ERROR "SDKLIBRARIES_ROOT is required to be defined as environment variables or cache variable in cmake command")
endif()

## HINTS PATH put all here
set(_SDKLibraries_PATHS PATHS "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GnuWin32\\SDKLibraries;InstallPath];${SDKLIBRARIES_ROOT};${CMAKE_INSTALL_PREFIX}")
if(DEBUG_FIND_SDKLibraries_HELPER)
    message("List of hints path to find SDKLibraries")
    foreach(_HINT_PATH ${_SDKLibraries_PATHS})
        message(" [${_HINT_PATH}]")
    endforeach()
endif()

find_path(_Installed_SDKLibraries_INCLUDE_DIR 
          NAMES SDKVersion.hpp
          PATHS ${_SDKLibraries_PATHS}
          PATH_SUFFIXES include
)

if(_Installed_SDKLibraries_INCLUDE_DIR)
    set(SDKLibraries_INCLUDE_DIR ${_Installed_SDKLibraries_INCLUDE_DIR})
endif()

set(SDKLibraries_VERSION "UNKNOWN")#TODO

set(_sdk_base_modules
  AsymKey
  BigNumbers
  ECPoint
  MessageHash
  Polynomial
  SecretSplit
  SymEncDec
)

if(NOT SDKLibraries_LIBRARIES)
  foreach(_base_module_name ${_sdk_base_modules})
    find_library(${_base_module_name}_RELEASE NAMES ${_base_module_name} PATHS ${_SDKLibraries_PATHS} PATH_SUFFIXES lib)
    if(DEFINED ${_base_module_name}_RELEASE)
      set(SDKLibraries_LIBRARY_RELEASE "${SDKLibraries_LIBRARY_RELEASE};${${_base_module_name}_RELEASE}")
    endif()
    find_library(${_base_module_name}_DEBUG NAMES ${_base_module_name}d PATHS ${_SDKLibraries_PATHS} PATH_SUFFIXES lib)
    if(DEFINED ${_base_module_name}_DEBUG)
      set(SDKLibraries_LIBRARY_DEBUG "${SDKLibraries_LIBRARY_DEBUG};${${_base_module_name}_DEBUG}")
    else()## If not found debug version, set debug to release version
      set(SDKLibraries_LIBRARY_DEBUG "${SDKLibraries_LIBRARY_DEBUG};${${_base_module_name}_RELEASE}")
    endif()
  endforeach()
endif()

include(SelectLibraryConfigurations)
SELECT_LIBRARY_CONFIGURATIONS(SDKLibraries)

# handle the QUIETLY and REQUIRED arguments and set SDKLibraries_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDKLibraries REQUIRED_VARS SDKLibraries_LIBRARIES SDKLibraries_INCLUDE_DIR)

mark_as_advanced(SDKLibraries_INCLUDE_DIR)
################################################################################################
################################################################################################

if(NOT SDKLibraries_FOUND)
    message("  -- Cmake cannot find SDKLibraries in [${SDKLIBRARIES_ROOT}]")
    message("  -- You need to put SDKLibraries library in the folder structure [${SDKLIBRARIES_ROOT}]")
endif()

if(DEBUG_FIND_SDKLibraries_HELPER)
    message("                 SDKLibraries_FOUND [${SDKLibraries_FOUND}]")
    message("           SDKLibraries_INCLUDE_DIR [${SDKLibraries_INCLUDE_DIR}]")
    message("             SDKLibraries_LIBRARIES [${SDKLibraries_LIBRARIES}]")
    message("        SDKLibraries_VERSION_STRING [${SDKLibraries_VERSION_STRING}]")
endif()
message("")
