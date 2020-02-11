In order to develop/contribute to Nakasendo, it is required to install and prepare some dev environments. It consist of installing tools/libraries for C++/Python/Javascript. It is recommended to

- Build from source when it is Linux environment
- Download prebuilt binaries when it is Windows environment
- Install everything in the same location and define environment variables poiting to the installed location. The build system will find them appropriately

## Prerequisite Tools
- [Python 3.7.3 (64-bit)](https://www.python.org/downloads/release/python-374/) - 
As part of the Python Windows installer, ensure to check the installation options for the "Download debugging symbols" and the "Download debug binaries (requires VS 2015 or later)" checkboxes.
- [CMake 3.14.7](https://cmake.org/download/)
- C++ Compiler: Visual Studio Community Edition 2017 on windows, g++7 on Linux
- OpenSSL 1.1.b : It needs two versions: one for native C++, the other for emcripten. The prebuilt openssl for Emscripten can be downloaded from https://bitbucket.org/nch-atlassian/sdklibraries/downloads/
- Protobuf 3.11.0 : either on Windows or Linux, compile it from source and install it. Then name the system variable Protobuf_ROOT pointing to its installation location. See detail in $SDK_SOURCE/cmake/modules/FindProtobufHelper.cmake

***Optional***

- If you want to compile our tests you need [Boost 1.69.0](https://www.boost.org/) to build CPP unit tests ([download prebuild binaries for windows](https://sourceforge.net/projects/boost/files/boost-binaries/1.69.0/))
- If you want to perform a Web Assembly compilation please install [EMSDK: 1.38.0](https://emscripten.org/docs/getting_started/downloads.html)
    1. Clone the EMSDK Git repository
    2. Pull latest changes
    3. From the newly created EMSDK repository's root directory, perform "emsdk list" to see the installable versions
    4. Choose the latest minor version of 1.38 e.g. "emsdk install 1.38.48"
    5. On Windows, run "emsdk" instead of "./emsdk", and "emsdk_env.bat" instead of "source ./emsdk_env.sh".
    6. Install the following tools, using "emsdk list"
    7. **clang**  e.g. emsdk install clang-e1.38.30-64bit
    8. **emscripten**  e.g. emsdk install emscripten-1.38.30
    9. **node** (need to install modules **mocha**, **mocha-junit-reporter** using NPM command: "npm install --global mocha mocha-junit-reporter")
    10. **binaryen** (not required  but useful tools)  e.g. emsdk install binaryen-tag-1.38.30-64bit
- Napi - To create a Napi build for Nakasendo please follow these steps
    1. Ensure that your installation of Node, node-gyp and NPM are up to date and that you have a working version of gcc/llvm/MSVC installed. 
    2. Refer to the [Prerequisite Tools](DeveloperSetup.md#'Prerequisite Tools') section for a comprehensive list of tools required to build the Nakasendo API.
    3. Update the 'include_dirs' and ‘libraries’ sections of the binding.gyp as shown in the code section at the foot of this numbered list of steps.
    4. The ***sdklibraries_root*** value should be replaced with the path to the source-code download from github or the location of the extracted tar ball. 
    5. The ***installation_root*** value should be replaced with the path to the extracted tar ball or to the build location of the tar ball.
    6. The ***openssl*** value should be replaced with the location of your local installation of the openssl development tools. Please not these values are also in the libraries portion of the make file. For sourcing the Nakasendo components, either the ***installation_root*** or the location of the local build of the library can be used. 
    7. Run **npm run build** or **npm -i** in the NapiBindings directory.
    8. Set your LD_LIBRARY_PATH (unix/macosx) or PATH (Windows) to include the location of the installed Nakasendo libraries and then run node index.js
```console
  'include_dirs': [
  "<!@(node -p \"require('node-addon-api').include\")","<openssl>/include",
  "<sdklibraries_root>/NapiBinding/cppsrc/Nakasendo_Bindings",
  "<sdklibraries_root>/NapiBinding/cppsrc/Nakasendo_TS_Bindings",
      "<sdklibraries_root>/src/core",
  "<installation_root>/generated/hpp",
  "<sdklibraries_root>/src/applications/TS_protobuf/cpp",
  "<installation_root>/generated/protobuf"
  ],  
  'libraries': ['-Wl,<openssl>/lib/libcrypto.a -L <installation_root>/x64/release 
  -lBigNumbers -lPolynomial -lECPoint -lMessageHash 
  -lSymEncDec -lAsymKey -lBSVAddress -lTSCore'],
```

  - **pathlib**       useful
  - **pytest**        to run Python tests
  - **ecdsa**         to study elliptic curves

## Build

**On Windows**

```console
C:\development\build> cmake -G"Visual Studio 15 2017" -A x64 ..\SDKLibraries && cmake --build . --target ALL_BUILD --config Debug && cmake --build . --target ALL_BUILD --config Release
```

If you wish to execute our test framework
```console 
C:\development\build> ctest -C Release
```

If you wish to create a Windows installer program you will need to install NSIS 3.04
```console 
C:\development\build> cpack -G NSIS -C Release
```

**On Linux**
```console
nchain@sdk:~$ cmake ../SDKLibraries -DCMAKE_BUILD_TYPE=Debug -DCUSTOM_SYSTEM_OS_NAME=Ubuntu; time -p make -j8
```

If you wish to execute our test framework
```console
nchain@sdk:~$ ctest
```

If you wish to create a Linux distribution
```console
nchain@sdk:~$ cpack -G TGZ
```

## Tests (optional)

**To run all tests**
```console
nchain@sdk:~$ ctest             # or "make test" on linux

C:\nchain\sdk> ctest -C Release # On Windows
C:\nchain\sdk> ctest -C Debug   # On Windows
```

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


