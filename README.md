---
title: Welcome
wikifile: Home.md
---

## Welcome to the SDKLibraries

The SDKLibraries allow working on cryptographic functionalities in different API, it can be build through C++/Python/Webassembley. The build system for the SDK is CMake

### Dependencies

To be able to build the SDK, users need to install some tools and libraries. In general, to install the libraries and tools listed below, it is recommended to manually build on Linux, and download prebuilt installer for windows.

* CMake 3.12 (https://cmake.org/download/) 
* C++ Compiler : Visual Studio 2017 on windows, g++7 on Linux
* Boost 1.69.0 to build cpp unit tests
* OpenSSL 1.1.b. It need to have two versions : one for native C++, other for emcripten
* EMSDK : 1.38.0 : https://emscripten.org/docs/getting_started/downloads.html. Use install and activate to install components
    clang
    emscripten
    node (need to install modules mocha, mocha-junit-reporter)
    binaryen (not required  but usefull tools)
* Python3
    pathlib       usefull
    pytest        to run python tests
    junitparser   to combine overall junit test results
    ecdsa         to study elliptic curves
    requests      to send http(s) request to different server api (jenkins/bitbucket)

Once build/install tools and libraries, it requires to do some post installation in order to let SDKLibraries build system to know how to find them :

* Add the directory of cmake executable to the environment variable PATH
* Add the directory of Python3 executable to the environment variable PATH
* Add the directory of emscripten executable to the environment variable PATH
* Set environment variable BOOST_ROOT pointing to the root of the installed Boost
* Set environment variable OPENSSL_ROOT_DIR pointing to the root of the installed OpenSSL for C++
* Set environment variable OPENSSL_WASM_ROOT_DIR pointing to the root of the installed OpenSSL for emscripten


The SDKLibraries can be built on any system, as long as all libraries and used tools are manually compiled to ensure the absolute compatibility. User than can choose the appropriated cmake generator to make the build (https://cmake.org/cmake/help/v3.14/manual/cmake-generators.7.html)
The CMake build allow to build everything separated from the source to keep the source repository clean. Once clone the source code to "SDKLibraries" directory, create a "build" directory along side, then run the cmake/make command to build

### Compile SDK on Windows
cmake -G"Visual Studio 15 2017" -A x64 ..\SDKLibraries && cmake --build . --target ALL_BUILD --config Debug && cmake --build . --target ALL_BUILD --config Release

### Compile SDK on Linux
cmake ..\SDKLibraries ; time -p make -j8

## To run all test set:
ctest            # or make test on linux
ctest -C Release # On Windows
ctest -C Debug   # On Windows