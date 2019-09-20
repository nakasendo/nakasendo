#¡/usr/bin/python



import time
import random
import sys
import getopt
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


def _get_defined_polynomials_deg_2_KTest(deg,modulo=None):
    polys = []
    poly1 = Nakasendo.Polynomial.initFromListModuloDec(['12947668528073543477308455109047462837072825858004012787241413393576035430745','55013223815770071147182329690762295770874013664780832787304292520359528476749','36019556482187218768965434630628212179822160817484260317191251481650930215005'],modulo)
    poly1.modulo = modulo
    polys.append(poly1)
    
    poly2 = Nakasendo.Polynomial.initFromListModuloDec(['30903677663702179748782267301606104159314022276587189700459304755073831098422','18995632419395261390699417660969743835622025373416973451754569295566410502007','50584150054476862072263347423818337225860918194856251265656197083076450252543'],modulo)
    poly2.modulo = modulo
    polys.append(poly2)
    
    poly3 = Nakasendo.Polynomial.initFromListModuloDec(['57716661734930940867996799593209204956083655977702937511427741063453826550930','43239301989509682802872047351774509775209807269359467217506485249141033704033','7861283138100517963911853786979773533312229790256709863328507186047832354808'],modulo)
    poly3.modulo = modulo
    polys.append(poly3)
    
    poly4 = Nakasendo.Polynomial.initFromListModuloDec(['35588194584818905752385345554075910379395729679904458183415350066675755602450','35693820587537279605003617581931288920516359152014244085334998976111720389782','115445388755280524848635834728309896353225808479121121042735198940761765465997'],modulo)
    poly4.modulo = modulo
    polys.append(poly4)
    
    poly5 = Nakasendo.Polynomial.initFromListModuloDec(['86928495085214256352229967517607872799504964396987092257069638198910769691080','110033267465270603464679701829784065549557631446229591390287554773927976007276','6556764205882583089394582200590164199382254698509076809176356070924868794070'],modulo)
    poly5.modulo = modulo
    polys.append(poly5)
    return polys
    
     
def _get_defined_polynomials_deg_2_EmpShareTest(deg,modulo=None):
    polys = []
    poly1 = Nakasendo.Polynomial.initFromListModuloDec(['24994218217093021844030563384394522342720574073032916335966628161802360891480','56923093566603907318069653594217543824215036840079028401560050980295471318454','26952071710174344418909339861165338044894613729183980669058721564552874069432'],modulo)
    poly1.modulo = modulo
    polys.append(poly1)
    
    poly2 = Nakasendo.Polynomial.initFromListModuloDec(['66742135906605113934406343148149031465344915379300741066825127064551833080090','88296953174360890944524534201258432664605628977257063653000349035570319044477','88987006406833879435126295369190642981041710812517955545390669917674853125981'],modulo)
    poly2.modulo = modulo
    polys.append(poly2)
    
    poly3 = Nakasendo.Polynomial.initFromListModuloDec(['65360924682439059750888104826226422760755880569804270629864492429500666345356','94347091288714181909366278105463642893877559090898116798486010255421734688851','97489734322929842511529613438892876692179790783470851989380373275049740958064'],modulo)
    poly3.modulo = modulo
    polys.append(poly3)
    
    poly4 = Nakasendo.Polynomial.initFromListModuloDec(['98009817394525134006196280576696818756475401162328090889431880792440398606149','50449179156879692099229040019944101394287268579099253355693766148518708450884','48486664757013888279131171635058289944142801471390797945715537409045710554366'],modulo)
    poly4.modulo = modulo
    polys.append(poly4)
    
    poly5 = Nakasendo.Polynomial.initFromListModuloDec(['46565140997137221469618068573671808865555934755317765562877035921538247087734','34943456099632866914915303152943250773956118395120797550864874535214690116166','100894379241904715806219407443737997564835803583390447620798873768493047483883'],modulo)
    poly5.modulo = modulo
    polys.append(poly5)
    return polys
    
    
