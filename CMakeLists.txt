#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

## How to build 
##
## Tool and dependencies requirements
##
## Run to build/test/install
##
## On Windows
##   cmake -G"Visual Studio 15 2017" -A x64 ..\sdklibraries && cmake --build . --target ALL_BUILD --config Debug && cmake --build . --target ALL_BUILD --config Release
## Windows alternative with MSBuild parallele:
##   cmake -G"Visual Studio 15 2017" -A x64 ..\sdklibraries && msbuild SDKLibraries.sln -maxcpucount:4 /p:Configuration=Debug && msbuild SDKLibraries.sln -maxcpucount:4 /p:Configuration=Release
##   cmake -G"Visual Studio 15 2017" -A x64 ..\sdklibraries && msbuild SDKLibraries.sln -v:q -maxcpucount:4 /p:Configuration=Debug && msbuild SDKLibraries.sln  -v:q -maxcpucount:4 /p:Configuration=Release
##
## On Linux (Ubuntu)
##   cmake ..\sdklibraries ; time -p make -j8 ; make install
##
## Packaging :
##   Windows : cpack -G NSIS -C Release ## Require to install NSIS
##   Linus   : cpack -G TGZ ## use at cmake time to get the precise os type installer name : -DCUSTOM_SYSTEM_OS_NAME=Ubuntu


cmake_minimum_required(VERSION 3.12)
project(SDKLibraries)

set(SDK_VERSION_MAJOR "1" CACHE INTERNAL "framework major version")
set(SDK_VERSION_MINOR "0" CACHE INTERNAL "framework minor version")
set(SDK_VERSION_PATCH "0" CACHE INTERNAL "framework patch version")

set(CMAKE_INCLUDE_CURRENT_DIR ON)## Always include the current directory

if(MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DNOMINMAX)
endif()

## Init CMake, find all external packages and eventually build them if necessary
include(cmake/SDKInit.cmake)
sdkInitCMake()

##TODO
# Separate source into core, tests
# From tests, separate functional test, unit tests, then separate cpp tests, python tests, js tests

add_subdirectory(src/BigNumbers)
add_subdirectory(src/MessageHash)
# TODO https://stackoverflow.com/questions/45260216/emscripten-cmake-pass-emscripten-options-in-cmakelist-file
#add_subdirectory(src/WasmTest)