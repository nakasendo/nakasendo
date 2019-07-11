#!/usr/bin/env python3

import sys
sys.path.append('../')
import FiniteGroup

def test_normalize_mod():
    assert(FiniteGroup.normalize_mod(0,7)==0)
    assert (FiniteGroup.normalize_mod(1, 7) == 1)
    assert (FiniteGroup.normalize_mod(-1, 7) == 6)
    assert (FiniteGroup.normalize_mod(-6, 7) == 1)
    assert (FiniteGroup.normalize_mod(-8, 7) == 6)
    assert (FiniteGroup.normalize_mod(7, 7) == 0)
    assert (FiniteGroup.normalize_mod(8, 7) == 1)
    assert (FiniteGroup.normalize_mod(13, 7) == 6)
    assert (FiniteGroup.normalize_mod(15, 7) == 1)

def test_mul_mod():
    assert(FiniteGroup.mul_mod(3,4,7)==5)
    assert (FiniteGroup.mul_mod(-1,4,7)==3)
    assert (FiniteGroup.mul_mod(0,4,7)==0)
    assert (FiniteGroup.mul_mod(4,0,7)==0)

def test_inv_mod():
    assert(FiniteGroup.inv_mod(1,7)==1)
    assert (FiniteGroup.inv_mod(2, 7) == 4)
    assert (FiniteGroup.inv_mod(-5, 7) == 4)
    assert (FiniteGroup.inv_mod(16,7) == 4)

def test_inv_mod():
    assert(FiniteGroup.inv_mod(1,7)==1)
    assert (FiniteGroup.inv_mod(2, 7) == 4)
    assert (FiniteGroup.inv_mod(-5, 7) == 4)
    assert (FiniteGroup.inv_mod(16,7) == 4)

def test_is_prime():
    assert (not FiniteGroup.isPrime(1))
    assert(FiniteGroup.isPrime(2))
    assert (FiniteGroup.isPrime(3))
    assert (FiniteGroup.isPrime(5))
    assert (FiniteGroup.isPrime(7))
    assert (FiniteGroup.isPrime(11))
    assert (FiniteGroup.isPrime(13))
    assert (FiniteGroup.isPrime(17))
    assert (FiniteGroup.isPrime(19))
    assert (FiniteGroup.isPrime(23))
    assert (FiniteGroup.isPrime(29))
    assert (FiniteGroup.isPrime(31))
    assert (FiniteGroup.isPrime(37))
    assert (FiniteGroup.isPrime(41))
    assert (FiniteGroup.isPrime(43))
    assert (FiniteGroup.isPrime(47))
    assert (FiniteGroup.isPrime(53))
    assert (FiniteGroup.isPrime(59))
    assert (FiniteGroup.isPrime(61))
    assert (FiniteGroup.isPrime(67))
    assert (FiniteGroup.isPrime(71))
    assert (FiniteGroup.isPrime(7919))

def test_random_generator():
    nbIter=1000
    p=1000000000000
    for i in range (nbIter):
        ran_x = FiniteGroup.generate_random_mod(p)
        assert (0<=ran_x and ran_x<p)
        ran_non_zero_x = FiniteGroup.generate_random_positive_mod(p)
        assert (0< ran_x and ran_x < p)