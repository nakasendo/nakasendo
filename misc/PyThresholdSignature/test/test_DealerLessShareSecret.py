#!/usr/bin/env python3

import random
import sys
sys.path.append('../')
import FiniteGroup
import Polynomial
import Threshold
import ecdsa

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

## return vect_c where vect_c_i= vect_a_i * vect_b_i
def __calc_mul_vector(vect_a, vect_b, modulo=None):
    assert (len(vect_a) == len(vect_b))
    vect_c = [k_j * q_j for [k_j, q_j] in zip(vect_a, vect_b)]
    if modulo is not None:
        for j in range(len(vect_c)):
            vect_c[j]=FiniteGroup.normalize_mod(vect_c[j],modulo)
    return vect_c

## return the vector sum of columns of a matrix
def __calc_matrix_sum_col(matrix, modulo=None):
    vect_sum_col=[0]* len(matrix[0])
    for row_i in matrix:
        for j in range(len(row_i)):
            x_ij=row_i[j]
            vect_sum_col[j] = FiniteGroup.add_mod(vect_sum_col[j],x_ij,modulo) if modulo is not None else vect_sum_col[j]+x_ij
    return vect_sum_col

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

## return list of coefficients
def __get_fixed_2list_polynomials():
    kk=[[1,2],[3,4],[5,6]]
    qq = [[7,8],[9,10],[11,12]]
    return kk,qq

def __test_DealerLessSharedSecret(tt,nn,modulo=None):## Interpolation
    #if modulo is not None and not FiniteGroup.isPrime(modulo):
    #    raise RuntimeError('Group modulo {} is not a prime number'.format(modulo))

    ts = Threshold.Threshold(tt,nn)
    print('\n{}'.format(ts))
    players=[]
    for i in range (ts.n()):
        players.append(__get_random_polynomial(ts.t(),modulo))
    player_indices = ts.indices()
    labels = ts.labels()

    fij=[]
    for i in range (ts.n()):
        fij.append(players[i](labels))

    shares=[0]*ts.n() ## secret shares for player i
    for row_i in fij:
        for j in range (len(row_i)):
            shares[j]= FiniteGroup.add_mod(shares[j],row_i[j],modulo) if modulo is not None else shares[j]+row_i[j]

    subgroup_indices,subgroup_labels = ts.pick_subset(ts.t()+1,labels)#subgroup t++
    print('Subgroup players {}'.format(subgroup_indices))
    shares_tpp= [0]*(len(subgroup_indices))
    for i in range(len(subgroup_indices)):
        player_index = subgroup_indices[i]
        shares_tpp[i] = shares[player_index]

    tn_points=[]
    for i in range(len(subgroup_labels)):
        tn_points.append([subgroup_labels[i], shares_tpp[i]])

    tn_interpolator = Polynomial.LagrangeInterpolator(tn_points, modulo)
    interpolated_share_secret = tn_interpolator(0)
    rebuild_share_secret=0
    for player_i in players:
        rebuild_share_secret= FiniteGroup.add_mod(rebuild_share_secret,player_i.a[0],modulo) if modulo is not None else rebuild_share_secret+player_i.a[0]

    print('Interpolated {}'.format(interpolated_share_secret))
    print('rebuild      {}'.format(rebuild_share_secret))
    assert(interpolated_share_secret == rebuild_share_secret)
    print('----------------------------------  End of test_DealerLessSharedSecret -------')

def __test_DealerLessMultiplication(tt,nn,modulo=None):
    if 2*tt+1>nn:
        raise RuntimeError('Bad threshold configuration 2t+1>n with t={} n={}'.format(tt,nn))

    ts = Threshold.Threshold(tt,nn)
    player_indices = ts.indices()
    player_labels  = ts.labels()
    subgroup_2tp1_indices, subgroup_2tp1_labels = ts.pick_subset(2*ts.t() + 1, player_labels)  # subgroup t++
    _ , subgroup_t_indices = ts.pick_subset(ts.t()+1, subgroup_2tp1_indices)  # subgroup t++
    subgroup_t_labels  = [player_labels[index] for index in subgroup_t_indices]
    print('\n---  test_DealerLessMultiplication -------')
    print(ts)
    print('Subgroup 2tp1 indices {}'.format(subgroup_2tp1_indices))
    print('Subgroup 2tp1 labels  {}'.format(subgroup_2tp1_labels))
    print('Subgroup    t indices {}'.format(subgroup_t_indices))
    print('Subgroup    t labels  {}'.format(subgroup_t_labels))
    assert(2*(len(subgroup_t_indices)-1)+1 == len(subgroup_2tp1_labels)) ## test 2t+1 correctness
    for i in range (len(subgroup_2tp1_labels)): ## test label == index+1
        assert((subgroup_2tp1_indices[i]+1) == subgroup_2tp1_labels[i])
    for i in range (len(subgroup_t_labels)):
        assert(subgroup_t_indices[i] in subgroup_2tp1_indices) ## test inclusion group_t inside group_2tp1
        assert(subgroup_t_labels[i] in subgroup_2tp1_labels)   ## test inclusion group_t inside group_2tp1
        assert((subgroup_t_indices[i]+1) == subgroup_t_labels[i])  ## test label == index+1

    polynomials_k=[]
    for i in range (ts.n()):
        polynomials_k.append(__get_random_polynomial(ts.t(),modulo))
    polynomials_q=[]
    for i in range (ts.n()):
        polynomials_q.append(__get_random_polynomial(ts.t(),modulo))

    matrix_k = __calc_matrix(polynomials_k, player_labels)
    matrix_q = __calc_matrix(polynomials_q, player_labels)

    vect_sum_col_k = __calc_matrix_sum_col(matrix_k, modulo)
    vect_sum_col_q = __calc_matrix_sum_col(matrix_q, modulo)

    vect_kq = __calc_mul_vector(vect_sum_col_q,vect_sum_col_k, modulo)
    kq_points = [] ## used to interpolate kq
    for index_j in subgroup_2tp1_indices:
        label_j = player_labels[index_j]
        kq_j    = vect_kq[index_j]
        kq_points.append([label_j , kq_j])
    kq_interpolator = Polynomial.LagrangeInterpolator(kq_points, modulo)
    interpolated_kq = kq_interpolator (0)
    shared_q = __calc_shared_secret(polynomials_q)
    shared_k = __calc_shared_secret(polynomials_k)
    rebuild_shared_kq = FiniteGroup.mul_mod(shared_k,shared_q,modulo) if modulo is not None else shared_k*shared_q
    print('Interpolated kq {}'.format(interpolated_kq))
    print('rebuild      kq {}'.format(rebuild_shared_kq))
    assert (rebuild_shared_kq == interpolated_kq)
    print('----------------------------------  End of test_DealerLessMultiplication -------')

