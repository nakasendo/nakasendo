#################################################################
#  Date             12/02/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
## Include guard
if(FindProtobufHelper_Include)
  return()
endif()
set(FindProtobufHelper_Include TRUE)

## Build protobuf from source and install it locally to your machine
## On Windows with Visual Studio
##    cmake -G"Visual Studio 15 2017" -A x64 C:\path\to\source\protobuf\cmake -DCMAKE_INSTALL_PREFIX=C:\path\to\install\directory\protobuf -Dprotobuf_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF && cmake --build . --target ALL_BUILD --config Debug && cmake --build . --target ALL_BUILD --config Release && cmake --build . --target INSTALL --config Debug && cmake --build . --target INSTALL --config Release
## On Linux
##    cmake /path/to/source/protobuf/cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/directory/protobuf -Dprotobuf_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF ; make -j4 ; make install
## Name the environment variable Protobuf_ROOT pointing to the directory where protobuf is installed
##


#### Help to find Protobuf.
macro(sdkHelpFindProtobuf)########################################################################################
  # find protobuf
  find_package(Protobuf)
endmacro()

function(sdkPrintProtobufInfo)
  sdkPrintProperties(protobuf::libprotobuf)
  sdkPrintProperties(protobuf::libprotobuf-lite)
  sdkPrintProperties(protobuf::libprotoc)
  sdkPrintProperties(protobuf::protoc)
  message(" --")
  message(" ---------- Protobuf_FOUND              [${Protobuf_FOUND}]")
  message(" ---------- Protobuf_VERSION            [${Protobuf_VERSION}]")
  message(" ---------- Protobuf_INCLUDE_DIRS       [${Protobuf_INCLUDE_DIRS}]")
  message(" ---------- Protobuf_LIBRARIES          [${Protobuf_LIBRARIES}]")
  message(" ---------- Protobuf_PROTOC_LIBRARIES   [${Protobuf_PROTOC_LIBRARIES}]")
  message(" ---------- Protobuf_LITE_LIBRARIES     [${Protobuf_LITE_LIBRARIES}]")
  message(" ---------- Protobuf_PROTOC_EXECUTABLE  [${Protobuf_PROTOC_EXECUTABLE}]")
endfunction()
