#!/usr/bin/env python3

#################################################################
#  Date             28/06/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
import random
import FiniteGroup

"""
Polynomial of degree N is defined an array of (N+1) coefficient
Lagrange polynomial is a suite of polynomial indexing from 0 to N, determined by an array of N+1 points ( x,f(x))
"""

class Polynomial:
    """Polynomial of degree N is defined an array of (N+1) coefficient"""
    def __init__(self, coeffs_array=None, group_modulo=None):
        if coeffs_array:
            self.a=coeffs_array
        if group_modulo:
            self.p=group_modulo

    def degree(self):
        return len(self.a)-1

    #def __add__(self, other):
    #    if self.degree() != other.degree():
    #        raise TypeError('Add polynomial of incompatible degree ')

class LagrangeInterpolator:
    """Lagrange interpolator is a set of N+1 points [x,f(x)]"""
    """Lagrange interpolator is a set of N polynomials that can evaluate at any point x"""
    def __init__(self, points=None, group_modulo=None):
        if points:
            self.xfx=points
        if group_modulo:
            self.p=group_modulo

    def degree(self):
        return len(self.xfx)-1