def __test_DealerLessInverse(tt,nn,modulo=None):
    if 2*tt+1>nn:
        raise RuntimeError('Bad threshold configuration 2t+1>n with t={} n={}'.format(tt,nn))

    ts = Threshold.Threshold(tt,nn)
    player_indices = ts.indices()
    player_labels  = ts.labels()
    subgroup_2tp1_indices, subgroup_2tp1_labels = ts.pick_subset(2*ts.t() + 1, player_labels)  # subgroup t++
    _ , subgroup_t_indices = ts.pick_subset(ts.t()+1, subgroup_2tp1_indices)  # subgroup t++
    subgroup_t_labels  = [player_labels[index] for index in subgroup_t_indices]
    print('\n---  test_DealerLessMultiplication -------')
    print(ts)
    print('Subgroup 2tp1 indices {}'.format(subgroup_2tp1_indices))
    print('Subgroup 2tp1 labels  {}'.format(subgroup_2tp1_labels))
    print('Subgroup    t indices {}'.format(subgroup_t_indices))
    print('Subgroup    t labels  {}'.format(subgroup_t_labels))
    assert(2*(len(subgroup_t_indices)-1)+1 == len(subgroup_2tp1_labels)) ## test 2t+1 correctness
    for i in range (len(subgroup_2tp1_labels)): ## test label == index+1
        assert((subgroup_2tp1_indices[i]+1) == subgroup_2tp1_labels[i])
    for i in range (len(subgroup_t_labels)):
        assert(subgroup_t_indices[i] in subgroup_2tp1_indices) ## test inclusion group_t inside group_2tp1
        assert(subgroup_t_labels[i] in subgroup_2tp1_labels)   ## test inclusion group_t inside group_2tp1
        assert((subgroup_t_indices[i]+1) == subgroup_t_labels[i])  ## test label == index+1

    polynomials_k=[]
    for i in range (ts.n()):
        polynomials_k.append(__get_random_polynomial(ts.t(),modulo))
    polynomials_q=[]
    for i in range (ts.n()):
        polynomials_q.append(__get_random_polynomial(ts.t(),modulo))

    matrix_k = __calc_matrix(polynomials_k, player_labels)
    matrix_q = __calc_matrix(polynomials_q, player_labels)

    vect_sum_col_k = __calc_matrix_sum_col(matrix_k, modulo)
    vect_sum_col_q = __calc_matrix_sum_col(matrix_q, modulo)

    vect_kq = __calc_mul_vector(vect_sum_col_q,vect_sum_col_k, modulo)
    kq_points = [] ## used to interpolate kq
    for index_j in subgroup_2tp1_indices:
        label_j = player_labels[index_j]
        kq_j    = vect_kq[index_j]
        kq_points.append([label_j , kq_j])
    kq_interpolator = Polynomial.LagrangeInterpolator(kq_points, modulo)
    interpolated_kq = kq_interpolator (0)
    inv_interpolated_kq = FiniteGroup.inv_mod(interpolated_kq ,modulo) if modulo is not None else 1/interpolated_kq
    invkq_q_points = []  ## used to interpolate kq
    for index_j in subgroup_t_indices:
        label_j = player_labels[index_j]
        invkq_q_j = FiniteGroup.mul_mod(inv_interpolated_kq,vect_sum_col_q[index_j], modulo) if modulo is not None else inv_interpolated_kq*vect_sum_col_q[index_j]
        invkq_q_points.append([label_j, invkq_q_j])
    invkq_q_interpolator = Polynomial.LagrangeInterpolator(invkq_q_points, modulo)
    interpolated_invkq_q = invkq_q_interpolator(0)
    interpolated_invk=interpolated_invkq_q
    rebuild_shared_k = __calc_shared_secret(polynomials_k)
    rebuild_inv_k = FiniteGroup.inv_mod(rebuild_shared_k,modulo) if modulo is not None else 1/rebuild_shared_k
    print('Interpolated inv_k {}'.format(interpolated_invk))
    print('rebuild      inv_k {}'.format(rebuild_inv_k))
    assert (interpolated_invk == rebuild_inv_k)
    print('----------------------------------  End of test_DealerLessInverse -------')

def test_DealerLessSharedSecret():## Interpolation
    tt=3
    margin = 3
    nn = 2*tt + 1 + margin
    __test_DealerLessSharedSecret(tt,nn,q)
    __test_DealerLessMultiplication(tt, nn, q)
    __test_DealerLessInverse(tt, nn, q)

