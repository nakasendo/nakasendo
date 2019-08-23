#!/usr/bin/env python3

import sys
import random
import os
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

    print ("starting....")
    
    AliceKey = Nakasendo.ECKey256K1()
    BobsKey = Nakasendo.ECKey256K1()
    
    
    msg = 'This is a message that will be hased'
    
    #Alice and Bob preformn the same steps
    
    
    #ALice
    newAliceKeyPemFormat = AliceKey.derivePrivateKey(msg)
    AliceNewKey = Nakasendo.ECKey256K1()
    AliceNewKey.FromPEMStr(newAliceKeyPemFormat)
    
    pubKeyAlice = AliceNewKey.pubKey
    
    print(pubKeyAlice)
    
    
    
    #Bob
    newBobKeyPemFormat = AliceKey.derivePrivateKey(msg)
    BobsNewKey = Nakasendo.ECKey256K1()
    BobsNewKey.FromPEMStr(newBobKeyPemFormat)
    
    pubKeyBob = BobsNewKey.pubKey
    print(pubKeyBob)
    
    #Alice & Bob swap public keys and preform these steps independantly
    
    BobsSharedSecret = BobsNewKey.CalculateSharedSecret(pubKeyAlice)
    AlicesSharedSecret = AliceNewKey.CalculateSharedSecret(pubKeyBob)
    
    print(BobsSharedSecret)
    print(AlicesSharedSecret)
    
    
    
    
    
    
    
    
    
    
