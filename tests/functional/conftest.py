#!/usr/bin/env python3

import os, sys
import pathlib
import pytest

#### Run individual test from command line with rootdir will load conftest.py
#python -m pytest -s --rootdir=/Path/to/root/test/dir --module_dir=/Path/to/module/dir --data_dir=/path/to/data/dir my_test_file.py

#### From Pycharm : https://www.jetbrains.com/help/pycharm/pytest.html
#### Open the Settings/Preferences | Tools | Python Integrated Tools settings dialog, chose In the "Default test runner" field select pytest.
#### From an instance, go to "Edit Configuration" on the top-right,
#### Create a Python tests :
####    Specify Scrip path to the test file
####    Specify Additional Arguments : -s --module_dir=/Path/to/module/dir --data_dir=/path/to/data/dir ## Option -s is to show the print to std output

default_module_dir = os.path.dirname(os.path.abspath(__file__))
default_data_dir = os.path.dirname(os.path.abspath(__file__)+'/test_data')
def pytest_addoption(parser):## hack to make pytest accept arguments
    parser.addoption('--module_dir', metavar='-w', default=default_module_dir,help='default:{}'.format(default_module_dir))
    parser.addoption('--data_dir', metavar='-w', default=default_data_dir,help='default:{}'.format(default_data_dir))

def pytest_configure(config):
    import os, sys
    arg_module_dir = config.getoption("--module_dir")
    sys.path = [arg_module_dir] + sys.path
    import PyBigNumbers, PyMessageHash, PySymEncDec, PyECPoint, PyAsymKey
    print('conftest.py successfully load all Py Modules')
    ## actual_Value = PyBigNumbers.GenerateRandHex(20) ## Try to test using PyBigNumbers

@pytest.fixture
def test_data_dir(request):
    return pathlib.Path(request.config.getoption("--data_dir"))