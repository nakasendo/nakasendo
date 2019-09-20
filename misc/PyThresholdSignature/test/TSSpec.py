#¡/usr/bin/python



import time
import random
import sys
import getopt
import PyPlayer
import ecdsa
import random
#Don't like this...how do you hash a byte stream
import hashlib


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

import DefinedPolys

B58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"
def b58_encode(d):
    out = ""
    p = 0
    x = 0

    while d[0] == 0:
        out += "1"
        d = d[1:]

    for i, v in enumerate(d[::-1]):
        x += v * (256 ** i)

    while x > 58 ** (p + 1):
        p += 1

    while p >= 0:
        a, x = divmod(x, 58 ** p)
        out += B58[a]
        p -= 1

    return out

def ripemd160(x):
    d = hashlib.new("ripemd160")
    d.update(x)
    return d
    
def HashMsg(msg,modulo=None):
    HASHMSG = Nakasendo.hash256(msg)
    Hm = Nakasendo.BigNum(str(int(HASHMSG.value,16)), modulo.value, isDec=True)
    return Hm
    
def Hashfunc(msg,func='ripemd160', modulo=None):
    HASHMSG = Nakasendo.H
def CreateBCAddr(pubkey):
    print('Creating a blockchain address')
    xVal, yVal = pubkey.GetAffineCoOrdinates()
    print(xVal, yVal)
    xValBytes = int(xVal).to_bytes(32,"big")
    yValBytes = int(yVal).to_bytes(32,"big")
    
    bytesInput = b"\x04" + xValBytes + yValBytes
    
    
    hash160 = ripemd160(hashlib.sha256(bytesInput).digest()).digest()
    
    addr = b"\x6f" + hash160
    checksum = hashlib.sha256(hashlib.sha256(addr).digest()).digest()[:4]
    addr += checksum
    addr = b58_encode(addr)
    
    print(addr)
    
    return addr
    
def __calc_shared_secret(polynomials, modulo=None,isDec=False):
    sharedSecret = Nakasendo.BigNum('0',modulo,isDec)
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
def __calc_matrix_sum_col(matrix, modulo=None, dec=False):
    vect_sum_col = []
    for i in range (len(matrix[0])):
        bn = Nakasendo.BigNum('0',modulo,dec)
        bn.isDec = dec
        vect_sum_col.append(bn)

    for row_i in matrix:
        for j in range(len(row_i)):
            x_ij = Nakasendo.BigNum(row_i[j], modulo,isDec=dec)
            vect_sum_col[j] =vect_sum_col[j] + x_ij
    return vect_sum_col

## return vect_c where vect_c_i= vect_a_i * vect_b_i
def __calc_mul_vector(vect_a, vect_b):
    assert (len(vect_a) == len(vect_b))
    vect_c = [k_j * q_j for [k_j, q_j] in zip(vect_a, vect_b)]
    return vect_c


def CalculateSecret(Players, modulo=None,isDec=False):
    #set up the polynomials:
    Polynoms = []
    labels = []
    for i in range(len(Players)):
        Polynoms.append(Players[i].polynomial)
        labels.append(Players[i].getOrdinal())
        
    matrix_d = __calc_matrix(Polynoms, labels)
    
    #print(matrix_d)
    
    shares = []

    #vect_d = __calc_matrix_sum_col(matrix_d, modulo.value)
    vect_d = __calc_matrix_sum_col(matrix_d,modulo,isDec)
    for i in range(len(vect_d)):
        shares.append(vect_d[i])
    
    #sharedSec = __calc_shared_secret(Polynoms,modulo.value)
    return shares
    
 
 
def DerivePubKey(Players, modulo=None,isDec=False):
    #Multiply the zero element by G and add them up
    #secretKeys = []
    secret = Nakasendo.BigNum(Players[0].polynomial.coefficients[0],modulo,isDec)
    for i in range(1,len(Players)):
        secret = secret + Nakasendo.BigNum(Players[i].polynomial.coefficients[0],modulo,isDec)    
    return secret


    
