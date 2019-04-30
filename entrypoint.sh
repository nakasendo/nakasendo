#!/bin/sh

cd MessageHash
make all
python MessageHashAPISetup.py build
cd BigNumbers
make all
python BigNumSetup.py build
