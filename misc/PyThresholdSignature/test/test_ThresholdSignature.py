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

def __test_ThresholdSignature(msg, tt,nn,modulo=None):##
    # s = k^{-1}(H(msg) + dr)
    if 2*tt+1>nn:
        raise RuntimeError('Bad threshold configuration 2t+1>n with t={} n={}'.format(tt,nn))

    start_time = time.time()
    ts = Threshold.Threshold(tt,nn)
    player_indices = ts.indices()
    player_labels  = ts.labels()
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
    matrix_d = __calc_matrix(polynomials_d, player_labels)
    vect_d = __calc_matrix_sum_col(matrix_d, modulo)
    ## Calculate interpolated P=d*G
    vect_P = [d_i*G for d_i in vect_d]
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
    #print('recalc   sig_r = {}'.format(sig_r))
    #print('interpolated_r = {}'.format(interpolated_r))

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
    #print('recalc       inv_k = {}'.format(inv_k))
    #print('interpolated_inv_k = {}'.format(interpolated_inv_k))

    ## calculate s shares s_{j} by calculating s_{j} = k^{-1}_j * (H(msg) + d_j r) ####################################################
    Hm = FiniteGroup.hash_mod(msg,modulo)
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

def test_ThresholdSignature():## Interpolation
    #tt=3
    #margin = 3
    #nn = 2*tt + 1 + margin

    tt = random.randint(1, 4)
    margin = random.randint(0,5)
    nn = 2 * tt + 1 + margin

    msg='hello, I am a string, sign me now'
    #__test_ThresholdSignature(msg, 1, 3, q)
    __test_ThresholdSignature(msg, tt,nn,q)