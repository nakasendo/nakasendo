In order to develop/contribute to Nakasendo, it is required to install and prepare some dev environments. It consist of installing tools/libraries for C++/Python/Javascript. It is recommended to

- Build from source when it is Linux environment
- Download prebuilt binaries when it is Windows environment
- Install everything in the same location and define environment variables poiting to the installed location. The build system will find them appropriately

## C++ libraries and tools

- [CMake 3.14.7](https://cmake.org/download/)
- C++ Compiler: Visual Studio Community Edition 2017 on windows, g++7 on Linux
- [Boost 1.69.0](https://www.boost.org/) to build CPP unit tests ([download prebuild binaries for windows](https://sourceforge.net/projects/boost/files/boost-binaries/1.69.0/))
- OpenSSL 1.1.b : It needs two versions: one for native C++, the other for emcripten. The prebuilt openssl for Emscripten can be downloaded from https://bitbucket.org/nch-atlassian/sdklibraries/downloads/
- Protobuf 3.11.0 : either on Windows or Linux, compile it from source and install it. Then name the system variable Protobuf_ROOT pointing to its installation location. See detail in $SDK_SOURCE/cmake/modules/FindProtobufHelper.cmake

## Tools for javascript

- [EMSDK: 1.38.0](https://emscripten.org/docs/getting_started/downloads.html)
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
- napi : TODO

## [Python 3.7.3 (64-bit)](https://www.python.org/downloads/release/python-374/)

As part of the Python Windows installer, ensure to check the installation options for the "Download debugging symbols" and the "Download debug binaries (requires VS 2015 or later)" checkboxes.

  - **pathlib**       useful
  - **pytest**        to run Python tests
  - **junitparser**   to combine overall junit test results
  - **ecdsa**         to study elliptic curves
  - **requests**      to send http(s) request to different server APIs (Jenkins/ Bitbucket)
  - **mkdocs**, **pymdown-extensions** **plantuml_markdown**  to build documentations with mkdocs

You can do it all at once after installing python
```console
nchain@sdk:~$ python -m pip install pathlib pytest junitparser ecdsa requests mkdocs pymdown-extensions plantuml_markdown
```


## [NSIS 3.04](https://sourceforge.net/projects/nsis/)
This is required for windows only, to build windows installer.

## Environment setup

Once the build tools and libraries are prepared, some post installation steps are required to let the SDKLibraries build system know how to find everything:

- Add C:\Development\repos\emsdk\clang\e1.38.30_64bit to the path too
- Add emscripten path to path as well
- Add the directory of CMake executable to the environment variable PATH
- Add the directory of Python3 executable to the environment variable PATH
- Add the directory of Emscripten executable to the environment variable PATH
- Set environment variable BOOST_ROOT pointing to the root of the Boost installation
- Set environment variable OPENSSL_ROOT_DIR pointing to the root of the OpenSSL for C++ installation
- Set environment variable OPENSSL_WASM_ROOT_DIR pointing to the root of the OpenSSL for Emscripten installation

The SDKLibraries can be built on any system, as long as all libraries and required tools are manually compiled to ensure absolute compatibility. Users can choose the appropriate [CMake generator](https://cmake.org/cmake/help/v3.14/manual/cmake-generators.7.html) to make the build.
The CMake build allows everything to be built separately from the source to keep the source repository clean. After cloning the source code to "SDKLibraries" directory, create a "build" directory alongside this, then run the cmake/make command to build

## Compile/test/pack

**On Windows**
```console
C:\development\build> cmake -G"Visual Studio 15 2017" -A x64 ..\SDKLibraries && cmake --build . --target ALL_BUILD --config Debug && cmake --build . --target ALL_BUILD --config Release && ctest -C Debug && ctest -C Release && cpack -G NSIS -C Release
```

**On Linux**
```console
nchain@sdk:~$ cmake ../SDKLibraries -DCMAKE_BUILD_TYPE=Debug -DCUSTOM_SYSTEM_OS_NAME=Ubuntu; time -p make -j8 ; ctest ; cpack -G TGZ
```

**To run all tests**
```console
nchain@sdk:~$ ctest             # or "make test" on linux

C:\nchain\sdk> ctest -C Release # On Windows
C:\nchain\sdk> ctest -C Debug   # On Windows
```