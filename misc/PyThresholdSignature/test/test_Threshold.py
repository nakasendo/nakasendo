#!/usr/bin/env python3

import random
import sys
sys.path.append('../')
import Threshold

def __get_random_vector(N):
    x=[]
    for i in range(N):
        x_i= random.randint(1, 100)
        x.append(x_i)
    return x

def test_threshold():
    tt=3
    margin = 4
    nn = tt + margin
    ts = Threshold.Threshold(tt,nn)
    random_vect = __get_random_vector(nn)
    print('Vector {}'.format(str(random_vect)))
    for i in range (10):
        picked_random = ts.pick_subset(tt, random_vect)
        print('SubVec {}'.format(str(picked_random)))