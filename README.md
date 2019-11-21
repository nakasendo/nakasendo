---
title: Welcome
wikifile: README.md
---

## Welcome to the SDKLibraries

The SDKLibraries brings cryptographic functionality to developers via several different APIs - C++, Python and Webassembley (and JavaScript soon). 

The build system for the SDK is CMake.

### Dependencies

To be able to build the SDK, users need to install some tools and libraries. In general, to install the libraries and tools listed below, it is recommended to manually build on Linux, and download the prebuilt installers for windows platforms.

- CMake 3.14.7 (https://cmake.org/download/)
- C++ Compiler: Visual Studio Community Edition 2017 on windows, g++7 on Linux
- Boost 1.69.0 to build CPP unit tests (https://www.boost.org/ and for the binary: https://sourceforge.net/projects/boost/files/boost-binaries/1.69.0/)
- OpenSSL 1.1.b. It needs two versions: one for native C++, the other for emcripten. The prebuilt openssl for Emscripten can be downloaded from https://bitbucket.org/nch-atlassian/sdklibraries/downloads/
- EMSDK: 1.38.0 : https://emscripten.org/docs/getting_started/downloads.html.
    Use install and activate to install components
	1. Clone the EMSDK Git repository
	2. Pull latest changes
	3. From the newly created EMSDK repository's root directory, perform "emsdk list" to see the installable versions
	4. Choose the latest minor version of 1.38 e.g. "emsdk install 1.38.48"
	5. On Windows, run "emsdk" instead of "./emsdk", and "emsdk_env.bat" instead of "source ./emsdk_env.sh".
	6. Install the following tools, using "emsdk list"
    - **clang**  e.g. emsdk install clang-e1.38.30-64bit
    - **emscripten**  e.g. emsdk install emscripten-1.38.30
    - **node** (need to install modules **mocha**, **mocha-junit-reporter** using NPM command: "npm install --global mocha mocha-junit-reporter")
    - **binaryen** (not required  but useful tools)  e.g. emsdk install binaryen-tag-1.38.30-64bit
- Python 3.7.3 (64-bit) (Release and Debug versions) (https://www.python.org/downloads/release/python-374/)
    As part of the Python Windows installer, ensure to check the installation options for the "Download debugging symbols" and the "Download debug binaries (requires VS 2015 or later)" checkboxes.
    - **pathlib**       useful i.e. python -m pip install pathlib
    - **pytest**        to run Python tests i.e. python -m pip install pytest
    - **junitparser**   to combine overall junit test results i.e. python -m pip install junitparser
    - **ecdsa**         to study elliptic curves i.e. python -m pip install ecdsa
    - **requests**      to send http(s) request to different server APIs (Jenkins/ Bitbucket) i.e. python -m pip install requests
- NSIS (Nullsoft Scriptable Install System) 3.04 : https://sourceforge.net/projects/nsis/

Once the build tools and libraries are prepared, some post installation steps are required to let the SDKLibraries build system know how to find everything:

- Add C:\Development\repos\emsdk\clang\e1.38.30_64bit to the path too
- Add emscripten path to path as well
- Add the directory of CMake executable to the environment variable PATH
- Add the directory of Python3 executable to the environment variable PATH
- Add the directory of Emscripten executable to the environment variable PATH
- Set environment variable BOOST_ROOT pointing to the root of the Boost installation
- Set environment variable OPENSSL_ROOT_DIR pointing to the root of the OpenSSL for C++ installation
- Set environment variable OPENSSL_WASM_ROOT_DIR pointing to the root of the OpenSSL for Emscripten installation

The SDKLibraries can be built on any system, as long as all libraries and required tools are manually compiled to ensure absolute compatibility. Users can choose the appropriate CMake generator to make the build (https://cmake.org/cmake/help/v3.14/manual/cmake-generators.7.html)
The CMake build allows everything to be built separately from the source to keep the source repository clean. After cloning the source code to "SDKLibraries" directory, create a "build" directory alongside this, then run the cmake/make command to build

###### Compile/test/pack on Windows
```console
C:\nchain\sdk> cmake -G"Visual Studio 15 2017" -A x64 ..\SDKLibraries && cmake --build . --target ALL_BUILD --config Debug && cmake --build . --target ALL_BUILD --config Release && ctest -C Debug && ctest -C Release && cpack -G NSIS -C Release
```

###### Compile/test/pack on Linux
```console
nchain@sdk:~$ cmake ../SDKLibraries -DCMAKE_BUILD_TYPE=Debug -DCUSTOM_SYSTEM_OS_NAME=Ubuntu; time -p make -j8 ; ctest ; cpack -G TGZ
```

###### To run all tests:
```console
nchain@sdk:~$ ctest             # or "make test" on linux

C:\nchain\sdk> ctest -C Release # On Windows
C:\nchain\sdk> ctest -C Debug   # On Windows
```