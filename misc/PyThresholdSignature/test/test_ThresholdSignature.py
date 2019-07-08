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

def __print_matrix(M):#list of list
    print('\nMatrix')
    for row_i in M:
        print(row_i)

def __get_random_polynomial(deg,modulo=None):
    return Polynomial.RandomPolynomial(deg,modulo)

def __test_threshold_signature(tt,nn,modulo=None):##
    # s = k^{-1}(H(msg) + dr)

    #### Setting up the group players, subgroup 2t+1, subsubgroup t players
    ts = Threshold.Threshold(tt,nn)
    player_indices = ts.indices()
    player_labels  = ts.labels()
    subgroup_2tp1_indices, subgroup_2tp1_labels = ts.pick_subset(2*ts.t() + 1, player_labels)  # subgroup t++
    _ , subgroup_t_indices = ts.pick_subset(ts.t()+1, subgroup_2tp1_indices)  # subgroup t++
    subgroup_t_labels  = [player_labels[index] for index in subgroup_t_indices]
    print('\n{}'.format(ts))
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



def test_ThresholdSignature():## Interpolation
    tt=3
    margin = 3
    nn = 2*tt + 1 + margin

    __test_threshold_signature(1, 3, q)
    #__test_threshold_signature(tt,nn,q)