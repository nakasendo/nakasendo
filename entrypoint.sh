#!/bin/sh
mkdir -p releasebuild
cd releasebuild
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCUSTOM_SYSTEM_OS_NAME=Ubuntu
make -j$(nproc --all)
cd -

mkdir -p debugbuild
cd debugbuild
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCUSTOM_SYSTEM_OS_NAME=Ubuntu
make -j$(nproc --all)
