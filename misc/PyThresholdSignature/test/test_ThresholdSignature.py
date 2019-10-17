#!/usr/bin/env python3

import random
import sys
sys.path.append('../')
import FiniteGroup
import Polynomial
import Threshold
import ecdsa
import random
import time

G= ecdsa.SECP256k1.generator
q = ecdsa.SECP256k1.order

def __calc_shared_secret(polynomials, modulo=None):
    shared_secret=0
    for polynomial_i in polynomials:
        shared_secret+=polynomial_i.a[0]
    return FiniteGroup.normalize_mod(shared_secret,modulo) if modulo is not None else shared_secret

## From a list of f[M] and a list of x[N], evaluate them and get the matrix MxN
## Matrix ij = f_i(x_j)
def __calc_matrix(polynomials, x_values):
    matrix=[]
    for f_i in polynomials:
        row_i=[]
        for x_j in x_values:
            row_i.append( f_i(x_j) )
        matrix.append(row_i)
    return matrix

## return the vector sum of columns of a matrix
def __calc_matrix_sum_col(matrix, modulo=None):
    vect_sum_col=[0]* len(matrix[0])
    for row_i in matrix:
        for j in range(len(row_i)):
            x_ij=row_i[j]
            vect_sum_col[j] = FiniteGroup.add_mod(vect_sum_col[j],x_ij,modulo) if modulo is not None else vect_sum_col[j]+x_ij
    return vect_sum_col

## return the vector sum of columns of a matrix
def __calc_sum_vector(v, modulo=None):
    sum=0
    for v_i in v:
        sum+=v_i
    sum = FiniteGroup.normalize_mod(sum,modulo) if modulo is not None else sum
    return sum

## return vect_c where vect_c_i= vect_a_i * vect_b_i
def __calc_mul_vector(vect_a, vect_b, modulo=None):
    assert (len(vect_a) == len(vect_b))
    vect_c = [k_j * q_j for [k_j, q_j] in zip(vect_a, vect_b)]
    if modulo is not None:
        for j in range(len(vect_c)):
            vect_c[j]=FiniteGroup.normalize_mod(vect_c[j],modulo)
    return vect_c

def __print_matrix(M, matrix_label=None):#list of list
    print('{} :'.format(matrix_label if matrix_label is not None else 'Matrix'))
    for row_i in M:
        print(row_i)

def __print_vector(V, vector_label=None):#list
    print('{} : {}'.format(vector_label if vector_label is not None else 'Vector', V))

def __get_random_polynomial(deg,modulo=None):
    #a = [i+1 for i in range(deg)]
    #return Polynomial.Polynomial(a,modulo)
    return Polynomial.RandomPolynomial(deg,modulo)
    
def _get_defined_polynomials_deg_2_KTest(deg,modulo=None):
    polys = []
    poly1 = Polynomial.Polynomial()
    poly1.a = [12947668528073543477308455109047462837072825858004012787241413393576035430745,55013223815770071147182329690762295770874013664780832787304292520359528476749,36019556482187218768965434630628212179822160817484260317191251481650930215005]
    poly1.q = modulo
    polys.append(poly1)
    
    poly2 = Polynomial.Polynomial()
    poly2.a = [30903677663702179748782267301606104159314022276587189700459304755073831098422,18995632419395261390699417660969743835622025373416973451754569295566410502007,50584150054476862072263347423818337225860918194856251265656197083076450252543]
    poly2.q = modulo
    polys.append(poly2)
    
    poly3 = Polynomial.Polynomial()
    poly3.a = [57716661734930940867996799593209204956083655977702937511427741063453826550930,43239301989509682802872047351774509775209807269359467217506485249141033704033,7861283138100517963911853786979773533312229790256709863328507186047832354808]
    poly3.q = modulo
    
    polys.append(poly3)
    
    poly4 = Polynomial.Polynomial()
    poly4.a = [35588194584818905752385345554075910379395729679904458183415350066675755602450,35693820587537279605003617581931288920516359152014244085334998976111720389782,115445388755280524848635834728309896353225808479121121042735198940761765465997]
    poly4.q = modulo
    polys.append(poly4)
    
    poly5 = Polynomial.Polynomial()
    poly5.a = [86928495085214256352229967517607872799504964396987092257069638198910769691080,110033267465270603464679701829784065549557631446229591390287554773927976007276,6556764205882583089394582200590164199382254698509076809176356070924868794070]
    poly5.q = modulo
    polys.append(poly5)
    return polys
    
