#!/usr/bin/env python3

#################################################################
#  Date             28/06/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
import random
import hashlib
"""
A finite group is defined by equivalent relation (modulo in our case)
It is defined by a class modulo p and a group order n.
"""

def _xgcd(a, b):
    """https://en.wikibooks.org/wiki/Algorithm_Implementation/Mathematics/Extended_Euclidean_algorithm"""
    """return (g, x, y) such that a*x + b*y = g = gcd(a, b)"""
    x0, x1, y0, y1 = 0, 1, 1, 0
    while a != 0:
        q, b, a = b // a, a, b % a
        y0, y1 = y1, y0 - q * y1
        x0, x1 = x1, x0 - q * x1
    return b, x0, y0

def normalize_mod(a,p):
    """Bring any number to [0 ... p-1]."""
    if a>p:
        return a%p
    opp_sign_a = -1 if a>0 else 1
    while a<0 or a>=p:
        a = a + opp_sign_a*p
    return a

def isPrime(n):
    """Returns True if n is prime."""
    if n <= 1:
        return False
    if n == 2:
        return True
    if n == 3:
        return True
    if n % 2 == 0:
        return False
    if n % 3 == 0:
        return False

    i = 5
    w = 2
    while i * i <= n:
        if n % i == 0:
            return False
        i += w
        w = 6 - w
    return True

def add_mod(a,b,p):
    """return (a+b)%p."""
    return normalize_mod((a+b),p)

def mul_mod(a,b,p):
    """return (a*b)%p."""
    return normalize_mod((a*b),p)

def inv_mod(a,p):
    """return x such that (x * a) % b == 1"""
    if p < 2:
        raise ZeroDivisionError('inv_mod({},{}) second parameter expected to be greater than 2'.format(a, p))

    norm_a = normalize_mod(a,p)
    if norm_a==0:
        raise ZeroDivisionError('inv_mod({},{}) error {}=0 mod{}'.format(a,p,a,p))
    g, x, _ = _xgcd(norm_a, p)
    if g != 1:
        raise RuntimeError('inv_mod({},{}) unable to calculate egcd({},{})'.format(a,p,a,p))
    return x % p

def generate_random_mod(p):
    """generate random int from [0 ... p-1]."""
    return random.randint(0, p - 1)

def generate_random_positive_mod(p):
    """generate random int from [1 ... p-1]."""
    return random.randint(1, p - 1)

def hash_mod(msg_str, p):
    """hash a message to an int in a group p."""
    msg_bin = bytes(msg_str, 'utf-8')
    hashed = hashlib.sha256(msg_bin)
    digest = hashed.hexdigest()
    res = int(digest,16)
    return normalize_mod(res,p) if p is not None else res