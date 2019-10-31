#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
## Include guard
if(FindOpenSSLHelper_Include)
  return()
endif()
set(FindOpenSSLHelper_Include TRUE)

## For Windows, download binary prebuilt OpenSSL and set %OPENSSL_ROOT_DIR% pointing to the root of the installed directory
## Take the first suggestion from https://wiki.openssl.org/index.php/Binaries. splroweb
## For Linux, build from source using the command :
##     ./config shared --prefix=/path/to/installed/directory --openssldir=/path/to/installed/directory && make depend && make -j8 && make test && make install
## set $OPENSSL_ROOT_DIR pointing to the root of the installed directory
##
## 
## 
#[[############################ Build openssl wasm  ############################
# Emscripten build is really portable cross plateform, so it only need to build on linux (easier) then the wasm build can be used on windows (for wasm only)
# Download and install emsdk following emscripten documentation https://emscripten.org/docs/getting_started/downloads.html
# set environment variables in ~/.bashrc
#export EMSDK_ROOT=/path/to/where/emsdk/installed
#export OPENSSL_WASM_ROOT_DIR=/path/to/where/openssl-1.1.1b_wasm/will/be/installed

source ~/.bashrc
wget https://www.openssl.org/source/openssl-1.1.1b.tar.gz  # Download openssl-1.1.1b
tar xvzf openssl-1.1.1b.tar.gz                             # Extract it
mv openssl-1.1.1b openssl-1.1.1b_src                       # Rename openssl source directory
cd openssl-1.1.1b_src                                      # Goto openssl source directory
source $EMSDK_ROOT/emsdk_env.sh                            # Setup emscripten environment variables
## Configure the build for openssl
emconfigure ./Configure --openssldir=/path/to/some/tmp/build/dir --prefix=$OPENSSL_WASM_ROOT_DIR no-asm no-dso no-shared no-threads linux-x86_64-clang

## Fix some minor issues : https://github.com/openssl/openssl/issues/5443
# Go to Makefile, make those changes :
# - CROSS_COMPILE set to blank
# - CNF_CFLAGS remove any of the options -arch armv7 -mios-version-min=6.0.0 -fno-common -isysroot # for openssl-1.1.b, it's ok to not doing anything
# - CFLAGS add -D__STDC_NO_ATOMICS__=1
# - modify all -O3 to -O0

emmake make -j4 ; emmake make install                          # Build and install openssl wasm
## To test the linking, create some test file using openssl, then
cd /path/to/some/tmp/directory/
emcc test_link_openssl_wasm.c -lssl -lcrypto -L $OPENSSL_WASM_ROOT_DIR/lib -I $OPENSSL_WASM_ROOT_DIR/include -std=c++11 -s WASM=1 -o index.html
python3 -m http.server
## open your browser and open https://localhost:8000
############################ Build openssl wasm  ############################
]]

#### Preset some variable to find and import OpenSSL
function(presetOpenSSLVariable)##########################################################################
  set(OPENSSL_USE_STATIC_LIBS ON CACHE BOOL "Preset variable to find OpenSSL" FORCE)## Force to use static version
  set(OPENSSL_MSVC_STATIC_RT ON CACHE BOOL "Preset variable to find OpenSSL" FORCE)## If turn it ON, should change the windows compiler flag to build the solution in MT mode
endfunction()

#### Help to find OpenSSL. Need to define the env variable OPENSSL_ROOT_DIR pointing to the root of installed directory
macro(HelpFindOpenSSL)########################################################################################

  presetOpenSSLVariable()
  find_package(OpenSSL REQUIRED)

  #sdkPrintOpenSSLInfo()#DebugLog
endmacro()


function(sdkPrintOpenSSLInfo)
  sdkPrintProperties(OpenSSL::Crypto)
  sdkPrintProperties(OpenSSL::SSL)

  message(" --")
  message(" ---------- OPENSSL_FOUND [${OPENSSL_FOUND}]")
  message(" ---------- OPENSSL_INCLUDE_DIR [${OPENSSL_INCLUDE_DIR}]")
  message(" ---------- OPENSSL_CRYPTO_LIBRARY [${OPENSSL_CRYPTO_LIBRARY}]")
  message(" ---------- OPENSSL_SSL_LIBRARY [${OPENSSL_SSL_LIBRARY}]")
  message(" ---------- OPENSSL_LIBRARIES [${OPENSSL_LIBRARIES}]")
  message(" ---------- OPENSSL_VERSION [${OPENSSL_VERSION}]")
  message(" ---------- OPENSSL_VERSION_MAJOR [${OPENSSL_VERSION_MAJOR}]")
  message(" ---------- OPENSSL_VERSION_MINOR [${OPENSSL_VERSION_MINOR}]")
  message(" ---------- OPENSSL_VERSION_FIX [${OPENSSL_VERSION_FIX}]")
  message(" ---------- OPENSSL_VERSION_PATCH_STRING [${OPENSSL_VERSION_PATCH_STRING}]")
endfunction()
