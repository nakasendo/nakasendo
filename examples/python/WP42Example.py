#!/usr/bin/env python3

import sys
import random

import PyBigNumbers
import PyECPoint
import Nakasendo

if __name__ == "__main__":

    print ("starting....")
    
    AliceKey = Nakasendo.ECKey256K1()
    BobsKey = Nakasendo.ECKey256K1()
    
    
    msg = 'This is a message that will be hased'
    
    #Alice and Bob preformn the same steps
    
    
    #ALice
    newAliceKeyPemFormat = AliceKey.derivePrivateKey(msg)
    AliceNewKey = Nakasendo.ECKey256K1()
    AliceNewKey.FromPEMStr(newAliceKeyPemFormat)
    
    pubKeyAlice = AliceNewKey.pubKey
    
    print(pubKeyAlice)
    
    
    
    #Bob
    newBobKeyPemFormat = AliceKey.derivePrivateKey(msg)
    BobsNewKey = Nakasendo.ECKey256K1()
    BobsNewKey.FromPEMStr(newBobKeyPemFormat)
    
    pubKeyBob = BobsNewKey.pubKey
    print(pubKeyBob)
    
    #Alice & Bob swap public keys and preform these steps independantly
    
    BobsSharedSecret = BobsNewKey.CalculateSharedSecret(pubKeyAlice)
    AlicesSharedSecret = AliceNewKey.CalculateSharedSecret(pubKeyBob)
    
    print(BobsSharedSecret)
    print(AlicesSharedSecret)
    
    
    
    
    
    
    
    
    
    
