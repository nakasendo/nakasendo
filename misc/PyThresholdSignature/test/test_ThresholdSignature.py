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

def __test_DealerLessSharedSecret(tt,nn,modulo=None):## Interpolation
    #if modulo is not None and not FiniteGroup.isPrime(modulo):
    #    raise RuntimeError('Group modulo {} is not a prime number'.format(modulo))

    ts = Threshold.Threshold(tt,nn)
    print('\n{}'.format(ts))
    players=[]
    for i in range (ts.n):
        players.append(__get_random_polynomial(ts.t,modulo))
    player_indices = ts.indices()
    labels = ts.labels()

    fij=[]
    for i in range (ts.n):
        fij.append(players[i](labels))

    shares=[0]*ts.n ## secret shares for player i
    for row_i in fij:
        for j in range (len(row_i)):
            shares[j]= FiniteGroup.add_mod(shares[j],row_i[j],modulo) if modulo is not None else shares[j]+row_i[j]

    subgroup_indices,subgroup_labels = ts.pick_subset(ts.t+1,labels)#subgroup t++
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

def test_DealerLessSharedSecret():## Interpolation
    __test_DealerLessSharedSecret(1, 2, q)
    tt=3
    margin = 3
    nn = 2*tt + 1 + margin
    __test_DealerLessSharedSecret(tt,nn,q)