#!/usr/bin/env python3

"""
Python script helping to load Py Modules given the modules path
Usage :
    PyModuleLoader --module_dir="/absolute/path/to/pymodules/dir" # (use unix path)

Or copy this file in your python working directory. In your python script,
    import PyModuleLoader
Run your script with the argument
    MyPythonTestScript.py --module_dir="/absolute/path/to/pymodules/dir"
"""

import os, sys
import pathlib
import argparse

default_module_dir = os.path.dirname(os.path.abspath(__file__))
parser = argparse.ArgumentParser(description='Replace string arguments parser')
parser.add_argument('--module_dir', metavar='-w', default=default_module_dir, help='default:{}'.format(default_module_dir))
args = parser.parse_args()
print('PyModuleLoader will try to load module from : {}'.format(args.module_dir))
sys_module_dir = pathlib.Path(args.module_dir)
sys_module_dir_str =str(sys_module_dir)
sys.path = [sys_module_dir_str] + sys.path

## In your python script, you can start to import Py Modules and use them
#import PyBigNumbers, PyMessageHash, PySymEncDec, PyECPoint, PyAsymKey