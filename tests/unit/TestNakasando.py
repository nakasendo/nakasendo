#!/usr/bin/env python3
import sys
import json
import string
#Please update the PYTHONPATH or use the sys.path.append with the path to 
#the Nakasando installation
#sys.path.append('/home/osboxes/nchain/SDK/sdklibraries/src/modules')
#sys.path.append ('/home/osboxes/nchain/SDK/build/x64/release')
import PyBigNumbers
import PyECPoint
import PySymEncDec
import PyMessageHash
import Nakasando

if __name__ == "__main__":
    print ("starting.....")
    bigNumA = Nakasando.BigNum()
    bigNumB = Nakasando.BigNum()
    
    c = bigNumA + bigNumB
    print ("operator A ->  %s, ..... operator B-> %s" % (bigNumA, bigNumB))
    print("...result of addition %s", c)


    ECPointA  = Nakasando.ECPoint ()
    print ("ECPointA output %s" % ECPointA ) 

    ECPointB = Nakasando.ECPoint ()
    print ("ECPointB output %s " % ECPointB)

    ECPointC = ECPointA + ECPointB
    print ("ECPointC (ECPointA + ECPointB) output %s " % ECPointC)


    print ("Testing multiplication (2 paramters)... ")
    BigNumScalerA = Nakasando.BigNum()
    ECPointG = Nakasando.ECPoint(); 

    ECPointRes = ECPointG.multipleScalar(BigNumScalerA)
    print("Multiplication res: %s " % ECPointRes)

    print ("Testing multiplication (3 paramters)... ")
    BigNumScalerB = Nakasando.BigNum()
    ECPointRes1 = ECPointG.multipltScalarEx(BigNumScalerA,BigNumScalerB)
    print ( "3 param Multiplicatin res: %s " % ECPointRes1)

    ECPointFromRes1 =Nakasando.ECPoint()
    if (ECPointFromRes1.IsPointOnCurve() == True):
        print( "Point validated on curve...")

    
    print ("Testing Affine co-ordinates call" )
    coords = ECPointRes1.GetAffineCoOrdinates()
    print ("Random Elliptic Curve Point P:\n x = %s \ny= %s" % (coords[0], coords[1]))

    print ("Testing comparison...")
    if ( ECPointA == ECPointB):
        print ("ECPtA == ECPtB")
    else:
        print ("ECPtA != ECPtB")


    print ("Creating a point on a curve with ID - 704 ... I don;t like this as it requires users to know the ID of the curve")
    ECPointOnScep112r1_a = Nakasando.ECPoint(704)
    ECPointOnScep112r1_b = Nakasando.ECPoint(704)

    print ( "ECPointOnScep112r1_a: %s\nECPointOnScep112r1_b %s\n" % (ECPointOnScep112r1_a, ECPointOnScep112r1_b))
    ECPointOnScep112r1_sum = ECPointOnScep112r1_a + ECPointOnScep112r1_b
    print ("Result of addtion on a curve with ID - 704: %s " % ECPointOnScep112r1_sum)

    print ("Check if its on the curve")

    print ("Check Encoding/Decoding/Key creation")
    password='j.murphy@nchain.com'
    iv='05101974'
    encoder = Nakasando.SymEncDec (password,iv)
    print ("Contents: %s" % encoder)
    encoder.GenerateKey(); 
    print ("Contents: %s" % encoder)

    msgToEncode='the quick brown fox jumped over the lazy dog'
    encodedAsHex = encoder.Encode(msgToEncode)
    print ("EncodedMsgAsHex: %s" % encodedAsHex)

    decoded =encoder.Decode(encodedAsHex)
    print ("Decoded Message %s" % decoded )

    print ("Testing Base64..Encoding")
    msgToEncode = 'Development team'
    for x in range(1,10):
        myMsgHash = Nakasando.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        print (encoded)
        decoded = myMsgHash.Bas64Decode(encoded)
        print (decoded)
    msgToEncode = 'Research team'
    for x in range (1,10):
        myMsgHash = Nakasando.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        decoded = myMsgHash.Bas64Decode(encoded)
        print(decoded)

    
    

    



    

    


