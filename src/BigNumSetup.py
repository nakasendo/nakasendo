#!/usr/bin/env python3
import sys
from distutils.core import setup, Extension

static_libraries = ['ssl','crypto']

libraries = ['dl']

if sys.platform == 'win32':
    #libraries.extend(static_libraries)
    #library_dirs.append(static_lib_dir)
    extra_objects = []
else: # POSIX
    extra_objects = ['-l:lib{}.a'.format(l) for l in static_libraries]


module1 = Extension('BigNumAPI',
                    define_macros = [('MAJOR_VERSION', '1'),
                                     ('MINOR_VERSION', '0')],
                    include_dirs = ['core','../../build/generated_hpp'],
                    libraries = libraries,
                    library_dirs = [],
                    sources = ['modules/PyBigNumbers/BigNumPythonAPI.cpp','core/BigNumbers/BigNumAPI.cpp','core/BigNumbers/BigNumbers.cpp','core/BigNumbers/BigNumbersImpl.cpp'],
                    extra_objects=extra_objects)

setup (name = 'BigNumberAPI',
       version = '1.0',
       description = 'This is a demo package',
       author = 'Murphy',
       author_email = 'j.murphy@nchain.com',
       url = 'https://docs.python.org/extending/building',
       long_description = '''
This is really just a demo package.
''',
       ext_modules = [module1])
