#¡/usr/bin/python



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



def __calc_shared_secret(polynomials, modulo=None):
    sharedSecret = Nakasendo.BigNum('0',modulo)
    for polynomial_i in polynomials:
        hiddenVal = Nakasendo.BigNum(polynomial_i.coefficients[0],modulo)
        sharedSecret = sharedSecret + hiddenVal
    return sharedSecret

## From a list of f[M] and a list of x[N], evaluate them and get the matrix MxN
## Matrix ij = f_i(x_j)
def __calc_matrix(polynomials, x_values):
    matrix=[]
    for f_i in polynomials:
        row_i=[]
        for x_j in x_values:
            row_i.append( f_i(str(x_j)) )
        matrix.append(row_i)
    return matrix
    
    
## return the vector sum of columns of a matrix
def __calc_matrix_sum_col(matrix, modulo=None):
    vect_sum_col = []
    for i in range (len(matrix[0])):
        bn = Nakasendo.BigNum('0',modulo)
        vect_sum_col.append(bn)
    
    for row_i in matrix:
        for j in range(len(row_i)):
            x_ij = Nakasendo.BigNum(row_i[j], modulo)
            vect_sum_col[j] = vect_sum_col[j] + x_ij
    return vect_sum_col

## return vect_c where vect_c_i= vect_a_i * vect_b_i
def __calc_mul_vector(vect_a, vect_b):
    assert (len(vect_a) == len(vect_b))
    vect_c = [k_j * q_j for [k_j, q_j] in zip(vect_a, vect_b)]
    return vect_c




def CalculateSecret(Players, modulo=None):
    #set up the polynomials:
    Polynoms = []
    labels = []
    for i in range(len(Players)):
        Polynoms.append(Players[i].polynomial)
        labels.append(Players[i].getOrdinal())
        
    matrix_d = __calc_matrix(Polynoms, labels)
    
    print(matrix_d)
    
    shares = []

    #vect_d = __calc_matrix_sum_col(matrix_d, modulo.value)
    vect_d = __calc_matrix_sum_col(matrix_d,modulo)
    for i in range(len(vect_d)):
        shares.append(vect_d[i])
    
    #sharedSec = __calc_shared_secret(Polynoms,modulo.value)
    return shares

def _get_defined_polynomials_deg_2(deg,modulo=None):
    polys = []
    poly1 = Nakasendo.Polynomial.initFromListDec([90486684032348777179857263718215968226709745781802876931942348748667124939196,111060844526219985844993439427010620819313558385116073114983958679266641376958,110718613858017256202536316680113603298106530379002848443141873670797350555901])
    poly1.modulo = modulo
    polys.append(poly1)
    
    poly2 = Nakasendo.Polynomial.initFromListDec([48258538045028570548826074391325925068362928894625813479710236795005509115956,41139461839016974753097870948508408313493639679101849774114275992558422165502,105505238895838311044030323377889983620964588122952954537617794251727518931914])
    poly2.modulo = modulo
    polys.append(poly2)
    
    poly3 = Nakasendo.Polynomial.initFromListDec([110535172131249658185500939112705858058708998630172814074009995699429063879869,108061290097758192663457950485994198362249979585265981699537314760614632599310,54315527299618878493913297670743197559397298559111552462118997327948940095323])
    poly3.modulo = modulo
    
    polys.append(poly3)
    
    poly4 = Nakasendo.Polynomial.initFromListDec([63710352881479095873539242469459354680667477233584651003526902199982051842677,81231863466428109795760520141443241880170664405340760623513426666830192789100,59219809793501109378576304501351695549242394470537788471337416562923475909016])
    poly4.modulo = modulo
    polys.append(poly4)
    
    poly5 = Nakasendo.Polynomial.initFromListDec([16521042744413024593243591929042350193025729042224752433240176429585864012128,50710796840315552985242326799140829689315074937842458178159955670052904488263,64761406637564736315367730746559201070992423444837082376880430448351029458749])
    poly5.modulo = modulo
    polys.append(poly5)
    return polys
    
def CalculateSignatureShares(Hm, keyshares, empKeys, interpolated_r):
    signatures = []
    print(interpolated_r)
    for i in range(len(keyshares)):
        val1 = keyshares[i] * interpolated_r
        val2 = Hm + val1
        sig = val2 * empKeys[i]
        signatures.append(sig)
        
    return signatures
 
def DerivePubKey(Players):
    #Multiply the zero element by G and add them up
    secretKeys = []
    for i in range(len(Players)):
        secret = Nakasendo.BigNum(Players[i].polynomial.coefficients[0],modulo)
        hideSecret = Nakasendo.MultiplyByGenerator(secret)
        secretKeys.append(hideSecret)
    
    
    pubKey = secretKeys[0]
    for i in range(1,len(Players)):
        pubKey = pubKey + secretKeys[i]
    
    return pubKey