def _get_defined_polynomials_deg_2(deg,modulo=None):
    polys = []
    poly1 = Polynomial.Polynomial()
    poly1.a = [90486684032348777179857263718215968226709745781802876931942348748667124939196,111060844526219985844993439427010620819313558385116073114983958679266641376958,110718613858017256202536316680113603298106530379002848443141873670797350555901]
    poly1.q = modulo
    polys.append(poly1)
    
    poly2 = Polynomial.Polynomial()
    poly2.a = [48258538045028570548826074391325925068362928894625813479710236795005509115956,41139461839016974753097870948508408313493639679101849774114275992558422165502,105505238895838311044030323377889983620964588122952954537617794251727518931914]
    poly2.q = modulo
    polys.append(poly2)
    
    poly3 = Polynomial.Polynomial()
    poly3.a = [110535172131249658185500939112705858058708998630172814074009995699429063879869,108061290097758192663457950485994198362249979585265981699537314760614632599310,54315527299618878493913297670743197559397298559111552462118997327948940095323]
    poly3.q = modulo
    
    polys.append(poly3)
    
    poly4 = Polynomial.Polynomial()
    poly4.a = [63710352881479095873539242469459354680667477233584651003526902199982051842677,81231863466428109795760520141443241880170664405340760623513426666830192789100,59219809793501109378576304501351695549242394470537788471337416562923475909016]
    poly4.q = modulo
    polys.append(poly4)
    
    poly5 = Polynomial.Polynomial()
    poly5.a = [16521042744413024593243591929042350193025729042224752433240176429585864012128,50710796840315552985242326799140829689315074937842458178159955670052904488263,64761406637564736315367730746559201070992423444837082376880430448351029458749]
    poly5.q = modulo
    polys.append(poly5)
    return polys

def _get_defined_polynomials_deg_2_EmpShareTest(deg,modulo=None):
    polys = []
    poly1 = Polynomial.Polynomial()
    poly1.a = [24994218217093021844030563384394522342720574073032916335966628161802360891480,56923093566603907318069653594217543824215036840079028401560050980295471318454,26952071710174344418909339861165338044894613729183980669058721564552874069432]
    poly1.modulo = modulo
    polys.append(poly1)
    
    poly2 =Polynomial.Polynomial()
    poly2.a = [66742135906605113934406343148149031465344915379300741066825127064551833080090,88296953174360890944524534201258432664605628977257063653000349035570319044477,88987006406833879435126295369190642981041710812517955545390669917674853125981]
    poly2.modulo = modulo
    polys.append(poly2)
    
    poly3 =Polynomial.Polynomial()
    poly3.a = [65360924682439059750888104826226422760755880569804270629864492429500666345356,94347091288714181909366278105463642893877559090898116798486010255421734688851,97489734322929842511529613438892876692179790783470851989380373275049740958064]
    poly3.modulo = modulo
    polys.append(poly3)
    
    poly4 = Polynomial.Polynomial()
    poly4.a = [98009817394525134006196280576696818756475401162328090889431880792440398606149,50449179156879692099229040019944101394287268579099253355693766148518708450884,48486664757013888279131171635058289944142801471390797945715537409045710554366]
    poly4.modulo = modulo
    polys.append(poly4)
    
    poly5 =Polynomial.Polynomial()
    poly5.a = [46565140997137221469618068573671808865555934755317765562877035921538247087734,34943456099632866914915303152943250773956118395120797550864874535214690116166,100894379241904715806219407443737997564835803583390447620798873768493047483883]
    poly5.modulo = modulo
    polys.append(poly5)
    return polys
    
