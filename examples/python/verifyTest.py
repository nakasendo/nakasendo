#!/usr/bin/python

import random
import sys
import hashlib
import ecdsa

G= ecdsa.SECP256k1.generator
N = ecdsa.SECP256k1.order

# NIST Curve P-192:
p = 6277101735386680763835789423207666416083908700390324961279
r = 6277101735386680763835789423176059013767194773182842284081
# s = 0x3045ae6fc8422f64ed579528d38120eae12196d5
# c = 0x3099d2bbbfcb2538542dcd5fb078b6ef5f3d6fe2c745de65
b = 0x64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1
Gx = 0x188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012
Gy = 0x07192b95ffc8da78631011ed6b24cdd573f977a11e794811


c192 = ecdsa.ellipticcurve.CurveFp(p, -3, b)
p192 = ecdsa.ellipticcurve.Point(c192, Gx, Gy, r)



try:
    import PyBigNumbers
    import PyECPoint
    import PySymEncDec
    import PyMessageHash
    import PyAsymKey
    import PyPolynomial
    import Nakasendo
except ImportError as e:
    print('Error while loading SDKLibraries python modules {}'.format(e.message))
    print('Try to define environment variable SDKLIBRARIES_ROOT pointing to the location of installed SDKLibraries or add this to PYTHONPATH')
    raise ImportError('Unable to load SDKLibraries python modules')


def HashMsg(msg,modulo=None, IsDec=False):
    HASHMSG = Nakasendo.hash256(msg)
    if(IsDec):
        Hm = Nakasendo.BigNum(str(int(HASHMSG.value,16)), modulo.value, isDec=True)
    else:
        Hm = Nakasendo.BigNum(HASHMSG.value,modulo.value, isDec=False)
    return Hm

if __name__ == "__main__":
    print ("...starting....")
    #print (sys.argv)
    sigRStr = sys.argv[1]
    sigSStr = sys.argv[2]
    pubKeyStr = sys.argv[3]
    message = sys.argv[4]
    
    dersigStr = None
    if(len(sys.argv) == 6):
        dersigStr=sys.argv[5]
    
    print ('[%s, %s] \n pubkey %s \nmessage = %s' %(sigRStr,sigSStr,pubKeyStr,message))
    # create an ECPoint
    pt = Nakasendo.ECPoint()
    pt.value = pubKeyStr
    print(pt.GetAffineCoOrdinates())
    
    coords = pt.GetAffineCoOrdinates()
    
    pempubkey = Nakasendo.pubKeyHexPtasPem(coords[0],coords[1])
    print(pempubkey)
    print('verify the pem format')
    pubKeyStrValidate = Nakasendo.pubKeyPEMasHex(pempubkey, True);
    print(pubKeyStrValidate)
    # get the affine co-ordinates
    # get pem format
    
    val = Nakasendo.verify(message, pempubkey, sigRStr, sigSStr)
    if(val == True):
        print('Calculated signature verified via Nakasendo verify')
        
    hmMessage = Nakasendo.hash256(message=message,modulo="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
    #create an ecdsa public key from the program parameter
    ecdsaPubKeyPoint = ecdsa.ellipticcurve.Point(ecdsa.SECP256k1.generator.curve(), int(coords[0],16), int(coords[1],16))
    pubkeytestB = ecdsa.ecdsa.Public_key(G, ecdsaPubKeyPoint)
    
    thresHoldSigLocalCal = ecdsa.ecdsa.Signature(int(sigRStr,16),int(sigSStr,16))
        
    if(pubkeytestB.verifies(int(hmMessage.value,16), thresHoldSigLocalCal)):
        print ("SUCESS WITH TS via ECDSA library .. point constructed from public key parameter")
    else:
        print("FAILURE WITH TS via ECDSA library")
        
    
    if(dersigStr is not None):
        orderlen = (1+len("%x" % N))//2 
        print(orderlen)
        print(len(dersigStr))
        print(len(bytes(dersigStr,'utf-8')))
        
        
        rBN = Nakasendo.BigNum(value=sigRStr,mod="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
        sBN = Nakasendo.BigNum(value=sigSStr,mod="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
        derSigInputBN = Nakasendo.BigNum(value=dersigStr,mod="FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141")
        DerFormatSigTest = Nakasendo.createDERFormat(rBN,sBN)
        assert (DerFormatSigTest == DerFormatSigTest)
        
        val = Nakasendo.verifyDER(message, pempubkey, dersigStr,False)
        if(val == True):
            print ('TS DER format verified')
    
    
