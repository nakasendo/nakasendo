#!/usr/bin/env python3

import os, sys
import pathlib
import argparse

default_module_dir = os.path.dirname(os.path.abspath(__file__))
parser = argparse.ArgumentParser(description='Replace string arguments parser')
parser.add_argument('--module_dir', metavar='-w', default=default_module_dir, help='default:{}'.format(default_module_dir))
parser.add_argument('--data_dir', metavar='-w', default=default_module_dir, help='default:{}'.format(default_module_dir))
test_args_input = parser.parse_args()

if not test_args_input.module_dir:
    raise Exception('--You have to specify module_dir argument to run the tests')
if not test_args_input.data_dir:
    raise Exception('--You have to specify data_dir argument to run the tests')

print('TestArgsParser will try to load Py Module from : {}'.format(test_args_input.module_dir))
test_module_dir = pathlib.Path(test_args_input.module_dir)
test_module_dir_str =str(test_module_dir)
sys.path = [test_module_dir_str] + sys.path

test_data_dir = pathlib.Path(test_args_input.data_dir)
test_data_dir_str =str(test_data_dir)
print('TestArgsParser use data from : {}'.format(test_data_dir_str))

## Import PyModules
import PyBigNumbers, PyMessageHash, PySymEncDec, PyECPoint, PyAsymKey