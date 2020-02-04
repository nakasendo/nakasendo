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

## Build protobuf from source and install it locally to your machine.
## Go to a build directory, clone the source code from github
##    git clone https://github.com/protocolbuffers/protobuf.git protobuf_src
## On Windows with Visual Studio
##    cmake -G"Visual Studio 15 2017" -A x64 C:\path\to\source\protobuf_src\cmake -DCMAKE_INSTALL_PREFIX=C:\path\to\install\directory\protobuf -Dprotobuf_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF -Dprotobuf_MSVC_STATIC_RUNTIME=OFF && cmake --build . --target ALL_BUILD --config Debug && cmake --build . --target ALL_BUILD --config Release && cmake --build . --target INSTALL --config Debug && cmake --build . --target INSTALL --config Release
## On Linux
##    cmake /path/to/source/protobuf_src/cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/directory/protobuf -Dprotobuf_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON; make -j4 ; make install
## Name the environment variable Protobuf_ROOT pointing to the directory where protobuf is installed
##
## To use wiht cmake, use the predefined function from module FindProtobuf.cmake::protobuf_generate
##    Preset the protobuf_generate_PROTOC_OUT_DIR to the desired directory
##    Preset the protobuf_generate_LANGUAGE to the desired language (cpp, js, python...)
##    Preset the protobuf_generate_TARGET to the single protobuf file .proto
##    or Preset the protobuf_generate_PROTOS to the list of protobuf files *.proto


#### Help to find Protobuf.
macro(sdkHelpFindProtobuf)########################################################################################
  # find protobuf
  find_package(Protobuf)

  if(Protobuf_FOUND)
    set(SDK_GENERATED_PROTOBUF_DIR "${CMAKE_BINARY_DIR}/generated/protobuf" CACHE STRING "Directory containing all generated protobuf files")
    file(MAKE_DIRECTORY "${SDK_GENERATED_PROTOBUF_DIR}")
    set(protobuf_generate_PROTOC_OUT_DIR "${SDK_GENERATED_PROTOBUF_DIR}" CACHE STRING "Directory containing all generated protobuf files")
    message(STATUS "Found protobuf version ${Protobuf_VERSION}:")
    message(STATUS "  All file *.proto will generate the source files in the directory [${protobuf_generate_PROTOC_OUT_DIR}]")
    message(STATUS "  Usage CMakeLists.txt, call:")
    message(STATUS "  protobuf_generate_cpp(MyFile_SRCS MyFile_HDRS /path/to/MyFile.proto)")
    message(STATUS "  add_executable(my_executable_prog my_executable_prog.cpp \${MyFile_SRCS} \${MyFile_HDRS})")
    message(STATUS "  target_include_directories(my_executable_prog PUBLIC \${protobuf_generate_PROTOC_OUT_DIR})")
    message(STATUS "  target_link_libraries(my_executable_prog protobuf::libprotobuf)")
  endif()
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