if __name__ == "__main__":
    print ("...starting....")
    
    start_time = time.time()
 
    tt = random.randint(2, 64)
    #tt = 2
    msg = 'The argparse module makes it easy to write user-friendly command-line interfaces. The program defines what arguments it requires, and argparse will figure out how to parse those out of sys.argv. The argparse module also automatically generates help and usage messages and issues errors when users give the program invalid arguments'
    
    '''
    try:
        opts, args = getopt.getopt(sys.argv[1], "t:m:",["threshold=","message="])
    except getopt.GetopError:
        print ('TSSteps.py -t <threshold> -m <MESSAGE>')
        sys.exit(2)
    
    
    
    # define the group size
    for opt,arg in opts:
        if opt in ("-t","--threshold"):
            tt = arg
        elif opt in ("-m","--message"):
            msg = arg
    '''
    
    #n is the order
    modulo_n = Nakasendo.BigNum()
    #modulo_n.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141'
    modulo_n.value = '115792089237316195423570985008687907852837564279074904382605163141518161494337'
    #p is the finite fielf
    modulo_p = Nakasendo.BigNum()
    modulo_p.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F'
    
    Hm = HashMsg(msg, modulo_n)
    #margin = random.randint(0,5)
    #tt = 2
    nn = 2 * tt + 1
    print ('the degree t %s and group size %s' % (tt, nn))
    
    #calculate Key shares & the private key
    Players=[]
    
    start_time = time.time()
    definedPolys = DefinedPolys._get_defined_polynomials_deg_2(2,modulo_n.value)
    for i in range(nn):
        label = "p" + str(i+1)
        new_player = PyPlayer.Player(label, i+1, False, False)
        new_player.CreatePolynomial (tt, modulo_n.value,decimal=True) 
        #new_player.polynomial = definedPolys[i]
        Players.append(new_player)
    
    #for i in range(nn):
     #   print (Players[i].polynomial)
        
        
    #pick a subgroup of t+1 players for the signature group
    
    subGroupPlayersIndex = []
    while(len(subGroupPlayersIndex) < tt+1):
        index = random.randint(0,len(Players)-1)
        if(index not in subGroupPlayersIndex):
            subGroupPlayersIndex.append(index)
            
    
    #print('Players SubGroup')
    
    vect_d = CalculateSecret(Players, modulo_n.value,isDec=True)
    #print(vect_d)
    secret = DerivePubKey(Players,modulo_n.value,isDec=True)
    
    
    GEN = Nakasendo.ECPoint(isDec=True)
    GENPOINT = GEN.GetGeneratorPoint()
    
    HiddenPoint = GENPOINT.multipleScalar(secret)
    PubKeyPoints = HiddenPoint.GetAffineCoOrdinates()
    print('Public Key %s %s' % (PubKeyPoints[0], PubKeyPoints[1]))
    
     #can we recover the key from a subgroup with interpolation
        
    private_d_points=[]
    for i in subGroupPlayersIndex:
        label_j = Players[i].getOrdinal()
        d_j = vect_d[i]
        private_d_points.append((int(label_j), d_j.value))
    
    private_d_interpolator = Nakasendo.LGInterpolator(private_d_points, modulo_n.value,decimal=True)
    vect_DG=[] ## Precalculate for each player j the interpolated value   L_j(0) * k_j *G. The sum of this will give k*G
    
    for i in range(len(private_d_points)):
        interpVal = private_d_interpolator('0',str(i))
        djBN=Nakasendo.BigNum(private_d_points[i][1],modulo_n,isDec=True)
        KInterpVal = interpVal *  djBN
        KGVal = GENPOINT.multipleScalar(KInterpVal)
        vect_DG.append(KGVal)
        
    #print(vect_KG)
        
    keySum = vect_DG[0]
    for vals in vect_DG[1:len(vect_DG)]:
        keySum = keySum + vals


    keySumPoints = keySum.GetAffineCoOrdinates()
    assert(PubKeyPoints[0] == keySumPoints[0])
    assert(PubKeyPoints[1] == keySumPoints[1])
    print ("Create a blockchain address from the recovered public key")
    CreateBCAddr(keySum)
    

    #Pick a new polynomial of degree t k
    #pick a new polynomial of degree t for alpha
    
    for i in range (len(Players)):
        Players[i].CreatePolynomial(tt,modulo_n.value,decimal=True)
    
    vect_k = CalculateSecret(Players, modulo_n.value, isDec=True)
 
    #calculate s signature before hiding k
     
    k = DerivePubKey(Players,modulo_n.value,isDec=True)
    inv_k = k.inverse()

    R = GENPOINT.multipleScalar(inv_k)
    CalcSig = GENPOINT.multipleScalar(k)
    CalcSigPoints = CalcSig.GetAffineCoOrdinates()
    sig_r = CalcSigPoints[0]
    print ('calculated signature from k (r,s) = (%s ,%s)' % (CalcSigPoints[0],CalcSigPoints[1]))
    #Take a subset of players with a 
    for i in range(len(Players)):
        Players[i].CreatePolynomial(tt,modulo_n.value, decimal=True)
        
    vect_alpha = CalculateSecret(Players, modulo_n.value, isDec=True)
    
    
    assert (len(vect_k) == len(vect_alpha))
    
    vect_v = __calc_mul_vector(vect_k, vect_alpha)
    
    vect_w = []
    for val in vect_alpha:
        ecpoint = GENPOINT.multipleScalar(val)
        vect_w.append(ecpoint)
    
    #interpolate vect_v
    xfx_v = []
    xfx_w = []
    for i in range(len(Players)):
    #for i in subGroupPlayersIndex:
        label_j = Players[i].getOrdinal()
        k_j = vect_v[i]
        w_j = vect_w[i]
        w_j_points = w_j.GetAffineCoOrdinates()
        xfx_v.append((int(label_j),k_j.value))
        xfx_w.append((int(label_j),w_j_points[0],w_j_points[1]))
        
    v_interpolator = Nakasendo.LGInterpolator(xfx_v, modulo_n.value, decimal=True)
    vZeroVal = v_interpolator('0')
    
    
    w_interpolator = Nakasendo.LGECInterpolator(xfx_w, modulo_n.value,decimal=True)

    
    
    wZeroVal = w_interpolator('0')
    
    
    vZeroValInv = vZeroVal.inverse()
    
    interpolated_r = wZeroVal.multipleScalar(vZeroValInv)
    if(interpolated_r.IsPointOnCurve() is not True):
        print("point not on curve")
        exit(1)

    
    interpolated_r_points = interpolated_r.GetAffineCoOrdinates()
    
    
    print('interpolated_r (%s,%s)' % ( interpolated_r_points[0],interpolated_r_points[1]))
    
    
    RPOINTS = R.GetAffineCoOrdinates()
    assert(RPOINTS[0] == interpolated_r_points[0])
    assert(RPOINTS[1] == interpolated_r_points[1])


    vect_s=[]
    interpR = Nakasendo.BigNum(interpolated_r_points[0],modulo_n.value,isDec=True) 
    for j in range(len(vect_k)):
        vect_inv_k_j=vect_k[j]
        d_j = vect_d[j]
        val_j = vect_inv_k_j * (Hm + (d_j * interpR))
        vect_s.append(val_j)
    
    #print(vect_s)
    s_points = []
    
    for index_j in range (len(Players)):
    #for index_j in subGroupPlayersIndex:
        label_j = Players[index_j].getOrdinal()
        s_j    = vect_s[index_j]
        s_points.append((int(label_j) , s_j.value))
        
    s_interpolator = Nakasendo.LGInterpolator(s_points, modulo_n.value,decimal=True)
    interpolated_s = s_interpolator ('0')
    
    SIG_R = Nakasendo.BigNum(sig_r, modulo_n.value, isDec=True)
    _sig_s = inv_k * (Hm + secret*SIG_R)
    print('recalc   sig_s = {}'.format(_sig_s.value))
    print('interpolated_s = {}'.format(interpolated_s.value))
    elapsed_time = time.time() - start_time
    
    #calculate r = inverse of (interpolated vector v) * (interpolated vector w)
    #hold a pair of (r & vect_k)
    
    
    
    #
    ecdsaPubKey = int(secret.value)*G
    print ('Public Key for ecdsa verification %s' %ecdsaPubKey)
    pubkeytest = ecdsa.ecdsa.Public_key(G, ecdsaPubKey) 
    privkeytest = ecdsa.ecdsa.Private_key(pubkeytest,int(secret.value))

    
    #ecdsa_sig = privkeytest.sign(int(Hm.value),int(k.value))
    ecdsa_sig = privkeytest.sign(int(Hm.value),int(inv_k.value))
    
    print('\n\nTest signature with native Python ecdsa module ---- ')
    print('Signing message = [{}]'.format(msg))
    print('interpolated_s = {}'.format(interpolated_s))
    print('ecdsa_sig_s    = {}'.format(ecdsa_sig.s))
    print('interpolated_r = {}'.format(interpolated_r_points[0]))
    print('ecdsa_sig_r    = {}'.format(ecdsa_sig.r))
    print ('calculation time for a (%s,%s) scheme is %s' % ((tt, len(Players), elapsed_time)))
    assert(int(interpolated_s.value) == ecdsa_sig.s)
    assert (int(interpolated_r_points[0]) == ecdsa_sig.r)
    
    print ("verify with ECDSA" )
    if (pubkeytest.verifies (int(Hm.value),ecdsa_sig)):
        print ("SUCCESS WITH ECDSA")
    else:
        print ("ERROR WITH ECDSA")
    
    msgBollocks ='A bollocks message'
    hmB = HashMsg(msgBollocks,modulo_n)
    
    thresHoldSig = ecdsa.ecdsa.Signature(int(interpolated_r_points[0]),int(interpolated_s.value))
    if(pubkeytest.verifies(int(Hm.value), thresHoldSig)):
        print ("SUCESS WITH TS")
    else:
        print("FAILURE WITH TS")
        
        
    if(pubkeytest.verifies(int(hmB.value), thresHoldSig)):
        print ("THIS IS BAD...VERY....VERY....BAD -> SUCESS WITH Bollocks message TS")


    