def __test_ThresholdSignature(msg, tt,nn,modulo=None):##
    # s = k^{-1}(H(msg) + dr)
    if 2*tt+1>nn:
        raise RuntimeError('Bad threshold configuration 2t+1>n with t={} n={}'.format(tt,nn))

    start_time = time.time()
    ts = Threshold.Threshold(tt,nn)
    player_indices = ts.indices()
    player_labels  = ts.labels()
    assert (2*ts.t()+1 == ts.n())
    subgroup_2tp1_indices, subgroup_2tp1_labels = ts.pick_subset(2*ts.t() + 1, player_labels)  # subgroup t++
    _ , subgroup_tp1_indices = ts.pick_subset(ts.t()+1, subgroup_2tp1_indices)  # subgroup t++
    subgroup_tp1_labels  = [player_labels[index] for index in subgroup_tp1_indices]
    print('\n---  test_ThresholdSignature -------')
    print(ts)
    print('Subgroup 2tp1 indices {}'.format(subgroup_2tp1_indices))
    print('Subgroup 2tp1 labels  {}'.format(subgroup_2tp1_labels))
    print('Subgroup    t indices {}'.format(subgroup_tp1_indices))
    print('Subgroup    t labels  {}'.format(subgroup_tp1_labels))
    assert(2*(len(subgroup_tp1_indices)-1)+1 == len(subgroup_2tp1_labels)) ## test 2t+1 correctness
    for i in range (len(subgroup_2tp1_labels)): ## test label == index+1
        assert((subgroup_2tp1_indices[i]+1) == subgroup_2tp1_labels[i])
    for i in range (len(subgroup_tp1_labels)):
        assert(subgroup_tp1_indices[i] in subgroup_2tp1_indices) ## test inclusion group_t inside group_2tp1
        assert(subgroup_tp1_labels[i] in subgroup_2tp1_labels)   ## test inclusion group_t inside group_2tp1
        assert((subgroup_tp1_indices[i]+1) == subgroup_tp1_labels[i])  ## test label == index+1

    ### Generate shares d_i for player i ############################################################################################
    polynomials_d=[]
    for i in range (ts.n()):
        polynomials_d.append(__get_random_polynomial(ts.t(),modulo))
    print(polynomials_d)
    matrix_d = __calc_matrix(polynomials_d, player_labels)
    vect_d = __calc_matrix_sum_col(matrix_d, modulo)

    ## Calculate interpolated P=d*G
    #vect_P = [d_i*G for d_i in vect_d]
    interpolated_P = vect_d[0]*G
    _sub_vect_d = vect_d[1:len(vect_d)]
    for i in range(len(_sub_vect_d)):
        interpolated_P = interpolated_P + _sub_vect_d[i]*G
    d = __calc_shared_secret(polynomials_d, modulo)
    P = d * G

    ### Generate shares k_i for player i ############################################################################################
    polynomials_k=[]
    for i in range (ts.n()):
        polynomials_k.append(__get_random_polynomial(ts.t(),modulo))
        
    matrix_k = __calc_matrix(polynomials_k, player_labels)
    vect_k = __calc_matrix_sum_col(matrix_k, modulo)
    k = __calc_shared_secret(polynomials_k, modulo)
    inv_k = FiniteGroup.inv_mod(k, modulo)
    kG = k*G
    kGx = kG.x()
    sig_r  = FiniteGroup.normalize_mod(kGx,modulo)

    ### Calculate interpolated r for everyone using a subgroup of t+1 players #######################################################
    private_k_points=[]##
    for index_j in subgroup_tp1_indices:
        label_j = player_labels[index_j]
        k_j    = vect_k[index_j]
        private_k_points.append([label_j , k_j])
    private_k_interpolator = Polynomial.LagrangeInterpolator(private_k_points, modulo)
    vect_kG=[] ## Precalculate for each player j the interpolated value   L_j(0) * k_j *G. The sum of this will give k*G
    for sub_index_j in range(len(subgroup_tp1_indices)):
        index_j = subgroup_tp1_indices[sub_index_j]
        label_j = player_labels[index_j]
        kG_j = FiniteGroup.normalize_mod((private_k_interpolator[sub_index_j](0) * private_k_points[sub_index_j][1]),modulo) *G
        vect_kG.append([label_j , kG_j])
    interpolated_kG = vect_kG[0]
    for kG_j in vect_kG[1:len(vect_kG)]:
        interpolated_kG = interpolated_kG + kG_j
    interpolated_kGx = kG.x()
    interpolated_r = FiniteGroup.normalize_mod(interpolated_kGx,modulo)
    print('recalc   sig_r = {}'.format(sig_r))
    print('interpolated_r = {}'.format(interpolated_r))

    ### Generate shares k^{-1}_i for player i using auxiliaire q ####################################################################
    polynomials_q=[]
    for i in range (ts.n()):
        polynomials_q.append(__get_random_polynomial(ts.t(),modulo))

    matrix_q = __calc_matrix(polynomials_q, player_labels)
    vect_q = __calc_matrix_sum_col(matrix_q, modulo)
    vect_kq = __calc_mul_vector(vect_q,vect_k, modulo)
    kq_points = [] ## used to interpolate kq
    for index_j in subgroup_2tp1_indices:
        label_j = player_labels[index_j]
        kq_j    = vect_kq[index_j]
        kq_points.append([label_j , kq_j])
    kq_interpolator = Polynomial.LagrangeInterpolator(kq_points, modulo)
    interpolated_kq = kq_interpolator (0)
    inv_interpolated_kq = FiniteGroup.inv_mod(interpolated_kq ,modulo) if modulo is not None else 1/interpolated_kq
    ## calculate inv_k shares k^{-1}_{j} by calculating k^{-1}_{ij} = \frac{1}{kq}q_{ij}, then k^{-1}_{j} = \sum_{i} k^{-1}_{ij}
    matrix_inv_k=[]
    for row_q_i in matrix_q:
        row_inv_k_i=[]
        for q_ij in row_q_i:
            val_ij= FiniteGroup.mul_mod(inv_interpolated_kq,q_ij, modulo) if modulo is not None else inv_interpolated_kq*q_ij
            row_inv_k_i.append(val_ij)
        matrix_inv_k.append(row_inv_k_i)
    vect_inv_k = __calc_matrix_sum_col(matrix_inv_k, modulo)
    inv_k_points = []  ## used to interpolate kq
    for index_j in subgroup_tp1_indices:
        label_j = player_labels[index_j]
        inv_k_j = vect_inv_k[index_j]
        inv_k_points.append([label_j , inv_k_j])
    inv_k_interpolator = Polynomial.LagrangeInterpolator(inv_k_points, modulo)
    interpolated_inv_k = inv_k_interpolator (0)
    print('recalc       inv_k = {}'.format(inv_k))
    print('interpolated_inv_k = {}'.format(interpolated_inv_k))

    ## calculate s shares s_{j} by calculating s_{j} = k^{-1}_j * (H(msg) + d_j r) ####################################################
    Hm = FiniteGroup.hash_mod(msg,modulo)
    print ('The message is %s and the hash is %s' %(msg, Hm))
    vect_s=[]
    for j in range(len(vect_inv_k)):
        vect_inv_k_j=vect_inv_k[j]
        d_j = vect_d[j]
        val_j = vect_inv_k_j * (Hm + d_j*interpolated_r)
        #val_j = interpolated_inv_k * (Hm + d_j * interpolated_r)
        val_j = FiniteGroup.normalize_mod(val_j, modulo) if modulo is not None else val_j
        vect_s.append(val_j)
    s_points=[]
    for index_j in subgroup_2tp1_indices:
        label_j = player_labels[index_j]
        s_j    = vect_s[index_j]
        s_points.append([label_j , s_j])
    s_interpolator = Polynomial.LagrangeInterpolator(s_points, modulo)
    interpolated_s = s_interpolator (0)
    elapsed_time = time.time() - start_time

    ## standard verification of signature #############################################################################################
    ## rebuild secret shares to test
    _sig_s = inv_k * (Hm + d*sig_r)
    sig_s = FiniteGroup.normalize_mod(_sig_s, modulo)
    #print('recalc   sig_s = {}'.format(sig_s))
    #print('interpolated_s = {}'.format(interpolated_s))


    print(type(P))
    pub_key = ecdsa.ecdsa.Public_key(G,P)
    key = ecdsa.ecdsa.Private_key(pub_key,d)
    ecdsa_sig = key.sign(Hm,k)
    ecdsa_sig_s = ecdsa_sig.s
    ecdsa_sig_r = ecdsa_sig.r
    print('\n\nTest signature with native Python ecdsa module ---- ')
    print('Signing message = [{}]'.format(msg))
    print('interpolated_s = {}'.format(interpolated_s))
    print('ecdsa_sig_s    = {}'.format(ecdsa_sig_s))
    print('interpolated_r = {}'.format(interpolated_r))
    print('ecdsa_sig_r    = {}'.format(ecdsa_sig_r))
    print('{} calculation time {} (s)'.format(ts, elapsed_time))
    assert(interpolated_s == ecdsa_sig_s)
    assert (interpolated_r == ecdsa_sig_r)
    
    print ("verify with ECDSA" )
    if (pub_key.verifies (Hm,ecdsa_sig)):
        print ("SUCCESS WITH ECDSA")
    else:
        print ("ERROR WITH ECDSA")
        x
    print ("Verify with threshold")
    
    thresHoldSig = ecdsa.ecdsa.Signature(interpolated_r,interpolated_s)
    if(pub_key.verifies(Hm, thresHoldSig)):
        print ("SUCESS WITH TS")
    else:
        print("FAILURE WITH TS")
    
