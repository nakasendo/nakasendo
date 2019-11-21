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
import binascii
import TransactionHandling

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
    
def HashMsg(msg,modulo=None, IsDec=False):
    HASHMSG = Nakasendo.hash256(msg)
    if(IsDec):
        Hm = Nakasendo.BigNum(str(int(HASHMSG.value,16)), modulo.value, isDec=True)
    else:
        Hm = Nakasendo.BigNum(HASHMSG.value,modulo.value, isDec=False)
    return Hm
    

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
 
    #tt = random.randint(2, 3)
    #tt = 2
    tt = 1
    #msg = 'The argparse module makes it easy to write user-friendly command-line interfaces. The program defines what arguments it requires, and argparse will figure out how to parse those out of sys.argv. The argparse module also automatically generates help and usage messages and issues errors when users give the program invalid arguments'
    msg = 'I love deadlines. I love the whooshing noise they make as they go by.'
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
    modulo_n.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141'
    #modulo_n.value = '115792089237316195423570985008687907852837564279074904382605163141518161494337'
    #p is the finite fielf
    modulo_p = Nakasendo.BigNum()
    modulo_p.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F'
    
    Hm = HashMsg(msg, modulo_n)
    margin = random.randint(0,15)
    #tt = 2
    #nn = 2 * tt + 1 + margin
    nn = 2 * tt +1
    print ('the degree t %s, the Signing Threshold is %s and group size %s' % (tt,(2*tt +1), nn))
    
    #calculate Key shares & the private key
    Players=[]
    
    start_time = time.time()
    definedPolys = DefinedPolys._get_defined_polynomials_deg_2_3_privKey(modulo_n.value)
    for i in range(nn):
        label = "p" + str(i+1)
        new_player = PyPlayer.Player(label, i+1, False, False)
        #new_player.CreatePolynomial (tt, modulo_n.value,decimal=False) 
        new_player.polynomial = definedPolys[i]
        Players.append(new_player)
    
    #for i in range(nn):
     #   print (Players[i].polynomial)
        
        
    #pick a subgroup of t+1 players for the signature group
    
    subGroupPlayersIndex = []
    while(len(subGroupPlayersIndex) < tt+1):
        index = random.randint(0,len(Players)-1)
        if(index not in subGroupPlayersIndex):
            subGroupPlayersIndex.append(index)
            
    
    subGroupSigningIndex = []
    while(len(subGroupSigningIndex)) < (2 * tt + 1):
        index = random.randint(0,len(Players)-1)
        if(index not in subGroupSigningIndex):
            subGroupSigningIndex.append(index)
    #print('Players SubGroup')
    
    vect_d = CalculateSecret(Players, modulo_n.value,isDec=False)
    print(vect_d)
    secret = DerivePubKey(Players,modulo_n.value,isDec=False)
    
    
    GEN = Nakasendo.ECPoint(isDec=False)
    GENPOINT = GEN.GetGeneratorPoint()
    
    HiddenPoint = GENPOINT.multipleScalar(secret)
    PubKeyPoints = HiddenPoint.GetAffineCoOrdinates()
    print('Public Key %s %s' % (PubKeyPoints[0], PubKeyPoints[1]))
    print('public key in compressed form %s' % HiddenPoint)
    print ('private key %s' % secret.value )
     #can we recover the key from a subgroup with interpolation
        
    #Recover the public key from a t+1 group. 
    print('Recover the public/private key pair from a group size of t+1 = %s'% (tt +1))
    private_d_points=[]
    for i in subGroupPlayersIndex:
        label_j = Players[i].getOrdinal()
        d_j = vect_d[i]
        private_d_points.append((int(label_j), d_j.value))
    
    private_d_interpolator = Nakasendo.LGInterpolator(private_d_points, modulo_n.value,decimal=False)
    vect_DG=[] ## Precalculate for each player j the interpolated value   L_j(0) * k_j *G. The sum of this will give k*G
    
    for i in range(len(private_d_points)):
        interpVal = private_d_interpolator('0',str(i))
        djBN=Nakasendo.BigNum(private_d_points[i][1],modulo_n,isDec=False)
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
    
    print('Orginal public key = %s \nRecovered public key %s'%(HiddenPoint,keySum))
    
    pauseVal=input()
    
    print('Derviving & validating key shares')
    
    #print ("Create a blockchain address from the recovered public key")
    #CreateBCAddr(keySum)
    

    #Pick a new polynomial of degree t k
    #pick a new polynomial of degree t for alpha
    
    definedPolys = DefinedPolys._get_defined_polynomials_deg_2_3_k(modulo_n.value)
    for i in range (len(Players)):
        #Players[i].CreatePolynomial(tt,modulo_n.value,decimal=False)
        Players[i].polynomial = definedPolys[i]
    
    vect_k = CalculateSecret(Players, modulo_n.value, isDec=False)
 
    print(vect_k)
    #calculate s signature before hiding k
     
    k = DerivePubKey(Players,modulo_n.value,isDec=False)
    inv_k = k.inverse()

    R = GENPOINT.multipleScalar(inv_k)
    CalcSig = GENPOINT.multipleScalar(k)
    CalcSigPoints = CalcSig.GetAffineCoOrdinates()
    sig_r = CalcSigPoints[0]
    print ('calculated signature from k (r,s) = (%s ,%s)' % (CalcSigPoints[0],CalcSigPoints[1]))
    #Take a subset of players with a 
    definedPolys = DefinedPolys._get_defined_polynomials_deg_2_3_alpha(modulo_n.value)
    for i in range(len(Players)):
        #Players[i].CreatePolynomial(tt,modulo_n.value, decimal=False)
        Players[i].polynomial = definedPolys[i]
        
        
    vect_alpha = CalculateSecret(Players, modulo_n.value, isDec=False)
    
    
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
        xfx_w.append((str(label_j),w_j_points[0],w_j_points[1]))
        
    v_interpolator = Nakasendo.LGInterpolator(xfx_v, modulo_n.value, decimal=False)
    vZeroVal = v_interpolator('0')
    
    
    w_interpolator = Nakasendo.LGECInterpolator(xfx_w, modulo_n.value,decimal=False)

    
    
    wZeroVal = w_interpolator('0')
    
    
    vZeroValInv = vZeroVal.inverse()
    
    interpolated_r = wZeroVal.multipleScalar(vZeroValInv)
    if(interpolated_r.IsPointOnCurve() is not True):
        print("point not on curve")
        exit(1)

    
    interpolated_r_points = interpolated_r.GetAffineCoOrdinates()
    
    
    #print('interpolated_r (%s,%s)' % ( interpolated_r_points[0],interpolated_r_points[1]))
    
    
    RPOINTS = R.GetAffineCoOrdinates()
    assert(RPOINTS[0] == interpolated_r_points[0])
    assert(RPOINTS[1] == interpolated_r_points[1])

    #Standard ECDSA requires a hash
    #but for the demo we removed the hash
   
    userTXInput=False
    print ('do you wish to enter TX information')
    userInput = input()
    if (userInput == 'y'):
        userTXInput=True
        print('Please enter funding TX')
        newMsg = None   
        newMsg = sys.stdin.readlines()
        if(len(newMsg) > 0):
            prevtxid, prevvalue, prevScriptKeyHash = TransactionHandling.ParseInfoFromTX(newMsg)
            '''
            print('NewMessage is %s' % newMsg)
            msg = newMsg
            Hm = Nakasendo.BigNum(msg,modulo_n.value,isDec=False)
            #Hm=HashMsg(msg, modulo_n)
            '''
            
        print('Please enter unsigned TX')
        newMsg = None   
        newMsg = sys.stdin.readlines()
        if(len(newMsg) > 0):
            txid, value, scriptKeyHash = TransactionHandling.ParseInfoFromTX(newMsg)
            
            transaction = TransactionHandling.CreateTransaction(prevtxid,prevScriptKeyHash,prevvalue,value,scriptKeyHash,HiddenPoint,prevIndex=0,sequenceNumber=4294967295)
        #print(' transaction is: %s ' % transaction)

        hashedT = hashlib.sha256(transaction).hexdigest()
        Hm=Nakasendo.BigNum(hashedT,modulo_n.value,isDec=False)
    else:
        Hm = Nakasendo.BigNum(msg,modulo_n.value,isDec=False)
        Hm=HashMsg(msg, modulo_n)
    msg = Hm
    
    vect_s=[]
    interpR = Nakasendo.BigNum(interpolated_r_points[0],modulo_n.value,isDec=False) 
    for j in range(len(vect_k)):
        vect_inv_k_j=vect_k[j]
        d_j = vect_d[j]
        #s = littleK * (Hm + (pks * r_bn))
        val_j = vect_inv_k_j * (Hm + (d_j * interpR))
        print ('%s * (%s + (%s * %s)) = %s' % (vect_inv_k_j, Hm, d_j, interpR,val_j ))
        vect_s.append(val_j)
    
    #print(vect_s)
    s_points = []
    
    #for index_j in range (len(Players)):
    for index_j in subGroupSigningIndex:
        label_j = Players[index_j].getOrdinal()
        s_j    = vect_s[index_j]
        s_points.append((int(label_j) , s_j.value))
        
    print(s_points)
    s_interpolator = Nakasendo.LGInterpolator(s_points, modulo_n.value,decimal=False)
    interpolated_s = s_interpolator ('0')
    
    
    #DER FORMAT (move this into the Nakasendo
    TWO = Nakasendo.BigNum('2',modulo_n,isDec=False)
    modDivByTwo = modulo_n / TWO  
    canonizedInteropolated_s = interpolated_s
    
    if ( interpolated_s > modDivByTwo):
        canonizedInteropolated_s = modulo_n - interpolated_s
        
    DerFormatSig = Nakasendo.createDERFormat(interpR,canonizedInteropolated_s)
    #print('Signature -> %s' % DerFormatSig)
    

    if(userTXInput):
        #41 is hex for 65 and is the sighash flag for ALL and FORKID
        #21 is the length of the compressed public key.
        #These values are hard coded for testing purpose.
        SigBytes = bytes.fromhex(DerFormatSig.value)
        HexSigBytes = binascii.hexlify(SigBytes)
        signature_length = len(SigBytes) + 1 # 1 byte will be added later for sighash flag
        signature_length = signature_length.to_bytes((signature_length.bit_length()+7)//8,'big')
        scriptSig_final = binascii.hexlify(signature_length) + HexSigBytes + b'4121' + binascii.hexlify(binascii.unhexlify(HiddenPoint.value))
        
        
        signedTX = TransactionHandling.SerialiseFinalTx(prevtxid,scriptSig_final,value,scriptKeyHash)
        print("\nThe final transaction to be submitted: {}". format(signedTX))
    
    
    
    
    #Verification via ECDSA library
    SIG_R = Nakasendo.BigNum(sig_r, modulo_n.value, isDec=False)
    _sig_s = inv_k * (Hm + secret*SIG_R)
    print('recalc   sig_s = {}'.format(_sig_s.value))
    print('interpolated_s = {}'.format(interpolated_s.value))
    elapsed_time = time.time() - start_time
    
    #calculate r = inverse of (interpolated vector v) * (interpolated vector w)
    #hold a pair of (r & vect_k)
    
    
    
    #
    ecdsaPubKey = int(secret.value,16)*G
    print ('Public Key for ecdsa verification %s' %ecdsaPubKey)
    pubkeytest = ecdsa.ecdsa.Public_key(G, ecdsaPubKey) 
    privkeytest = ecdsa.ecdsa.Private_key(pubkeytest,int(secret.value,16))

    #print ('private key %s' % secret.value )

    ecdsa_sig = privkeytest.sign(int(Hm.value,16),int(inv_k.value,16))
    
    signature = ecdsa.util.sigencode_der_canonize(ecdsa_sig.r, ecdsa_sig.s,int(modulo_n.value,16))
   
    print('\n\nTest signature with native Python ecdsa module ---- ')
    print('Signing message = [{}]'.format(msg))
    print('interpolated_s = {}'.format(interpolated_s.value))
    print('ecdsa_sig_s    = {}'.format(hex(ecdsa_sig.s)))
    print('interpolated_r = {}'.format(interpolated_r_points[0]))
    print('ecdsa_sig_r    = {}'.format(hex(ecdsa_sig.r)))
    print ('calculation time for a (%s,%s) scheme is %s' % ((tt, len(Players), elapsed_time)))
    assert(int(interpolated_s.value,16) == ecdsa_sig.s)
    assert (int(interpolated_r_points[0],16) == ecdsa_sig.r)
    
    
    print ("verify with ECDSA" )
    if (pubkeytest.verifies (int(Hm.value,16),ecdsa_sig)):
        print ("SUCCESS WITH ECDSA")
    else:
        print ("ERROR WITH ECDSA")
    
    msgBollocks ='A fake message'
    hmB = HashMsg(msgBollocks,modulo_n,IsDec=False)
    
    thresHoldSig = ecdsa.ecdsa.Signature(int(interpolated_r_points[0],16),int(interpolated_s.value,16))
    if(pubkeytest.verifies(int(Hm.value,16), thresHoldSig)):
        print ("SUCESS WITH TS")
    else:
        print("FAILURE WITH TS")
        
        
    if(pubkeytest.verifies(int(hmB.value,16), thresHoldSig)):
        print ("THIS IS BAD...VERY....VERY....BAD -> SUCESS WITH Bollocks message TS")

    

