#!/usr/bin/env python3

import sys
sys.path.append('../')
import Polynomial

def test_polynomial_degree1():
    # 3 + 2x
    rp = Polynomial.Polynomial()
    rp.append_coeff(3)
    rp.append_coeff(2)
    eval_at_0= rp(0)
    eval_at_1 = rp(1)
    assert(eval_at_0 == 3)
    assert (eval_at_1 == 5)

def test_polynomial_degree2():
    # 1 + 2x + 3x^2
    rp = Polynomial.Polynomial()
    rp.append_coeff(1)
    rp.append_coeff(2)
    rp.append_coeff(3)
    eval_at_0= rp(0)
    eval_at_1 = rp(1)
    eval_at_2 = rp(2)
    assert(eval_at_0 == 1)
    assert (eval_at_1 == 6)
    assert (eval_at_2 == 17)

def test_polynomial_degree2_mod5():
    # 1 + 2x + 3x^2 [mod 5]
    rp = Polynomial.Polynomial()
    rp.set_group_modulo(5)
    rp.append_coeff(6)
    rp.append_coeff(7)
    rp.append_coeff(8)
    eval_at_0= rp(0)
    eval_at_1 = rp(1)
    eval_at_2 = rp(2)
    assert(eval_at_0 == 1)
    assert (eval_at_1 == 1)
    assert (eval_at_2 == 2)

def test_random_polynomial():
    rp = Polynomial.RandomPolynomial(5)
    print('Random raw polynomial')
    print(str(rp))
    print('Random modulo polynomial 17')
    rp.set_group_modulo(17)
    print(str(rp))
    rp.append_coeff(18)
    print('Random modulo polynomial 17 append coeff 18')
    print(str(rp))