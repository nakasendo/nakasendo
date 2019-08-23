#!/usr/bin/env python3
import os
import sys
import json
import string
import pathlib

## Try to prepend the $SDKLIBRARIES_ROOT to the system path
if 'SDKLIBRARIES_ROOT' in os.environ:
    sdk_libraries_root = pathlib.Path(os.environ['SDKLIBRARIES_ROOT'])
    sdk_libraries_lib_dir_str = str(sdk_libraries_root/ 'lib')
    sys.path = [sdk_libraries_lib_dir_str] + sys.path
    print('Found SDKLIBRARIES_ROOT="{}"'.format(str(sdk_libraries_root)))
    print('Modules directory      ="{}"'.format(sdk_libraries_lib_dir_str))

try:
    import PyBigNumbers
    import PyECPoint
    import PySymEncDec
    import PyMessageHash
    import Nakasendo
except ImportError as e:
    print('Error while loading SDKLibraries python modules {}'.format(e.message))
    print('Try to define environment variable SDKLIBRARIES_ROOT pointing to the location of installed SDKLibraries or add this to PYTHONPATH')
    raise ImportError('Unable to load SDKLibraries python modules')


if __name__ == "__main__":
    print ("starting.....")
    
    
    print ("Testing Base64..Encoding")
    msgToEncode = 'Development team'
    for x in range(1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        #print (encoded)
        if (encoded != 'RGV2ZWxvcG1lbnQgdGVhbQ=='):
            print ("WEIRDNESS")
        decoded = myMsgHash.Bas64Decode(encoded)
        #print (decoded)
    msgToEncode = 'Research team'
    for x in range (1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        #print(encoded)
        if (encoded != 'UmVzZWFyY2ggdGVhbQ=='):
            print ('WEIRDNESS')
        decoded = myMsgHash.Bas64Decode(encoded)
        #print(decoded)
        
    msgToEncode = 'nChain Limited UK branch is fast growing in FinTech industry'
    for x in range (1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        #print(encoded)
        if (encoded != 'bkNoYWluIExpbWl0ZWQgVUsgYnJhbmNoIGlzIGZhc3QgZ3Jvd2luZyBpbiBGaW5UZWNoIGluZHVzdHJ5'):
            print ('WEIRDNESS')
            print (encoded)
        decoded = myMsgHash.Bas64Decode(encoded)
        #print(decoded)
