#!/usr/bin/env python3
import os
import sys
import json
import string
import pathlib
import random

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
    import PyAsymKey
    import PyPolynomial
    import PyBCHAddress
    import Nakasendo
except ImportError as e:
    print('Error while loading SDKLibraries python modules {}'.format(e.message))
    print('Try to define environment variable SDKLIBRARIES_ROOT pointing to the location of installed SDKLibraries or add this to PYTHONPATH')
    raise ImportError('Unable to load SDKLibraries python modules')

if __name__ == "__main__":
    print ("starting.....")
    bigNumA = Nakasendo.BigNum()
    bigNumB = Nakasendo.BigNum()
    
    c = bigNumA + bigNumB
    print ("operator A ->  %s, ..... operator B-> %s" % (bigNumA, bigNumB))
    print("...result of addition %s", c)


    ECPointA  = Nakasendo.ECPoint ()
    print ("ECPointA output %s" % ECPointA ) 

    ECPointB = Nakasendo.ECPoint ()
    print ("ECPointB output %s " % ECPointB)

    ECPointC = ECPointA + ECPointB
    print ("ECPointC (ECPointA + ECPointB) output %s " % ECPointC)


    print ("Testing multiplication (2 parameters)... ")
    BigNumScalerA = Nakasendo.BigNum()
    ECPointG = Nakasendo.ECPoint(); 

    ECPointRes = ECPointG.multipleScalar(BigNumScalerA)
    print("Multiplication res: %s " % ECPointRes)

    print ("Testing multiplication (3 parameters)... ")
    BigNumScalerB = Nakasendo.BigNum()
    ECPointRes1 = ECPointG.multipltScalarEx(BigNumScalerA,BigNumScalerB)
    print ( "3 param Multiplication res: %s " % ECPointRes1)

    ECPointFromRes1 =Nakasendo.ECPoint()
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
    ECPointOnScep112r1_a = Nakasendo.ECPoint(704)
    ECPointOnScep112r1_b = Nakasendo.ECPoint(704)

    print ( "ECPointOnScep112r1_a: %s\nECPointOnScep112r1_b %s\n" % (ECPointOnScep112r1_a, ECPointOnScep112r1_b))
    ECPointOnScep112r1_sum = ECPointOnScep112r1_a + ECPointOnScep112r1_b
    print ("Result of addtion on a curve with ID - 704: %s " % ECPointOnScep112r1_sum)

    print ("Check if its on the curve")
    

    print ("Check Encoding/Decoding/Key creation")
    password='j.murphy@nchain.com'
    encoder = Nakasendo.SymEncDec (password)
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
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        print (encoded)
        decoded = myMsgHash.Bas64Decode(encoded)
        print (decoded)
    msgToEncode = 'Research team'
    for x in range (1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        decoded = myMsgHash.Bas64Decode(encoded)
        print(decoded)

    
    
   
    
    msgToEncode='the quick brown fox jumped over the lazy dog'
    encodedAsHex = encoder.Encode(msgToEncode)
    print ("EncodedMsgAsHex: %s" % encodedAsHex)

    decoded =encoder.Decode(encodedAsHex)
    print ("Decoded Message %s" % decoded )    
    
    mykey = Nakasendo.ECKey256K1();
    print (mykey)
    print ("derive a public key..")
    myderivedKey = mykey.derivePublicKey("Using this message")
    print(myderivedKey)
    shares = mykey.SplitKey(3,6);

    print ('And now recover')
    recoveredKey = Nakasendo.ECKey256K1()
    recoveredKey.RecoverKey(shares)
    print (recoveredKey)
    
    
    
    
    #Sign a msessage
    msg = 'The quick brown fox jumped over the lazy dog'
    sig = mykey.sign(msg);
    print(sig)
    verifyIt = Nakasendo.verify(msg, mykey.pubKey, sig[0], sig[1])
    if (verifyIt == True):
        print ('msg verified')
        
    #generate a shared secret
    
    AliceKey = Nakasendo.ECKey256K1();
    BobsKey = Nakasendo.ECKey256K1();
    
    msgFromBob = BobsKey.CalculateSharedSecret(AliceKey.pubKey); 
    msgFromAlice = AliceKey.CalculateSharedSecret(BobsKey.pubKey);
    
    print(msgFromBob)
    print(msgFromAlice)
    
    # Demo loading creating PEM file with Encryption and passphrase
    passphrase = "A deal is a promise and a promise is unbreakable"
    tempKey = Nakasendo.ECKey256K1()
    print (tempKey)
    encryptedPem = tempKey.ToEncryptedPEMStr( passphrase )
    f = open( "testPem.pem", "w" )
    f.write( encryptedPem )
    f.close()
    
    print("encryptedPem = ", encryptedPem )
    newKey = Nakasendo.ECKey256K1() 

    f = open( "testPem.pem", "r" )
    pemStr = f.read( ) 
    f.close( ) 
    os.remove( "testPem.pem" )

    newKey.FromEncryptedPEMStr( pemStr, passphrase )
    # this will throw:
    #newKey.FromEncryptedPEMStr( pemStr, "This is not a correct passphrase" )

    print ("public key = ", newKey.pubKey )
    print ("private key = ", newKey.priKey )
    
    # generate a derived pub key PEM & convert to HEX (compressed & uncompressed)
    pubkeyPEM = BobsKey.derivePublicKey(msg)
    print ("Derived pub key PEM = ", pubkeyPEM)
    derivedPubKeyHex = Nakasendo.pubKeyPEMasHex(pubkeyPEM)
    print ("Dervied pub key hex uncompressed = ", derivedPubKeyHex)
    derivedPubKeyHex = Nakasendo.pubKeyPEMasHex(pubkeyPEM,True)
    print ("Dervied pub key hex uncompressed = ", derivedPubKeyHex)

    # Test1 randomPolynomial( degree, modulo )
    # Test2 randomPolynomial( degree, modulo, fixed a_0 )
    # Test3 randomPolynomial( degree, modulo, min, max )
    # Test4 evalute for x=2
    # Test5 create polynomial from list


    #Test1 (Dec and Hex variants)
    print("\nTesting initRandomDec (degree, modulo) ")
    degree = 3
    modulo = "17"
    poly1 = Nakasendo.Polynomial.initRandomDec( degree, modulo )
    print ( "Nakasendo Polynomial = ", poly1 )
    
    print("\nTesting initRandomHex (degree, modulo) ")
    modulo = "03"
    poly1 = Nakasendo.Polynomial.initRandomHex( degree, modulo )
    print ( "Nakasendo Polynomial = ", poly1 )    

    #Test2
    print("\nTesting initRandomFixed_a_0_Dec (degree, modulo, fixed a_0) ")
    modulo = "17"
    a_0 = "5"
    poly2 = Nakasendo.Polynomial.initRandomFixed_a_0_Dec( degree, modulo, a_0 )
    print ( "Nakasendo Polynomial = ", poly2 )

    print("\nTesting initRandomFixed_a_0_Hex (degree, modulo, fixed a_0) ")
    modulo = "1D"
    a_0 = "05"
    poly2 = Nakasendo.Polynomial.initRandomFixed_a_0_Hex( degree, modulo, a_0 )
    print ( "Nakasendo Polynomial = ", poly2 )

    #Test3
    print("\nTesting initRandomMinMaxDec (degree, modulo, min, max) ")
    modulo = "0"
    min = "0"
    max = "100"
    poly3 = Nakasendo.Polynomial.initRandomMinMaxDec( degree, modulo, min, max )
    print( "Nakasendo Polynomial = ", poly3 )

    print("\nTesting initRandomMinMaxHex (degree, modulo, min, max) ")
    modulo = "0"
    min = "0"
    max = "64"
    poly3 = Nakasendo.Polynomial.initRandomMinMaxHex( degree, modulo, min, max )
    print( "Nakasendo Polynomial = ", poly3 )    

    #Test4 evaluate function
    print("\nTesting evaluate function")
    x = "02"
    print ("value for x = %s is %s" % ( x, poly1(x) ) )

    #Test5 creating from list []
    print ("\nTesting create from list [1, 2, 3] - Decimal")
    poly4 = ( Nakasendo.Polynomial.initFromListDec( ["1", "2", "3"] ) )
    print( "Nakasendo Polynomial = ", poly4 )
    #and evaluate for x=1
    x = "1"
    print ("value for x = %s is %s" % ( x, poly4(x) ) )

    print ("\nTesting create from list [01, 02, 03] - Hexadecimal")
    poly4 = ( Nakasendo.Polynomial.initFromListHex( ["01", "02", "03"] ) )
    print( "Nakasendo Polynomial = ", poly4 )
    #and evaluate for x=1
    x = "01"
    print ("value for x = = %s is %s" % ( x, poly4(x) ) )    

    #Test6 creating from list [] with modulo
    print ("\nTesting create from list [6, 7, 8], mod=5 - Decimal")
    modulo = "17"
    poly5 = ( Nakasendo.Polynomial.initFromListModuloDec( ["1", "2", "5", "13", "3"], modulo ) )
    print( "Nakasendo Polynomial = ", poly5 )
    #and evaluate for x=1
    x = "2"
    print ("value for x = %s is %s" % ( x, poly5(x) ) )

    print ("\nTesting create from list [06, 07, 08], mod=5 - Hexadecimal")
    modulo = "05"
    poly5 = ( Nakasendo.Polynomial.initFromListModuloHex( ["06", "07", "08"], modulo ) )
    print( "Nakasendo Polynomial = ", poly5 )
    #and evaluate for x=1
    x = "07"
    print ("value for x = %s is %s" % ( x, poly5(x) ) )
    
    #----------------------------------------------------------------
    #----------------------------------------------------------------
    #Test Interpolation
    #----------------------------------------------------------------

    print("\n\nTesting Interpolation, create object with degree=3, mod=17 - decimal version " )
    degree = 3
    modulo = "17"
    polyDecimal = Nakasendo.Polynomial.initRandomDec( degree, modulo )
    print ( "polyDecimal = ", polyDecimal )
    
    
    # using polyDecimal, setup xfx
    margin = 2 
    npPoint = polyDecimal.degree + 1 + margin 
    vectorX = []
    for x in range( npPoint ) :   
        firstNum = 1 
        secondNum = x
        firstNum = firstNum + secondNum
        vectorX.append(firstNum)

    xfx = []
    for x in vectorX :
        xfx.append( (x, polyDecimal(str(x))))
    
    #create interpolator
    lgInterpolator = Nakasendo.LGInterpolator( xfx, polyDecimal.modulo, polyDecimal.isDec ) 
    print ("lgInterpolator = ", lgInterpolator )

    xValue      = random.randint( 0, int(lgInterpolator.modulo) - 1 )
    basisPoint  = random.randint( 0, len(lgInterpolator.points) - 1 )

    print( "xValue = %s, basisPoint = %s" % (xValue, basisPoint))
    value = lgInterpolator( str(xValue)) 
    print ("Full LG Interpolation evaluation for xValue=%s is %s" % (xValue, value ) )
    valBasis = lgInterpolator( str(xValue), str(basisPoint) )
    print ("LG Interpolation evaluation for %sth basis point where xValue=%s is %s" % \
        (basisPoint, xValue, valBasis ) )


    #---------------------------------------------------------------------------
    print("\n\nTesting Interpolation, create object with degree=3, mod=17 - hexadecimal version " )
    degree = 3
    modulo = "11"
    polyHexadecimal = Nakasendo.Polynomial.initRandomHex( degree, modulo )
    print ( "polyHexadecimal = ", polyHexadecimal )  

   # repeat test with Hexadecimal, setup xfx
    margin = 2 
    npPoint = polyHexadecimal.degree + 1 + margin 
    vectorX = []
    for x in range( npPoint ) :   
        firstNum = 1 
        secondNum = x
        firstNum = firstNum + secondNum
        vectorX.append(firstNum)

    xfx = []
    for x in vectorX :
        xfx.append( (x, polyHexadecimal(str(x))))
    
    #create interpolator
    lgInterpolator = Nakasendo.LGInterpolator( xfx, polyHexadecimal.modulo, polyHexadecimal.isDec ) 
    print ("lgInterpolator = ", lgInterpolator )

    xValue      = random.randint( 0, int(lgInterpolator.modulo) - 1 )
    basisPoint  = random.randint( 0, len(lgInterpolator.points) - 1 )

    print( "xValue = %s, basisPoint = %s" % (xValue, basisPoint))
    value = lgInterpolator( str(xValue)) 
    print ("Full LG Interpolation evaluation for xValue=%s is %s" % (xValue, value ) )
    valBasis = lgInterpolator( str(xValue), str(basisPoint) )
    print ("LG Interpolation evaluation for %sth basis point where xValue=%s is %s" % \
        (basisPoint, xValue, valBasis ) )

    #----------------------------------------------------------------
    #----------------------------------------------------------------
    #Test BCHAddress
    #----------------------------------------------------------------
    
    print( "\n----------------------------------------------------------------\n")
    print("1) Write out the public key and corresponding BCH address")
    key = "023cba1f4d12d1ce0bced725373769b2262c6daa97be6a0588cfec8ce1a5f0bd09"
    version = 0
    address = Nakasendo.BCHAddress(key, version )
    
    print( "BCHAddress from key %s is %s\n" % (key, address.address ) )
    
    print( "2) Write out a string representation of the address object")
    print("Address information\n%s" % address )

    print( "3) access the object fields individually")
    print("Valid: %s" % bool( address.valid ) )
    print("Decoded address: %s" % address.decodedAddress )
    print("Network Prefix: %s" % address.prefix )
    print("Network Type: %s" % address.NetworkType)

    print( "\n4) Create BCHAddress from existing address string")
    addressStrInvalid   = "017nDmDt3ZsHqQWAwuc5H8y7cNdZqDyfXAd" 
    addressStrValid     = "17nDmDt3ZsHqQWAwuc5H8y7cNdZqDyfXAd" 
    try:
        Nakasendo.BCHAddress.initFromAddress( addressStrInvalid )
    except Exception as e : 
        print( e )

    address = Nakasendo.BCHAddress.initFromAddress( addressStrValid ) 
    print("Address: %s has valid = %s" % (addressStrValid, bool( address.valid ) ) )
    
