#!/bin/sh
ls -alrt
pwd
mkdir -p build
cd build
cmake ..
make
