FROM ubuntu:19.10

RUN apt-get update \
    && apt-get install -y git \
    gcc \
    g++  \
    make \
    wget \
    cmake \
    build-essential\
    maven \
    ca-certificates \
    gosu \
    dirmngr \
    libboost-all-dev \
    libtool \
    libssl-dev \
    libzmq3-dev \
    python3 \
    python3-pip \
    coreutils \
    doxygen \
    libgmp3-dev \
    libkeyutils1 \
    vim \
    rpm \
    npm \
    software-properties-common
    
RUN npm install -g mocha mocha-junit-reporter binaryen && pip3 install pytest certifi && apt-get purge -y --auto-remove openssl \
   && apt-get install -y ca-certificates && update-ca-certificates --fresh && export SSL_CERT_DIR=/etc/ssl/certs && apt-get clean

#update the memory settings according to the SDK source
RUN sed -i.bak '/# End of file/i root\t\thard\tmemlock\t\tunlimited\nroot\t\tsoft\tmemlock\t\tunlimited' /etc/security/limits.conf \
    && sed -i.bak '$a kernel.keys.maxkeys = 25000\nkernal.keys.maxbytes = 2500000' /etc/sysctl.conf

# install key utils
WORKDIR /home
RUN wget -O keyutils-1.5.10.tar.bz2  http://people.redhat.com/~dhowells/keyutils/keyutils-1.5.10.tar.bz2 \
    && tar xvf keyutils-1.5.10.tar.bz2 && cd keyutils-1.5.10 && make && make install \
    && cp /x86_64-linux-gnu/* /usr/lib/x86_64-linux-gnu

WORKDIR /
SHELL ["/bin/bash", "-c"]
# Get the emsdk repo
RUN  git clone https://github.com/emscripten-core/emsdk.git

WORKDIR /emsdk
RUN ./emsdk list && ./emsdk install 1.38.42 && ./emsdk activate 1.38.42 && chmod a+x ./emsdk_env.sh

WORKDIR /
RUN OPENSSL_VERSION="1.1.1d" ;  . /emsdk/emsdk_env.sh ; \
    wget https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz ;  \
    mkdir -p openssl ; \
    tar xvf "openssl-${OPENSSL_VERSION}.tar.gz" --strip-components=1 --directory=openssl
    
WORKDIR /openssl
RUN export CC=emcc ; export CXX=emcc ; export LINK=${CXX} ;export ARCH_FLAGS="" ; export ARCH_LINK="" ; \
    export CXX=${CC} ; export LINK=${CXX} ; export ARCH_FLAGS="" ; export ARCH_LINK="" ; \
    export CPPFLAGS=" ${ARCH_FLAGS} " ; export CXXFLAGS=" ${ARCH_FLAGS} " ; \
    export CFLAGS=" -D__STDC_NO_ATOMICS__=1 -DUSE_PTHREADS=0" ; export LIBS=-ldl ;\
    . /emsdk/emsdk_env.sh ; \
    ./Configure linux-generic64 no-shared no-ssl2 no-ssl3 no-comp \
    no-engine no-dso no-dgram no-sock no-threads no-ocsp no-psk no-ts no-asm  --prefix=/openssl --openssldir=/openssl && \
    PATH=$TOOLCHAIN_PATH:$PATH __STDC_NO_ATOMICS__=1 USE_PTHREADS=0 make depend && __STDC_NO_ATOMICS__=1 USE_PTHREADS=0 \
    make  &&  unset CC ; unset CXX ; unset LINK ; unset ARCH_FLAGS ; unset ARCH_LINK ; \
    unset CXX ; unset LINK ; unset ARCH_FLAGS ; unset ARCH_LINK ; unset CPPFLAGS ; unset CXXFLAGS ; \
    unset CFLAGS ; unset LDFLAGS && mkdir /openssl/lib && cp /openssl/libssl.a /openssl/lib/ && cp /openssl/libcrypto.a /openssl/lib/

WORKDIR /
   
RUN mkdir -p /opt/openssl && OPENSSL_VERSION="1.1.1d" ; tar xvf "openssl-${OPENSSL_VERSION}.tar.gz" --directory=/opt/openssl
WORKDIR /opt/openssl/openssl-1.1.1d
RUN  export LIBS=-ldl && ./config no-shared --prefix=/opt/openssl --openssldir=/opt/openssl \
     && make && make install

WORKDIR /

RUN  git clone https://github.com/protocolbuffers/protobuf.git 
WORKDIR protobuf
RUN  git submodule update --init --recursive && ./autogen.sh && ./configure && make && make install && ldconfig 

WORKDIR /

COPY ./devops/entrypoint.py .
COPY ./devops/releasenotes.py .
RUN chmod +x /entrypoint.py && chmod +x /releasenotes.py && useradd -G users Jenkins
USER Jenkins

