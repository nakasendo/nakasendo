#!/usr/bin/python

import sys

import test_Threshold
import test_ThresholdSignature
import test_LagrangeInterpolator

if __name__ == "__main__":

    #test_Polynomial.test_polynomial_degree1()
    #test_Polynomial.test_polynomial_degree2()
    #test_Polynomial.test_polynomial_vector_degree2()
    #test_Polynomial.test_polynomial_degree2_mod5()
    #test_Polynomial.test_random_polynomial()
    
    
    #test_Threshold.test_threshold()
    
    test_ThresholdSignature.test_ThresholdSignature()
    
    test_LagrangeInterpolator.test_interpolation()
    
    
