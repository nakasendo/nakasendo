#!/bin/bash

#######################################################################################
# prerequisites
# * Emscripten
#######################################################################################

GIT=0

# ==> Select latest version for security reasons
OPENSSL_VERSION="1.1.0h"
#OPENSSL_VERSION="1.1.1-pre8"

if [ "$GIT" -eq 1 ]; then
  if [ ! -d openssl ]; then
    git clone --depth 10 https://github.com/openssl/openssl.git
  fi
  cd openssl || exit 1
  git pull
else
  if [ ! -f openssl-${OPENSSL_VERSION}.tar.gz ]; then
    wget https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz
  fi

  rm -rf "openssl"
  mkdir openssl
  tar xvf "openssl-${OPENSSL_VERSION}.tar.gz" --strip-components=1 --directory=openssl
  cd openssl
fi

export CCCMD='docker run --rm -v '
echo $CCCMD
export CCCMDPATH=$(pwd)
echo $CCCMDPATH
export CCCMDPATHBACK=':/src -u 1000 trzeci/emscripten emcc'
echo $CCCMDPATHBACK
export CC=$CCCMD$CCCMDPATH$CCCMDPATHBACK

export CXX=${CC}
export LINK=${CXX}
export ARCH_FLAGS=""
export ARCH_LINK=""
export CPPFLAGS=" ${ARCH_FLAGS} "
export CXXFLAGS=" ${ARCH_FLAGS} "
export CFLAGS=" ${ARCH_FLAGS} "
export LDFLAGS=" ${ARCH_LINK} "
echo  $OSTYPE | grep -i darwin > /dev/null 2> /dev/null
if [ $? -eq 0 ]; then
  OLD=$(pwd)
  HOMEBREW_PREFIX="/usr/local/Cellar/binutils"
  cd $HOMEBREW_PREFIX || exit 1
  TARGET=$(find . -name ar)
  export AR=${HOMEBREW_PREFIX}/${TARGET}
  cd "$OLD"
fi

./Configure \
  --openssldir=/tmp \
  --api=1.1.0 \
  no-engine no-dso no-dgram no-sock no-srtp no-ui no-ocsp no-psk no-ts no-asm \
linux-generic32
PATH=$TOOLCHAIN_PATH:$PATH make
