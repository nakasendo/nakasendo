## Installation

Download the [Nakasendo installer<sup>TM</sup>](https://github.com/nakasendo/nakasendo) for your preferred platform.

The best way to run the example code is to define the environment variable **SDKLIBRARIES_ROOT** pointing to the location where Nakasendo<sup>TM</sup> is installed.

## C++ examples
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

## Python examples
Python examples need to know where to find all the python modules.
If you have defined the variable **SDKLIBRARIES_ROOT** correctly, just open python files and run it normally.
Another way is to define the **PYTHONPATH** variable pointing to the installation of Nakasendo installer<sup>TM</sup> (where it have all module files) and run it
Note that on windows, if you have a debug version of SDK installation, you should use the python_d interpreter instead of python

## Javascript examples
To run javascript example, you need a http server. The simplest way is to use python to run the http server.
Open the terminal, go to $SDKLIBRARIES_ROOT/example/javascript, run the comnmand :
```console
nchain@sdk:~$ python -m http.server
```
Then from the browser, open **localhost:8000**