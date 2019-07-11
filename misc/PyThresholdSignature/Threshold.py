#!/usr/bin/env python3

#################################################################
#  Date             28/06/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
import random
import ecdsa

"""
Define the concept of Threshold signature
"""

class Threshold:
    """(t,n)-Threshold 1<t<n"""
    """t is the degree of the polynomial"""
    """n is the number of players"""
    def __init__(self, tt=1,nn=3):
        if tt<1 or tt>=nn:
            raise RuntimeError('Invalid Threshold (t={},n={})'.format(tt,nn))
        self.__t=tt
        self.__n=nn

    def __repr__(self):
        outstr = '({},{})-Threshold'.format(self.__t, self.__n)
        return outstr

    def t(self):
        return self.__t
    def t2p1(self):
        return 2*self.__t+1
    def n(self):
        return self.__n
    def indices(self):
        return list(self.__get_indices_set(self.__n))
    def labels(self):
        labs=[]
        for i in range(self.__n):
            labs.append(i+1)
        return labs

    ## From a full set of elements, and a sub cardinal, randomly pick a subset of the full set (in the indices order)
    def pick_subset(self, sub_card, full_set):
        full_set_card = len(full_set)
        if sub_card > full_set_card:
            raise RuntimeError('Unable to pickup {}-subset of {}-elements'.format(sub_card, full_set_card))
        unpicked_indices = list(self.__get_indices_set(full_set_card))
        picked_indices = set()
        while len(picked_indices)<sub_card:
            nb_unpicked = len(unpicked_indices)
            random_unpicked = unpicked_indices[random.randint(0, nb_unpicked-1)]
            if random_unpicked in picked_indices:
                raise RuntimeError('Error picking random subset')
            picked_indices.add(random_unpicked)
            unpicked_indices.remove(random_unpicked)
        random_picked_indices = sorted(picked_indices)
        subset=[]
        for i in range (len(random_picked_indices)):
            index=random_picked_indices[i]
            subset.append(full_set[index])
        return random_picked_indices,subset

    def __get_indices_set(self,N):
        indx=set()
        for i in range(N):
            indx.add(i)
        return indx