def _get_defined_polynomials_deg_2(deg, modulo=None):
    polys=[]
    poly1 = Nakasendo.Polynomial.initFromListModuloDec(['90486684032348777179857263718215968226709745781802876931942348748667124939196','111060844526219985844993439427010620819313558385116073114983958679266641376958','110718613858017256202536316680113603298106530379002848443141873670797350555901'],modulo)
    poly1.modulo = modulo
    polys.append(poly1)
    
    poly2 = Nakasendo.Polynomial.initFromListModuloDec(['48258538045028570548826074391325925068362928894625813479710236795005509115956','41139461839016974753097870948508408313493639679101849774114275992558422165502','105505238895838311044030323377889983620964588122952954537617794251727518931914'],modulo)
    poly2.modulo = modulo
    polys.append(poly2)
    
    poly3 = Nakasendo.Polynomial.initFromListModuloDec(['110535172131249658185500939112705858058708998630172814074009995699429063879869','108061290097758192663457950485994198362249979585265981699537314760614632599310','54315527299618878493913297670743197559397298559111552462118997327948940095323'],modulo)
    poly3.modulo = modulo
    polys.append(poly3)
    
    poly4 = Nakasendo.Polynomial.initFromListModuloDec(['63710352881479095873539242469459354680667477233584651003526902199982051842677','81231863466428109795760520141443241880170664405340760623513426666830192789100','59219809793501109378576304501351695549242394470537788471337416562923475909016'],modulo)
    poly4.modulo = modulo
    polys.append(poly4)
    
    poly5 = Nakasendo.Polynomial.initFromListModuloDec(['16521042744413024593243591929042350193025729042224752433240176429585864012128','50710796840315552985242326799140829689315074937842458178159955670052904488263','64761406637564736315367730746559201070992423444837082376880430448351029458749'],modulo)
    poly5.modulo = modulo
    polys.append(poly5)
    return polys
    
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
    
    
def CalculateSignatureShares(Hm, keyshares, empKeys, interpolated_r):
    signatures = []
    print(interpolated_r)
    for i in range(len(keyshares)):
        val1 = keyshares[i] * interpolated_r
        val2 = Hm + val1
        sig = val2 * empKeys[i]
        signatures.append(sig)
        
    return signatures
 
def DerivePubKey(Players, modulo=None,isDec=False):
    #Multiply the zero element by G and add them up
    #secretKeys = []
    secret = Nakasendo.BigNum(Players[0].polynomial.coefficients[0],modulo,isDec)
    for i in range(1,len(Players)):
        secret = secret + Nakasendo.BigNum(Players[i].polynomial.coefficients[0],modulo,isDec)
        
        
        
    #hideSecret = Nakasendo.MultiplyByGenerator(secret)
    
    
    return secret


