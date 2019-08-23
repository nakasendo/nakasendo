#!/usr/bin/python

import sys
import json
import Nakasendo
import random




if __name__ == "__main__":

    mykey = Nakasendo.ECKey256K1();
    print (mykey)

    shares = mykey.SplitKey(5,15);
    
    print(shares)
    
    #At some point later....
    print ("a random collection of shares")
    
    secure_random = random.SystemRandom()
    
    subset = secure_random.choices(shares, k=5)
    
    print(subset)
    
    
    
    
    recoveredKey = Nakasendo.ECKey256K1()
    recoveredKey.RecoverKey(subset)
    
    print(recoveredKey)
    
