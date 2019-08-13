#!/usr/bin/env python3
import sys
import json
import string
#Please update the PYTHONPATH or use the sys.path.append with the path to 
#the Nakasando installation
import PyBigNumbers
import PyECPoint
import PySymEncDec
import PyMessageHash
import PyAsymKey
import PyPolynomial
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
    encoder = Nakasando.SymEncDec (password)
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

    
    
   
    
    msgToEncode='the quick brown fox jumped over the lazy dog'
    encodedAsHex = encoder.Encode(msgToEncode)
    print ("EncodedMsgAsHex: %s" % encodedAsHex)

    decoded =encoder.Decode(encodedAsHex)
    print ("Decoded Message %s" % decoded )    
    
    mykey = Nakasando.ECKey256K1();
    print (mykey)
    print ("derive a publci key..")
    myderivedKey = mykey.derivePublicKey("Using this message")
    print(myderivedKey)
    shares = mykey.SplitKey(3,6);

    print ('And now recover')
    recoveredKey = Nakasando.ECKey256K1()
    recoveredKey.RecoverKey(shares)
    print (recoveredKey)
    
    
    
    
    #Sign a msessage
    msg = 'The quick brown fox jumped over the lazy dog'
    sig = mykey.sign(msg);
    print(sig)
    verifyIt = Nakasando.verify(msg, mykey.pubKey, sig[0], sig[1])
    if (verifyIt == True):
        print ('msg verified')
        
    #generate a shared secret
    
    AliceKey = Nakasando.ECKey256K1();
    BobsKey = Nakasando.ECKey256K1();
    
    msgFromBob = BobsKey.CalculateSharedSecret(AliceKey.pubKey); 
    msgFromAlice = AliceKey.CalculateSharedSecret(BobsKey.pubKey);
    
    print(msgFromBob)
    print(msgFromAlice)
    
    
    # Test1 randomPolynomial( degree, modulo )
    # Test2 randomPolynomial( degree, modulo, fixed a_0 )
    # Test3 randomPolynomial( degree, modulo, min, max )
    # Test4 evalute for x=2
    # Test5 create polynomial from list


    #Test1
    print("Testing initRandom (degree, modulo) ")
    degree = 3
    modulo = "17"
    poly1 = Nakasando.Polynomial.initRandom( degree, modulo )
    print ( "Nakasando Polynomial = ", poly1 )

    #Test2
    print("Testing initRandomFixed_a_0 (degree, modulo, fixed a_0) ")
    degree = 3
    modulo = "17"
    a_0 = "5"
    poly2 = Nakasando.Polynomial.initRandomFixed_a_0( degree, modulo, a_0 )
    print ( "Nakasando Polynomial = ", poly2 )

    #Test3
    print("Testing initRandomMinMax (degree, modulo, min, max) ")
    degree = 3
    modulo = "0"
    min = "0"
    max = "100"
    poly3 = Nakasando.Polynomial.initRandomMinMax( degree, modulo, min, max )
    print( "Nakasando Polynomial = ", poly3 )

    #Test4 evaluate function
    print("Testing evaluate function")
    x = "2"
    print ("value for x = %s is %s" % ( x, poly1(x) ) )

    #Test5 creating from list []
    print ("Testing create from list [1, 2, 3]")
    poly4 = ( Nakasando.Polynomial.initFromList( ["1", "2", "3"] ) )
    print( "Nakasando Polynomial = ", poly4 )
    #and evaluate for x=1
    x = "1"
    print ("value for x = %s is %s" % ( x, poly4(x) ) )

    #Test6 creating from list [] with modulo
    print ("Testing create from list [6, 7, 8], mod=5")
    modulo = "5"
    poly5 = ( Nakasando.Polynomial.initFromListModulo( ["6", "7", "8"], modulo ) )
    print( "Nakasando Polynomial = ", poly5 )
    #and evaluate for x=1
    x = "7"
    print ("value for x = %s is %s" % ( x, poly5(x) ) )