if __name__ == "__main__":
    print ("...starting....")
    
 
    tt = random.randint(2, 40)
    msg = 'hello, I am a string, sign me now'
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
    nn = 2 * tt + 1
    #n is the order
    modulo_n = Nakasendo.BigNum()
    #modulo_n.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141'
    modulo_n.value = '115792089237316195423570985008687907852837564279074904382605163141518161494337'
    #p is the finite fielf
    modulo_p = Nakasendo.BigNum()
    modulo_p.value = 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F'
    
    tt = random.randint(2, 11)
    #margin = random.randint(0,5)
    #tt = 2
    nn = 2 * tt + 1
    print ('the degree t %s and group size %s' % (tt, nn))
    
    #calculate Key shares & the private key
    Players=[]
    
    start_time = time.time()
    definedPolys = _get_defined_polynomials_deg_2(2,modulo_n.value)
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
    #print(subGroupPlayersIndex)
    
    vect_d = CalculateSecret(Players, modulo_n.value,isDec=True)
    #print(vect_d)
    secret = DerivePubKey(Players,modulo_n.value,isDec=True)
    
    
    GEN = Nakasendo.ECPoint(isDec=True)
    GENPOINT = GEN.GetGeneratorPoint()
    
    HiddenPoint = GENPOINT.multipleScalar(secret)
    PubKeyPoints = HiddenPoint.GetAffineCoOrdinates()
    print('Public Key %s %s' % (PubKeyPoints[0], PubKeyPoints[1]))
    
    
    polynomials_k=[]
    polynomials_k = _get_defined_polynomials_deg_2_EmpShareTest(2, modulo_n.value)
    for i in range (nn):
        Players[i].CreatePolynomial(tt,modulo_n.value,decimal=True)
    
    vect_k = CalculateSecret(Players, modulo_n.value, isDec=True)

    
    k = DerivePubKey(Players,modulo_n.value,isDec=True)
    inv_k = k.inverse()

    CalcSig = GENPOINT.multipleScalar(k)
    CalcSigPoints = CalcSig.GetAffineCoOrdinates()
    sig_r = CalcSigPoints[0]
    #print ('calculated sig_r %s sig_s %s' % (CalcSigPoints[0],CalcSigPoints[1]))
    
    #pick the first t+1 players & try to interpolate for the signature
    private_k_points=[]##
    for i in subGroupPlayersIndex:
        label_j = Players[i].getOrdinal()
        k_j = vect_k[i]
        private_k_points.append((int(label_j), k_j.value))
    
    private_k_interpolator = Nakasendo.LGInterpolator(private_k_points, modulo_n.value,decimal=True)
    vect_kG=[] ## Precalculate for each player j the interpolated value   L_j(0) * k_j *G. The sum of this will give k*G
    
    for i in range(len(private_k_points)):
        interpVal = private_k_interpolator('0',str(i))
        kjBN=Nakasendo.BigNum(private_k_points[i][1],modulo_n,isDec=True)
        KInterpVal = interpVal *  kjBN
        vect_kG.append((private_k_points[i][0],KInterpVal))
        
    #print(vect_kG)
    vect_KG = []
    for i in range(len(vect_kG)):
        valG = GENPOINT.multipleScalar(vect_kG[i][1])
        vect_KG.append(valG)
        
    #print(vect_KG)
        
    sigSum = vect_KG[0]
    for vals in vect_KG[1:len(vect_KG)]:
        sigSum = sigSum + vals
    
    interpoloated_Sig = sigSum.GetAffineCoOrdinates()
    assert (interpoloated_Sig == CalcSigPoints)
    #print ('interpolated_Sig_r %s' % interpoloated_Sig[0])

    
    polynomials_q=[]
    for i in range(len(Players)):
        Players[i].CreatePolynomial(tt,modulo_n.value,decimal=True)
        
    
    vect_q = CalculateSecret(Players, modulo_n.value,isDec=True)
    vect_kq = __calc_mul_vector(vect_q,vect_k)
    kq_points = []
    for i in  range(len(Players)):
        label_j = Players[i].getOrdinal()
        kq_j = vect_kq[i]
        kq_points.append((int(label_j),kq_j.value))
    
    kq_interpolator = Nakasendo.LGInterpolator(kq_points, modulo_n.value,decimal=True)
    interpolated_kq = kq_interpolator('0')
    inv_Interpolated_kq = interpolated_kq.inverse()

    
    
    
    #matrix q is the polynomials evaluated
    Polynoms = []
    labels = []
    for i in range(len(Players)):
        Polynoms.append(Players[i].polynomial)
        labels.append(Players[i].getOrdinal())
        
    matrix_q = __calc_matrix(Polynoms, labels)

    matrix_inv_k=[]
    invKG = inv_Interpolated_kq

    for row_q_i in matrix_q:
        row_inv_k_i=[]
        for q_ij in row_q_i:
            q_ijBN = Nakasendo.BigNum(q_ij, modulo_n.value,isDec=True)
            val_ij = q_ijBN * invKG
            row_inv_k_i.append(val_ij.value)
        matrix_inv_k.append(row_inv_k_i)
    
    vect_inv_k = __calc_matrix_sum_col(matrix_inv_k, modulo_n.value,dec=True)

    inv_k_points = []  ## used to interpolate kq

    for index_j in subGroupPlayersIndex:
        label_j = Players[index_j].getOrdinal()
        inv_k_j = vect_inv_k[index_j]
        inv_k_points.append((int(label_j) , inv_k_j.value))
    inv_k_interpolator = Nakasendo.LGInterpolator(inv_k_points, modulo_n.value, decimal=True)
    interpolated_inv_k = inv_k_interpolator ('0')
    #print('recalc       inv_k = {}'.format(inv_k))
    #print('interpolated_inv_k = {}'.format(interpolated_inv_k))
    
    ## calculate s shares s_{j} by calculating s_{j} = k^{-1}_j * (H(msg) + d_j r) ####################################################
    
    HASHMSG = Nakasendo.hash256(msg)
    
    Hm = Nakasendo.BigNum(str(int(HASHMSG.value,16)), modulo_n.value, isDec=True)
    interpolated_r = Nakasendo.BigNum(interpoloated_Sig[0], modulo_n.value, isDec=True)
    #print('Message Hash is %s' % Hm)
    #print('Interpolated_r is %s' %interpolated_r)
 
    vect_s=[]
    for j in range(len(vect_inv_k)):
        vect_inv_k_j=vect_inv_k[j]
        d_j = vect_d[j]
        val_j = vect_inv_k_j * (Hm + (d_j * interpolated_r))
        vect_s.append(val_j)
    
    #print(vect_s)
    s_points = []
    
    for index_j in range (len(Players)):
        label_j = Players[index_j].getOrdinal()
        s_j    = vect_s[index_j]
        s_points.append((int(label_j) , s_j.value))
        
    s_interpolator = Nakasendo.LGInterpolator(s_points, modulo_n.value,decimal=True)
    interpolated_s = s_interpolator ('0')
    elapsed_time = time.time() - start_time
    
    ## standard verification of signature #############################################################################################
    ## rebuild secret shares to test

    SIG_R = Nakasendo.BigNum(sig_r, modulo_n.value, isDec=True)
    _sig_s = inv_k * (Hm + secret*SIG_R)
    #print('recalc   sig_s = {}'.format(_sig_s))
    #print('interpolated_s = {}'.format(interpolated_s))


    
    
    ecdsaPubKey = int(secret.value)*G
    print ('Public Key for ecdsa verification %s' %ecdsaPubKey)
    pubkeytest = ecdsa.ecdsa.Public_key(G, ecdsaPubKey) 
    privkeytest = ecdsa.ecdsa.Private_key(pubkeytest,int(secret.value))

    ecdsa_sig = privkeytest.sign(int(Hm.value),int(k.value))
    #print(ecdsa_sig.r)
    #print(ecdsa_sig.s)
    
    
    print('\n\nTest signature with native Python ecdsa module ---- ')
    print('Signing message = [{}]'.format(msg))
    print('interpolated_s = {}'.format(interpolated_s))
    print('ecdsa_sig_s    = {}'.format(ecdsa_sig.s))
    print('interpolated_r = {}'.format(interpoloated_Sig[0]))
    print('ecdsa_sig_r    = {}'.format(ecdsa_sig.r))
    print ('calculation time for a (%s,%s) scheme is %s' % ((tt, len(Players), elapsed_time)))
    assert(int(interpolated_s.value) == ecdsa_sig.s)
    assert (int(interpoloated_Sig[0]) == ecdsa_sig.r)
    
    print ("verify with ECDSA" )
    if (pubkeytest.verifies (int(Hm.value),ecdsa_sig)):
        print ("SUCCESS WITH ECDSA")
    else:
        print ("ERROR WITH ECDSA")
   
    print ("Verify with threshold")
    
    thresHoldSig = ecdsa.ecdsa.Signature(int(interpoloated_Sig[0]),int(interpolated_s.value))
    if(pubkeytest.verifies(int(Hm.value), thresHoldSig)):
        print ("SUCESS WITH TS")
    else:
        print("FAILURE WITH TS")
    

    

