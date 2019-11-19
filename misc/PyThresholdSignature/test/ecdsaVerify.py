#¡/usr/local/bin/python


import time
import random
import sys
import PyPlayer
import ecdsa

G= ecdsa.SECP256k1.generator

try:
    import PyBigNumbers
    import PyECPoint
    import PySymEncDec
    import PyMessageHash
    import PyAsymKey
    import PyPolynomial
    import Nakasendo
    import Threshold
except ImportError as e:
    print('Error while loading SDKLibraries python modules {}'.format(e.message))
    print('Try to define environment variable SDKLIBRARIES_ROOT pointing to the location of installed SDKLibraries or add this to PYTHONPATH')
    raise ImportError('Unable to load SDKLibraries python modules')
    
    
if __name__ == "__main__":
    print ("...starting....")
    P = 329089934168988773665023929850092829462160520693935257033944358940573284829198
    
    print(P)
    pub_key = ecdsa.ecdsa.Public_key(G,P)
    #thresHoldSig = ecdsa.ecdsa.Signature(interpolated_r,interpolated_s)
