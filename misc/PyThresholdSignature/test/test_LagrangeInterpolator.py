#!/usr/bin/env python3

import sys
sys.path.append('../')

import random
import Polynomial
import FiniteGroup


def __get_vector_x(nbPoint):
    x=[]
    for i in range(nbPoint):
        x.append(i+1)
    return x

def __get_random_subset_xfx(xfx,nbPoint):
    if nbPoint> len(xfx):
        raise RuntimeError('Cannot substract subset of {} points from {} points'.format(nbPoint, len(xfx)))

    ## Use this commented code to get a hardcoded subset of the first nbPoint points, easy to debug
    #subset_xfx = []## Help hardcoded to debug
    #for i in range (nbPoint):
    #    subset_xfx.append(xfx[i])
    #return subset_xfx
    fullnbpoint = len(xfx)
    subset_indexes = set()
    while len(subset_indexes) < nbPoint:
        random_index = random.randint(0, fullnbpoint - 1)
        subset_indexes.add(random_index)
    subset_indexes = sorted (subset_indexes)# To remove
    subset_xfx = []
    for random_index in subset_indexes:
        subset_xfx.append(xfx[random_index])
    return subset_xfx

def __get_polynomial(degree, modulo):
    ## Use this commented code to get a hardcoded polynomial x_k=(k+1), help to debug
    #f = Polynomial.Polynomial()## HELP debug, hardcoded polynomial
    #for i in range (degree):
    #    f.append_coeff(i+1)
    #f.append_coeff(degree + 1)
    #return f
    if modulo and FiniteGroup.isPrime(modulo):
        return Polynomial.RandomPolynomial(degree, modulo)
    else:
        return Polynomial.RandomPolynomial(degree)

def __interpolation_polynomial(degree, modulo):
    margin = 2
    nbPoint = degree+1+ margin

    f = __get_polynomial(degree, modulo)
    #print('Polynomial {}\n'.format(str(f)))

    full_xfx=[]
    x=__get_vector_x(nbPoint)
    for i in range (len(x)):
        full_xfx.append([x[i],f(x[i])])

    ## run random interpolations :
    ## Substract a subset of points. build Lagrange Interpolator from those points and reevaluate the values
    for nb_test_interpolation in range (100):
        xfx = __get_random_subset_xfx(full_xfx,degree+1)
        interpolator = Polynomial.LagrangeInterpolator(xfx, (modulo if modulo else None))
        #print('Interpolator {}\n'.format(str(interpolator)))
        for i in range(len(xfx)):
            x_i = xfx[i][0]
            f_i = xfx[i][1]
            ## test interpolator evaluation
            assert(f_i == interpolator(x_i))
        L=[]
        for i in range(interpolator.degree()+1):
            L.append(interpolator[i])
        for i in range(len(xfx)):
            x_i = xfx[i][0]
            f_i = xfx[i][1]
            sum=0
            for j in range(len(xfx)):
                Lji = L[j](x_i)
                assert(Lji == (1 if i==j else 0))
                sum += xfx[j][1] * Lji
            sum = FiniteGroup.normalize_mod(sum, modulo) if modulo else sum
            assert(sum==f_i)

def test_interpolation():
    __interpolation_polynomial(3, 17)