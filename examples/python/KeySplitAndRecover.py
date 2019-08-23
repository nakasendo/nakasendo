#!/usr/bin/python

import sys
import os
import json
import random
import pathlib

# Try to prepend the $SDKLIBRARIES_ROOT to the system path
if 'SDKLIBRARIES_ROOT' in os.environ:
    sdk_libraries_root = pathlib.Path(os.environ['SDKLIBRARIES_ROOT'])
    sdk_libraries_lib_dir_str = str(sdk_libraries_root/ 'lib')
    sys.path = [sdk_libraries_lib_dir_str] + sys.path
    print('Found SDKLIBRARIES_ROOT="{}"'.format(str(sdk_libraries_root)))
    print('Modules directory      ="{}"'.format(sdk_libraries_lib_dir_str))
    
try:
    import Nakasendo
except ImportError as e:
    print('Error while loading SDKLibraries python modules {}'.format(e.message))
    print('Try to define environment variable SDKLIBRARIES_ROOT pointing to the location of installed SDKLibraries or add this to PYTHONPATH')
    raise ImportError('Unable to load SDKLibraries python modules')

if __name__ == "__main__":

    mykey = Nakasendo.ECKey256K1();
    print (mykey)

    shares = mykey.SplitKey(5,15);
    
    print(shares)
    
    #At some point later....
    print ("a random collection of shares")
    
    secure_random = random.SystemRandom()
    
    subset = []
    sharesCopy = shares
    random.shuffle (sharesCopy)
    for i in range(5):
        subset.append(sharesCopy[i])
    
    print(subset)
    
    
    
    
    recoveredKey = Nakasendo.ECKey256K1()
    recoveredKey.RecoverKey(subset)
    
    print(recoveredKey)
    
