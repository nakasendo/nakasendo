#!/usr/bin/env python3
import sys
import json
import string
#Please update the PYTHONPATH or use the sys.path.append with the path to 
#the Nakasendo installation
sys.path.append('/users/j.murphy/nchain/SDK/sdklibraries/src/modules')
sys.path.append ('/users/j.murphy/nchain/SDK/build/x64/release')
import PyBigNumbers
import PyECPoint
import PySymEncDec
import PyMessageHash
import Nakasendo

if __name__ == "__main__":
    print ("starting.....")
    
    
    print ("Testing Base64..Encoding")
    msgToEncode = 'Development team'
    for x in range(1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        #print (encoded)
        if (encoded != 'RGV2ZWxvcG1lbnQgdGVhbQ=='):
            print ("WEIRDNESS")
        decoded = myMsgHash.Bas64Decode(encoded)
        #print (decoded)
    msgToEncode = 'Research team'
    for x in range (1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        #print(encoded)
        if (encoded != 'UmVzZWFyY2ggdGVhbQ=='):
            print ('WEIRDNESS')
        decoded = myMsgHash.Bas64Decode(encoded)
        #print(decoded)
        
    msgToEncode = 'nChain Limited UK branch is fast growing in FinTech industry'
    for x in range (1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        #print(encoded)
        if (encoded != 'bkNoYWluIExpbWl0ZWQgVUsgYnJhbmNoIGlzIGZhc3QgZ3Jvd2luZyBpbiBGaW5UZWNoIGluZHVzdHJ5'):
            print ('WEIRDNESS')
            print (encoded)
        decoded = myMsgHash.Bas64Decode(encoded)
        #print(decoded)
