#¡/usr/bin/python

import sys
import json
import ecdsa
import binascii
import hashlib 


if __name__ == "__main__":
    print ("...starting....")
   
    TX1Buf = None
    TX1Buf = sys.stdin.readlines()
    
    
    #print(TX1Buf)
    newVal = ''.join(TX1Buf)
    print('Printing buffer')
    print(newVal)
    TXBufJson = json.loads(newVal)
    
   
    txid1 = TXBufJson["txid"]  
    value = TXBufJson["vout"][0]["value"]
    scriptKeyHash = TXBufJson["vout"][0]["scriptPubKey"]["hex"]
    
    value = int(float(value) * (10**8))
    print(value)
   
    
    val =  b'\x80\xea8\xba\x8dUO\xd8gQ\xce\xd6a0\xa9\xa5nY\x90\xf6\xd3\xc0\x9a\xec\x8b\xb4\xce\xa7_\xf4I\xcc' 
    
    print(binascii.hexlify(val))
    
    valAsHex = binascii.hexlify(val)
    
    print(valAsHex)
    
    hashedT = hashlib.sha256(valAsHex).hexdigest()
    print(hashedT)
    
    sig_val = '3045022100a7c04bb3a0d8f0b2d8d83382325be1a273d35d47ac5f6c3df7dd430e0a573e5002204da08830f5856a8f7e79f153ef139d265d9666f83ee8a858a536ae5fee8d3f24'
    
    
    hex_data = bytes.fromhex(sig_val)
  
    print(hex_data)
    

    hexDataHex = binascii.hexlify(hex_data)
    print(hexDataHex)
    
    signature_length = len(hexDataHex) + 1 # 1 byte will be added later for sighash flag
    print(signature_length)
    print(signature_length.bit_length())
    val = (signature_length.bit_length()+7)//8
    print(val)
    signature_length = signature_length.to_bytes((signature_length.bit_length()+7)//8,'big')
   
    print(type(signature_length))
    print(signature_length)
    print(binascii.hexlify(signature_length))
    
    print ('Do you wish to enter TX data')
    
    keyval = input()
    if(keyval == 'Y'):
        print('do thing')
    else:
        print ('dont do thing')
    