if __name__ == "__main__":
    print ("...starting....")
    nn = 5
    modulo_n = Nakasendo.BigNum()
    modulo_n.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141'
    
    modulo_p = Nakasendo.BigNum()
    modulo_p.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F'
    
    modulo = Nakasendo.BigNum()
    modulo.value = modulo_p.value
    #modulo.value = '17'
    Players=[]
    
    for i in range(nn):
        label = "p" + str(i)
        new_player = PyPlayer.Player(label, i+1, False, False)
        Players.append(new_player)
        
    #set the polynomials
    Players[0].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["1", "2", "3"] ) )
    Players[1].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["5", "6", "7"] ) )
    Players[2].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["9", "8", "7"] ) )
    Players[3].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["4", "5", "6"] ) )
    Players[4].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["7", "8", "9"]) )
        
    for i in range(len(Players)):
        print(Players[i].to_json()) 
        print(Players[i].polynomial)
        
    secrets = CalculateSecret(Players, modulo.value)
    for i in range(len(secrets)):
        print ('%s %s' % (secrets[i].value, secrets[i].mod))
    Polynoms = []
    for i in range(len(Players)):
        Polynoms.append(Players[i].polynomial)
    pubKey = __calc_shared_secret(Polynoms, modulo.value)

    
    P = Nakasendo.MultiplyByGenerator(pubKey)
    points = P.GetAffineCoOrdinates()
    
    print(points[0], points[1])
    if P.IsPointOnCurve():
        print ('shared public key on the curve')
    
    #reset the polynomials & calculate k shares
    Players[0].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["6", "7", "1"] ) )
    Players[1].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["8", "9", "5"]) )
    Players[2].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["2", "4", "6"] ) )
    Players[3].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["1", "6", "8"] ) )
    Players[4].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["7", "2", "1"]) )
    kVals = CalculateSecret(Players,  modulo.value)
    print(kVals)
    #reset the polynomials & calculate alphas
    Players[0].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["7", "5", "4"] ) )
    Players[1].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["6", "8", "2"] ) )
    Players[2].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["4", "9", "6"] ) )
    Players[3].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["9", "1", "1"] ) )
    Players[4].polynomial = ( Nakasendo.Polynomial.initFromListDec( ["5", "4", "9"]) )
    alphas = CalculateSecret(Players,modulo.value)
    print(alphas)
    
    print ('Calculating v = kAlpha')
    #Calculate v shares
    kAlpha = __calc_mul_vector(kVals,alphas)
    #print(kAlpha)
    print(kAlpha)
    #calculate W
    wG = []
    for i in range(len(alphas)):
        value = Nakasendo.MultiplyByGenerator(alphas[i])
        wG.append(value)
    
    #print(wG)    

    #create the labels
    xfx = []
    for i in range(len(Players)):
        xfx.append((Players[i].getOrdinal(),kAlpha[i].value))
    
    #print(xfx)
    #interpolate kAlpha
    
    interp = Nakasendo.LGInterpolator(xfx,modulo=modulo.value,decimal=False)
    valAtZero = interp('0')
    valAtZero.mod = modulo.value
    print('kAlpha interpolated at zero = %s' %valAtZero)

    #should have 1 r value
    
    #invert a negative
    invInterpK = valAtZero.inverse();
    print(invInterpK)
    
    wGInterpInput = []
    for i in range(len(Players)):
        points = wG[i].GetAffineCoOrdinates()
        wGInterpInput.append((Players[i].getOrdinal(),points[0],points[1]))
       
   # print(wGInterpInput)
    wgPointInterp = Nakasendo.LGECInterpolator(wGInterpInput,modulo=modulo.value)
    wgAtZero = wgPointInterp('0')
    print ('wG interpolated at zero = %s' % wgAtZero)
    
    
    #take the x co-ord of wgAtZero
    wgATZeroPoints = wgAtZero.GetAffineCoOrdinates()
    xWAtZero = Nakasendo.BigNum(wgATZeroPoints[0],modulo.value)
    keyVal = xWAtZero * invInterpK
    newRkey = Nakasendo.MultiplyByGenerator(keyVal)
    newRKeyPoints = newRkey.GetAffineCoOrdinates()
    print ('Vals of newRKey %s %s' % (newRKeyPoints[0],newRKeyPoints[1]))
    
    
    rKey = wgAtZero.multipleScalar(invInterpK)
    print('wgAtzero * invInterpK = %s' % rKey)
    pointsK = rKey.GetAffineCoOrdinates()
    
    
    
  #  print (pointsK[0],pointsK[1])
    
    message = 'The quick brown fox jumped over the lazy dog'
    Hm = Nakasendo.hash256(message)
    Hm.mod = modulo.value
    print(Hm)
    rXVal = Nakasendo.BigNum(pointsK[0],modulo.value)
    sign = CalculateSignatureShares(Hm,secrets,kVals,rXVal)
 #   print(sign)
    
    sigList = []
    for i in range(len(Players)):
        sigList.append((Players[i].getOrdinal(),sign[i].value))
    interpSig = Nakasendo.LGInterpolator(sigList,modulo=modulo.value,decimal=False)
    
    S = interpSig('0')
    print(S)
    print (type(S))
    
    privateKeyPoints = P.GetAffineCoOrdinates()
    print('shared public key \n %s,%s' % (privateKeyPoints[0],privateKeyPoints[1]))
    
    
    print ('shared r value \n %s, %s' % (pointsK[0],pointsK[1]))
    print ('shared r value as hex %s' % rKey)
    
    
    #Verify
    print(pubKey.value)
    pubkeyVal = int(pubKey.value,16)
    #print(pubkeyVal)
    ecdsaPubKey = pubkeyVal*G
    print (ecdsaPubKey)
    pubkeytest = ecdsa.ecdsa.Public_key(G, ecdsaPubKey) 
    privkeytest = ecdsa.ecdsa.Private_key(pubkeytest,pubkeyVal)

    signaturetest = privkeytest.sign(int(Hm.value,16),pubkeyVal)
    print(signaturetest.r)
    print(signaturetest.s)
    
    if pubkeytest.verifies( int(Hm.value,16), signaturetest ):
        print ("Demo verification succeeded.")
    else:
        print ("*** Demo verification failed.")
    