def testTSSteps(msg, tt,nn,modulo=None):
    # s = k^{-1}(H(msg) + dr)
    if 2*tt+1>nn:
        raise RuntimeError('Bad threshold configuration 2t+1>n with t={} n={}'.format(tt,nn))

    start_time = time.time()
    ts = Threshold.Threshold(tt,nn)
    player_indices = ts.indices()
    player_labels  = ts.labels()
    assert (2*ts.t()+1 == ts.n())
    subgroup_2tp1_indices, subgroup_2tp1_labels = ts.pick_subset(2*ts.t() + 1, player_labels)  # subgroup t++
    _ , subgroup_tp1_indices = ts.pick_subset(ts.t()+1, subgroup_2tp1_indices)  # subgroup t++
    subgroup_tp1_labels  = [player_labels[index] for index in subgroup_tp1_indices]
    print('\n---  test_ThresholdSignature -------')
    print(ts)
    print('Subgroup 2tp1 indices {}'.format(subgroup_2tp1_indices))
    print('Subgroup 2tp1 labels  {}'.format(subgroup_2tp1_labels))
    print('Subgroup    t indices {}'.format(subgroup_tp1_indices))
    print('Subgroup    t labels  {}'.format(subgroup_tp1_labels))
    assert(2*(len(subgroup_tp1_indices)-1)+1 == len(subgroup_2tp1_labels)) ## test 2t+1 correctness
    for i in range (len(subgroup_2tp1_labels)): ## test label == index+1
        assert((subgroup_2tp1_indices[i]+1) == subgroup_2tp1_labels[i])
    for i in range (len(subgroup_tp1_labels)):
        assert(subgroup_tp1_indices[i] in subgroup_2tp1_indices) ## test inclusion group_t inside group_2tp1
        assert(subgroup_tp1_labels[i] in subgroup_2tp1_labels)   ## test inclusion group_t inside group_2tp1
        assert((subgroup_tp1_indices[i]+1) == subgroup_tp1_labels[i])  ## test label == index+1
        
    ## Generate shares d_i for player i ############################################################################################
    polynomials_d=[]
    polynomials_d = _get_defined_polynomials_deg_2(tt,modulo)
    print(polynomials_d)
    matrix_d = __calc_matrix(polynomials_d, player_labels)
    vect_d = __calc_matrix_sum_col(matrix_d, modulo)
    print(vect_d)
    ## Calculate interpolated P=d*G
    #vect_P = [d_i*G for d_i in vect_d]
    interpolated_P = vect_d[0]*G
    _sub_vect_d = vect_d[1:len(vect_d)]
    for i in range(len(_sub_vect_d)):
        interpolated_P = interpolated_P + _sub_vect_d[i]*G
    d = __calc_shared_secret(polynomials_d, modulo)
    P = d * G
    print ('value of G is %s' % G)
    print ('value of d is %s' % d)
    print ('private key in testTSSteps is %s'% P)
    
     ### Generate shares k_i for player i ############################################################################################
    polynomials_k=[]
    polynomials_k = _get_defined_polynomials_deg_2_EmpShareTest(tt, modulo)
    
    
    print(polynomials_k)
    matrix_k = __calc_matrix(polynomials_k, player_labels)
    vect_k = __calc_matrix_sum_col(matrix_k, modulo)
    k = __calc_shared_secret(polynomials_k, modulo)
    print(k)
    inv_k = FiniteGroup.inv_mod(k, modulo)
    print(inv_k)
    kG = k*G
    kGx = kG.x()
    kGy = kG.y()
    sig_r  = FiniteGroup.normalize_mod(kGx,modulo)
    sig_s = FiniteGroup.normalize_mod(kGy,modulo)
    print ('calculated sig_r %s sig_s %s' % (sig_r, sig_s))
    
    private_k_points=[]##
    #for index_j in subgroup_tp1_indices:
    #    label_j = player_labels[index_j]
    #    k_j    = vect_k[index_j]
    #    private_k_points.append([label_j , k_j])
    for i in range(3):
        label_j = player_labels[i]
        k_j = vect_k[i]
        private_k_points.append([label_j, k_j])

    private_k_interpolator = Polynomial.LagrangeInterpolator(private_k_points, modulo)
    vect_kG=[] ## Precalculate for each player j the interpolated value   L_j(0) * k_j *G. The sum of this will give k*G
    for sub_index_j in range(len(subgroup_tp1_indices)):
        index_j = subgroup_tp1_indices[sub_index_j]
        label_j = player_labels[index_j]        
        kG_j = FiniteGroup.normalize_mod((private_k_interpolator[sub_index_j](0) * private_k_points[sub_index_j][1]),modulo) *G
        vect_kG.append([label_j , kG_j])
    interpolated_kG = vect_kG[0]
    for kG_j in vect_kG[1:len(vect_kG)]:
        interpolated_kG = interpolated_kG + kG_j
    interpolated_kGx = kG.x()
    interpolated_r = FiniteGroup.normalize_mod(interpolated_kGx,modulo)
    print('recalc   sig_r = {}'.format(sig_r))
    print('interpolated_r = {}'.format(interpolated_r))
    
    
    ### Generate shares k^{-1}_i for player i using auxiliaire q ####################################################################
    polynomials_q=[]

    polynomials_q = _get_defined_polynomials_deg_2_KTest(tt, modulo)
    matrix_q = __calc_matrix(polynomials_q, player_labels)

    vect_q = __calc_matrix_sum_col(matrix_q, modulo)
    vect_kq = __calc_mul_vector(vect_q,vect_k, modulo)
    print(vect_kq)
    kq_points = [] ## used to interpolate kq
    for index_j in subgroup_2tp1_indices:
    #for index_j in range(3):
        label_j = player_labels[index_j]
        kq_j    = vect_kq[index_j]
        kq_points.append([label_j , kq_j])

    print(kq_points)
    kq_interpolator = Polynomial.LagrangeInterpolator(kq_points, modulo)
    interpolated_kq = kq_interpolator (0)
    inv_interpolated_kq = FiniteGroup.inv_mod(interpolated_kq ,modulo) if modulo is not None else 1/interpolated_kq
    print(inv_interpolated_kq)
    matrix_inv_k=[]
    for row_q_i in matrix_q:
        row_inv_k_i=[]
        for q_ij in row_q_i:
            val_ij= FiniteGroup.mul_mod(inv_interpolated_kq,q_ij, modulo) if modulo is not None else inv_interpolated_kq*q_ij
            row_inv_k_i.append(val_ij)
        matrix_inv_k.append(row_inv_k_i)
    print ('MATRIX_INV_K')
    print(matrix_inv_k)
    
    vect_inv_k = __calc_matrix_sum_col(matrix_inv_k, modulo)
    print(vect_inv_k)
    inv_k_points = []  ## used to interpolate kq
    for index_j in subgroup_tp1_indices:
    #for index_j in range(3):
        label_j = player_labels[index_j]
        inv_k_j = vect_inv_k[index_j]
        inv_k_points.append([label_j , inv_k_j])
    inv_k_interpolator = Polynomial.LagrangeInterpolator(inv_k_points, modulo)
    interpolated_inv_k = inv_k_interpolator (0)
    print('recalc       inv_k = {}'.format(inv_k))
    print('interpolated_inv_k = {}'.format(interpolated_inv_k))

       ## calculate s shares s_{j} by calculating s_{j} = k^{-1}_j * (H(msg) + d_j r) ####################################################
    Hm = FiniteGroup.hash_mod(msg,modulo)
    print ('The message is %s and the hash is %s' %(msg, Hm))
    print('Interpolated_r is %s' %interpolated_r)
    print ('interpolated_inv_k is %s' % interpolated_inv_k)
    vect_s=[]
    for j in range(len(vect_inv_k)):
        vect_inv_k_j=vect_inv_k[j]
        d_j = vect_d[j]
        val_j = vect_inv_k_j * (Hm + d_j*interpolated_r)
        #val_j = interpolated_inv_k * (Hm + d_j * interpolated_r)
        val_j = FiniteGroup.normalize_mod(val_j, modulo) if modulo is not None else val_j
        vect_s.append(val_j)
    print(vect_s)
    s_points=[]
    for index_j in subgroup_2tp1_indices:
        label_j = player_labels[index_j]
        s_j    = vect_s[index_j]
        s_points.append([label_j , s_j])
    s_interpolator = Polynomial.LagrangeInterpolator(s_points, modulo)
    interpolated_s = s_interpolator (0)
    elapsed_time = time.time() - start_time

    ## standard verification of signature #############################################################################################
    ## rebuild secret shares to test
    _sig_s = inv_k * (Hm + d*sig_r)
    sig_s = FiniteGroup.normalize_mod(_sig_s, modulo)
    print('recalc   sig_s = {}'.format(sig_s))
    print('interpolated_s = {}'.format(interpolated_s))
    
    print(type(P))
    pub_key = ecdsa.ecdsa.Public_key(G,P)
    key = ecdsa.ecdsa.Private_key(pub_key,d)
    print(k)
    print('d value is %s' % d)
    print('hash value is %s' % Hm)
    ecdsa_sig = key.sign(Hm,k)
    ecdsa_sig_s = ecdsa_sig.s
    ecdsa_sig_r = ecdsa_sig.r
    print('\n\nTest signature with native Python ecdsa module ---- ')
    print('Signing message = [{}]'.format(msg))
    print('interpolated_s = {}'.format(interpolated_s))
    print('ecdsa_sig_s    = {}'.format(ecdsa_sig_s))
    print('interpolated_r = {}'.format(interpolated_r))
    print('ecdsa_sig_r    = {}'.format(ecdsa_sig_r))
    print('{} calculation time {} (s)'.format(ts, elapsed_time))
    assert(interpolated_s == ecdsa_sig_s)
    assert (interpolated_r == ecdsa_sig_r)
    
    print ("verify with ECDSA" )
    if (pub_key.verifies (Hm,ecdsa_sig)):
        print ("SUCCESS WITH ECDSA")
    else:
        print ("ERROR WITH ECDSA")
        x
    print ("Verify with threshold")
    
    thresHoldSig = ecdsa.ecdsa.Signature(interpolated_r,interpolated_s)
    if(pub_key.verifies(Hm, thresHoldSig)):
        print ("SUCESS WITH TS")
    else:
        print("FAILURE WITH TS")

def test_ThresholdSignature():## Interpolation
    #tt=3
    #margin = 3
    #nn = 2*tt + 1 + margin

    #tt = random.randint(1, 4)
    #margin = random.randint(0,5)
    tt=2
    nn = 2 * tt + 1
    print (q)

    msg='hello, I am a string, sign me now'
    #__test_ThresholdSignature(msg, 1, 3, q)
    __test_ThresholdSignature(msg, tt,nn,q)
    
    testTSSteps(msg,tt,nn, q)
