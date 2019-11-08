import os
import sys
import json
import string
import pathlib
import random
import ecdsa

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
    import Nakasendo
except ImportError as e:
    print('Error while loading SDKLibraries python modules {}'.format(e.message))
    print('Try to define environment variable SDKLIBRARIES_ROOT pointing to the location of installed SDKLibraries or add this to PYTHONPATH')
    raise ImportError('Unable to load SDKLibraries python modules')


if __name__ == "__main__":
    print ("starting.....")
    
    ECPointA  = Nakasendo.ECPoint ()
    print ("ECPointA output %s" % ECPointA ) 

    ECPointB = Nakasendo.ECPoint ()
    print ("ECPointB output %s " % ECPointB)


    coordsA = ECPointA.GetAffineCoOrdinates()
    print ("Random Elliptic Curve Point A:\n x = %s \ny= %s" % (coordsA[0], coordsA[1]))
    
    coordsB = ECPointB.GetAffineCoOrdinates()
    print ("Random Elliptic Curve Point B:\n x = %s \ny= %s" % (coordsB[0], coordsB[1]))
    
    
    bigNumA = Nakasendo.BigNum()
    bigNumB = Nakasendo.BigNum(ECPointA.value)
    print (bigNumB)
    
    GENPOINTA = ECPointA.GetGeneratorPoint()
    GENPOINTB = ECPointB.GetGeneratorPoint()
    print (GENPOINTA)
    print (GENPOINTB)
    
    
   # print ("Random Elliptic Curve Point GEN \n x = %s \ny= %s" % (coordsGen[0], coordsGen[1]))
    
    G = ecdsa.SECP256k1.generator
    print (G)
    
    
    #create a list of ECPoints
    ECPoints = []
    for i in range (1,5):
        pt = Nakasendo.ECPoint()
        coords = pt.GetAffineCoOrdinates()
        ECPoints.append((str(i),coords[0],coords[1] ))
    
    #create interpolator
    modulo='FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141'
    print (ECPoints)
    lgECInterpolator = Nakasendo.LGECInterpolator( ECPoints, modulo,False ) 
    print ("lgInterpolator = ", lgECInterpolator )

    xValue = 0;
    valueECPoint = lgECInterpolator( str(xValue)) 
    print ("Full LG EC Interpolation evaluation for xValue=%s is %s" % (xValue, valueECPoint.GetAffineCoOrdinates() ) )
   
    xValue = 1;
    valueECPoint = lgECInterpolator( str(xValue)) 
    print ("Full LG EC Interpolation evaluation for xValue=%s is %s" % (xValue, valueECPoint.GetAffineCoOrdinates() ) )
 
 
    print ("Hiding the BigNumber %s by multiplying it by the CurveGenerator Point %s" % ( bigNumA, GENPOINTA.GetAffineCoOrdinates()))
    
    hiddenVal = Nakasendo.MultiplyByGenerator(bigNumA)
    print(hiddenVal)
    print(len(hiddenVal.value))
    hiddenVal2 = Nakasendo.MultiplyByGenerator(bigNumA)
    print(hiddenVal)
    
    print(hiddenVal.GetAffineCoOrdinates())
    
    xValue = 1;
    value1 = lgECInterpolator( str(xValue)) 
    print ("Full LG Interpolation evaluation for xValue=%s is %s" % (xValue, value1.GetAffineCoOrdinates() ) )
    
    
    
    isDecimal = False
    if(isDecimal):
        modulo = str(ecdsa.SECP256k1.order)
    degree = random.randint(20, 100)
    #degree = 3
    # create a poly..
    poly1 = None
    if(isDecimal):
        print(modulo)
        poly1 = Nakasendo.Polynomial.initRandomDec( degree, modulo )
    else:
        poly1 = Nakasendo.Polynomial.initRandomHex( degree, modulo )
    print(poly1)
    # evalulate for a few points 
    ordlist = [1,2,3,4,5]
    evalRes = []
    for val in ordlist:
        evalRes.append(poly1(str(val)))
        
    print(' '.join(evalRes))
    # encryt the poly
    
    
    encryptedCoeffs = []
    for coeff in poly1.coefficients:
        bigNumb = Nakasendo.BigNum(coeff,modulo,isDecimal)
        val = Nakasendo.MultiplyByGenerator(bigNumb,isDecimal)
        encryptedCoeffs.append(val)
        
    

    for label in ordlist:
        bignumLabel = Nakasendo.BigNum(str(label),modulo, isDecimal)
        movingLabel = Nakasendo.BigNum(str(label),modulo, isDecimal)
        sumpoints = encryptedCoeffs[0]
        for points in encryptedCoeffs[1:]:
            labelTimesPoints = points.multipleScalar(movingLabel)
            sumpoints = sumpoints + labelTimesPoints
            movingLabel = movingLabel * bignumLabel
        
        bigNumb = Nakasendo.BigNum(evalRes[label-1],modulo,isDecimal)
        val = Nakasendo.MultiplyByGenerator(bigNumb,isDecimal,compressed=True)
        
        assert(sumpoints == val)
      
  
    
    
    
   
    
    
    
    
    
    
