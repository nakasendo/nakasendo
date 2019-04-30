#!/bin/sh

cd src/core/MessageHash
make all
python MessageHashAPISetup.py build
cd ../BigNumbers
make all
python BigNumSetup.py build
