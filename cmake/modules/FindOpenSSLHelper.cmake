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

#### For Windows, download binary prebuilt OpenSSL and set %OPENSSL_ROOT_DIR% pointing to the root of the installed directory
#### Take the first suggestion from https://wiki.openssl.org/index.php/Binaries. splroweb
#### For Linux, build from source using the command :
####     ./config shared --prefix=/path/to/installed/directory --openssldir=/path/to/installed/directory && make depend && make -j8 && make test && make install
#### set $OPENSSL_ROOT_DIR pointing to the root of the installed directory


#### Preset some variable to find and import OpenSSL
function(presetOpenSSLVariable)##########################################################################
  set(OPENSSL_USE_STATIC_LIBS ON CACHE BOOL "Preset variable to find OpenSSL" FORCE)## Force to use static version
  set(OPENSSL_MSVC_STATIC_RT ON CACHE BOOL "Preset variable to find OpenSSL" FORCE)##
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
