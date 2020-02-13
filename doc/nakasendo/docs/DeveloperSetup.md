In order to develop/ contribute to Nakasendo <sup>TM</sup>, it is required to install and prepare some dev environments. It consists of installing tools/libraries for C++/Python/Javascript. It is recommended to:

- Build from source when using a Linux environment
- Download prebuilt binaries when using a Windows environment
- Install everything in the same location and define environment variables pointing to the installed location. The build system will find them appropriately

---

## Prerequisite Tools
- [Python 3.7.3 (64-bit)](https://www.python.org/downloads/release/python-374/) - 
As part of the Python Windows installer, ensure to check the installation options for the "Download debugging symbols" and the "Download debug binaries (requires VS 2015 or later)" checkboxes.
- [CMake 3.14.7](https://cmake.org/download/)
- C++ Compiler: Visual Studio Community Edition 2017 on windows, g++7 on Linux
- OpenSSL 1.1.b : It needs two versions: one for native C++, the other for emcripten. The prebuilt openssl for Emscripten can be downloaded from our [GitHub](https://github.com/nakasendo/nakasendo/releases)
- Protobuf 3.11.0 : either on Windows or Linux, compile it from source and install it. Then name the system variable Protobuf_ROOT pointing to its installation location. See detail in $SDK_SOURCE/cmake/modules/FindProtobufHelper.cmake

---

## Dependencies
Dependencies marked optional apply if you wish to run the unit tests. See [Tests](#Tests) for more details.

---

## Build Environments

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

---

## Tests <a name="Tests"></a>

Once the build tools and libraries are prepared, some post installation steps are required to let the Nakasendo<sup>TM</sup> build system know how to find everything:

**To build only the library**

- Add the directory of CMake executable to the environment variable PATH
- Add the directory of Python3 executable to the environment variable PATH
- Set environment variable OPENSSL_ROOT_DIR pointing to the root of the OpenSSL for C++ installation

**To build the tests**

- Set environment variable BOOST_ROOT pointing to the root of the Boost installation

**To run all tests**
```console
nchain@sdk:~$ ctest             # or "make test" on linux

C:\nchain\sdk> ctest -C Release # On Windows
C:\nchain\sdk> ctest -C Debug   # On Windows
```

**To build Webassembly**

- Add C:\Development\repos\emsdk\clang\e1.38.30_64bit to the environment variable PATH
- Add emscripten path to environment variable PATH
- Add the directory of Emscripten executable to the environment variable PATH
- Set environment variable **OPENSSL_WASM_ROOT_DIR** pointing to the root of the OpenSSL for Emscripten installation

Nakasendo<sup>TM</sup> can be built on any system, as long as all libraries and required tools are manually compiled to ensure absolute compatibility. Users can choose the appropriate [CMake generator](https://cmake.org/cmake/help/v3.14/manual/cmake-generators.7.html) to make the build.
The CMake build allows everything to be built separately from the source to keep the source repository clean. After cloning the source code to "SDKLibraries" directory, create a "build" directory alongside this, then run the cmake/make command to build

**C++ examples**
On Windows, you'll need Visual Studio Community Edition 2017 and CMake 3.14.7. On Linux, you'll need gcc7, cmake 3.12 or newer.
On Windows, run the following command e.g:
```console
C:\nchain\sdk> cmake -G"Visual Studio 15 2017" -A x64 "%SDKLIBRARIES_ROOT%\example\cpp"
```
This will create the Visual Studio solution (.sln) file. 
Add %SDKLIBRARIES_ROOT%\lib to your %PATH%
Open the .sln file in Visual Studio and you can start to Build and Run the C++ examples.
[You may also have to install Twisted, Python 3, pip3, ecdsa, nodejs and npm]

On Linux (Ubuntu 18.04), run the command :
```console
nchain@sdk:~$ cmake -DCMAKE_BUILD_TYPE=Debug "$SDKLIBRARIES_ROOT/example/cpp" ; make -j4
```
It will create a Makefile project and build it. You can start to compile/run the c++ examples

**Python examples**
Python examples need to know where to find all the python modules.
If you have defined the variable **SDKLIBRARIES_ROOT** correctly, just open python files and run it normally.
Another way is to define the **PYTHONPATH** variable pointing to the installation of Nakasendo<sup>TM</sup> (where it has all module files) and run it
Note that on windows, if you have a debug version of Nakasendo <sup>TM</sup> installation, you should use the python_d interpreter instead of python

**Javascript examples**
To run javascript example, you need a http server. The simplest way is to use python to run the http server.
Open the terminal, go to $SDKLIBRARIES_ROOT/example/javascript, run the comnmand :
```console
nchain@sdk:~$ python -m http.server
```
Then from the browser, open **localhost:8000**
