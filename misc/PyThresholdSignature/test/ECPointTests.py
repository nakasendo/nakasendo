import time
import random
import sys
import PyPlayer
import ecdsa
import random

G= ecdsa.SECP256k1.generator
N = ecdsa.SECP256k1.order
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
    
    a = Nakasendo.ECPoint(isDec=False)
    print(a)
    b = Nakasendo.ECPoint(isDec=True)
    print(b)
    
    print ("Getting affine co-ordinates")
    pointsHex = a.GetAffineCoOrdinates()
    print(pointsHex[0],pointsHex[1])
    
    if(a.IsPointOnCurve()):
        print ("Hex Point on Curve")
    
    if(b.IsPointOnCurve()):
        print ("Dec Point on Curve")
    
    print ("calling affine co-ordinates with a decimal")
    pointsDec = b.GetAffineCoOrdinates()
    print(pointsDec[0],pointsDec[1])
    
    GEN = Nakasendo.ECPoint(isDec=True)
    GENPOINT = GEN.GetGeneratorPoint()
    GENPoints = GENPOINT.GetAffineCoOrdinates()
    print(GENPoints[0],GENPoints[1])
    
    modulo_n = Nakasendo.BigNum()
    modulo_n.isDec = True
    modulo_n.value = '115792089237316195423570985008687907852837564279074904382605163141518161494337'
    modulo_n.mod = '115792089237316195423570985008687907852837564279074904382605163141518161494337'
    
    #Create a list of xfx_w
    xfx_w = [(1, '63769552406511992431712156604471311609720103846568080072229729509423318722284','28282562206110063725926628130895230151789231303128345638410183475995030054406'),  (2,'36348836344355853951312865323625532021241780691448892375636305971240290022300', '50555871815444722014312144140186208565955273796939602931700783920118510710802'), (3, '48814341334724761476596055827499995595025346680288991715774406612067669398038', '24307831139400252435984639589681688450211389650318442975295758693557978365915'), (4, '45939073219703336369836690973151529243135253920737874572887943235374197979023', '65649629963468964221035724183685586276155952448981560703180238408873784412510'), (5, '39712221075893920613707040560890324102760528882868566364227099016279876651127', '351074729200096918752738019766857359495541978947195568799359699009320710566')]
    
    for i in range(len(xfx_w)):
        print(xfx_w[i])
    
    w_interpolator = Nakasendo.LGECInterpolator(xfx_w, modulo_n.value,decimal=True)
    
    wVal = w_interpolator(str(xfx_w[0][0]))
    wvalPoints = wVal.GetAffineCoOrdinates()
    print('points are %s %s' % (wvalPoints[0], wvalPoints[1]))
    assert(wvalPoints[0] == xfx_w[0][1])
    assert(wvalPoints[1] == xfx_w[0][2])
    
    for i in range(len(xfx_w)):
        wVal = w_interpolator(str(xfx_w[i][0]))
        wValPoints = wVal.GetAffineCoOrdinates()
        assert(wValPoints[0] == xfx_w[i][1])
        assert(wValPoints[1] == xfx_w[i][2])
        xval = Nakasendo.BigNum(wValPoints[0],isDec=True)
        yval = Nakasendo.BigNum(wValPoints[1],isDec=True)
        if(wVal.IsPointOnCurve()):
            print('Interpolated point on the curve %s' % wVal)
    
    wValAtZero = w_interpolator('0')
    if(wValAtZero.IsPointOnCurve()):
        print('Interpolated zero point is on the curve %s', wValAtZero)
        
        
    
    TWO = Nakasendo.BigNum('2',modulo_n,isDec=True)
    modDivByTwo = modulo_n / TWO  
    
    
    randomNumber = Nakasendo.BigNum()
    randomNumber.mod =modulo_n.value
    HEXTWO = Nakasendo.BigNum('2',modulo_n,isDec=False)
    
    modDivByTwoHex = randomNumber / HEXTWO
    print(modDivByTwoHex)
    print(randomNumber)
    
    
    
    #print(modDivByTwo)
        
          
 
    
