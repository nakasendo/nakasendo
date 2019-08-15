#!/usr/bin/env python3
import sys
import json
import string
#Please update the PYTHONPATH or use the sys.path.append with the path to 
#the Nakasendo installation
import PyBigNumbers
import PyECPoint
import PySymEncDec
import PyMessageHash
import Nakasendo



def DoEncodeDecode (msgToEncode, encodedMsg):
    for x in range (1,10):
        myMsgHash =  Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base64Encode()
        if (encoded != encodedMsg):
            print ('API ENCODE PANIC %s is not equal to %s' % (encoded, encodedMsg))
        #print (encoded)
        decoded = myMsgHash.Bas64Decode(encoded)
        if (decoded != msgToEncode):
            print ('API DECODE PANIC %s is not equal to %s' % (decoded, msgToEncode))
        #print (decoded)
        
        
def DoEncodeDecodeB58 (msgToEncode, encodedMsg):
    for x in range (1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base58Encode()
        if(encoded != encodedMsg):
            print ('API ENCODE 58 panic %s is not equal to %s' % (encoded, encodedMsg))
        decoded = myMsgHash.Base58Decode(encoded)
        if(decoded != msgToEncode):
            print ('API DECODE 58 panic %s is not equal to %s' % (encoded, encodedMsg))
            
def DoEncodeDecodeB58Checked (msgToEncode, encodedMsg):
    for x in range (1,10):
        myMsgHash = Nakasendo.MessageHash(msgToEncode)
        encoded = myMsgHash.Base58CheckedEncode()
        if(encoded != encodedMsg):
            print ('API ENCODE 58 Checked panic %s is not equal to %s' % (encoded, encodedMsg))
        decoded = myMsgHash.Base58CheckedDecode(encoded)
        if(decoded != msgToEncode):
            print ('API DECODE 58 Checked panic %s is not equal to %s' % (encoded, encodedMsg))
    
if __name__ == "__main__":
    print ("starting.....")

    
    print ("Testing Base64..Encoding")
    msgToEncode = 'Development team'
    encodedMsg = 'RGV2ZWxvcG1lbnQgdGVhbQ=='
    DoEncodeDecode (msgToEncode, encodedMsg)
        
    msgToEncode = 'Research team'
    encodedMsg = 'UmVzZWFyY2ggdGVhbQ=='
    DoEncodeDecode (msgToEncode, encodedMsg)
    
        
    msgToEncode = 'team'
    encodedMsg = 'dGVhbQ=='
    DoEncodeDecode (msgToEncode, encodedMsg)
        
    msgToEncode = 'nChain Limited UK branch is fast growing in FinTech industry'
    encodedMsg = 'bkNoYWluIExpbWl0ZWQgVUsgYnJhbmNoIGlzIGZhc3QgZ3Jvd2luZyBpbiBGaW5UZWNoIGluZHVzdHJ5'
    DoEncodeDecode(msgToEncode, encodedMsg)
    
    msgToEncode = 'Programmer'
    encodedMsg = 'UHJvZ3JhbW1lcg=='
    DoEncodeDecode (msgToEncode, encodedMsg)
    
    
    msgToEncode = 'Our mission is to leverage global trade through blockchain-driven solutions'
    encodedMsg = 'T3VyIG1pc3Npb24gaXMgdG8gbGV2ZXJhZ2UgZ2xvYmFsIHRyYWRlIHRocm91Z2ggYmxvY2tjaGFpbi1kcml2ZW4gc29sdXRpb25z'
    DoEncodeDecode (msgToEncode, encodedMsg)
    
    msgToEncode = 'nChain creates a platform of cryptography application'
    encodedMsg = 'bkNoYWluIGNyZWF0ZXMgYSBwbGF0Zm9ybSBvZiBjcnlwdG9ncmFwaHkgYXBwbGljYXRpb24='
    DoEncodeDecode (msgToEncode,encodedMsg)
    
    print ("Tesing Base 58 encoding")
    msgToEncode = 'The power of the Bitcoin blockchain'
    encodedMsg = '9RVHjYrMbXcuv99YiGbS7N4T4XV9te878PGbFsihLZc4Qv93'
    DoEncodeDecodeB58(msgToEncode,encodedMsg)
    
    
    msgToEncode = 'Be our next Research & Development Specialist'
    encodedMsg = '2eHQGCXjS1F5PdgpxDv1JFBYghsGhY3cw5fcWDvG2hizZnQQrk2UxcQW7m5ZAB'
    DoEncodeDecodeB58(msgToEncode, encodedMsg)
    
    msgToEncode = 'Our PHD-qualified scientists and researchers are finding new ways to scale the Bitcoin network and ignite the power of blockchain technology'
    encodedMsg = 'gHGyvFP4hJFxGBUUkSJTfbAuc2Tc8tDNbKXVVm5ENrJwNfkgwKducNUWNDpdjCrs2JwDDBM1Tb1GdqTy2zvbJE9CgDQdHe2QnjdoeqXDq95HAUcDo8Z7SQXW3wfdDeTTwe18xg7fZJGCMkXu5oy1Nx3NWuS3kFBRW7RGbaMVtRyCpSdZ2SebVCjMjChknoa'
    DoEncodeDecodeB58(msgToEncode,encodedMsg)
    
    msgToEncode = 'Our PHD-qualified scientists and researchers are finding new ways to scale the Bitcoin network and ignite the power of blockchain technology Their discoveries will enable new functions of the Bitcoin SV blockchain and re-invent the way enterprises operate'
    encodedMsg = 'qG2dm1WC6DtqVs4qFWrpfETZz9cgmwtFsWKKXiJ2uv2ehHcSwt1JPUtygXDC63gD12V6qDG2Y6zF43XEUUeW8p7PXoT9L4j3Bomc7dsWjkgknTLpK9iKfocicGfQHoUymsdMpq3LXR9JdNwHxhtEWn8Ro2hcBUqvqf6QC17AbG5x1c4fXPvtV3cHf2sjhBqx8RtZ1UaLArhk2X4QjHUz5sETM3vysMBBpHp2nmu1xsccBucvknD16kUeeArVscHMpDBz27CqCeTtoXVN9GY3XG8or7YV9WuRQR6rMeYwrqdxCJQrgGuxpEhpq4TJCUnqTzN8iq5TnptNtkKQp7SU9KpHdCFn'
    DoEncodeDecodeB58(msgToEncode,encodedMsg)
    
    msgToEncode = 'nChain experienced developers are working on applications that will allow our research insights to be harnessed by business enterprises and in addition to proprietary products and also we will support the Bitcoin SV community by creating open-source and royalty-free software tools that help accelerate blockchain technology'
    encodedMsg = '3wgiRkXLWjNRacdqjm4m1LWerLiZdzCboVBSHcsB95As5X62Ng81p1PtDcKsQw8zsqYT7HTCgJeynGHWN17vgAiFyRDjNigXHpcGPUb7ENFLT5taFwZ3b8T2EznjPEo48Cc5KsBna1kVLqyuaGpdwEJvpL8BcVbdmpQRDa8MU1ehPu9JwVeF7hNZTPTQVJJphUhBxrUE6SeNjTVjjRjtWV2YKqjwuqo5QjKGUjQCKkiQsHHvNCXZiUQbQM2kSddjopDQiJzZdVcbTxQHV41dqnkHhqknRC3smpauXMa23FsC6ogQJqQ8nFocxQq6TUFPcuHZjT2KE2M7RsaeRwhF5LtWeCykokpRksRVJxEqbcZNRQbXU5tLLyJbPdtT3zVLmTeKdqnFqLNnmDZtps7iPsTkTKgw5WzmupVMnLgxYcbQK8tq5Fmcv69Ghmi'
    DoEncodeDecodeB58(msgToEncode,encodedMsg)
    
    print ("Testing Base 58-checked encoding")
    msgToEncode = 'The power of the Bitcoin blockchain'
    encodedMsg = 'x7VWSoeJxFaY95vFJvuxEbjfK2tCZWwN1fdK3vm2dJJDExFwriPq5'
    DoEncodeDecodeB58Checked(msgToEncode,encodedMsg)
    
    msgToEncode = 'Our PHD-qualified scientists and researchers are finding new ways to scale the Bitcoin network and ignite the power of blockchain technology'
    encodedMsg = '5S3ETGhJwWAgaherHYZSXrshqpdyauZt3euWczvmauDKp6PWud6eKoSPZHLn6Gpq6RyRFebRoxJwwF36zcCzyWCYAFdn8EaFf82M53L5UiLq5hDVgSdHPpgiFMkUXJggRUXLGgDCzeG87TLTMLfkxLn1dW9QAMDDWxQdFYzrf37hP2GSWBrxd4B3KTwXqSw8UYPFB'
    DoEncodeDecodeB58Checked(msgToEncode,encodedMsg)
    
    msgToEncode = 'Our PHD-qualified scientists and researchers are finding new ways to scale the Bitcoin network and ignite the power of blockchain technology Their discoveries will enable new functions of the Bitcoin SV blockchain and re-invent the way enterprises operate and transform people\'s lives'
    #print (msgToEncode)
    #encodedMsg =  'AfrHWu8di5XFxq5BvbBBse4BBxksXEm4dCVT4ZvBnf3CgDAA4nZASRws51ZxiQfnp5eu1xX4XdqvS5fekEbtK1GDFm9NiLCFrw1EZoLG7TftKaH4rRSheFnAHY8JWkkXcR9AiGFYAV64HcDSxX4PcSmunTQXFDhBiMNdHoWihJ9vNnPCDmY6zVsk5qLChuXxd8kGHd9sE7GSvRPHoJLLLWeQgbq3bNrYymJAVjJKvajm38CdrTf2kjoMfzA4z3jAWnNnEtVdKMa4951EAM21KTeHo3UYaWQaT5Jdytdwu4T6XZCvT9nhSUtansoGEwP2FBRipmT2sDukjhxLSBqG2UUTZpVwuJcc36suHSGPwrcGVfoK63VVyUXiWTWVAbiQmAw4'     
    encodedMsg = '26H1AwjJTeAcDxYorAKvG1TA8sc6sHjo8y7gTWubxvTX1BvBnfVke2zydJxg2wrtm68Z2V9qa2oys9Ybbhb6dxs48oPQfAQ6zWKrwSksuKQZdbZFNjwsSyjmGcYwEpPN7gXUPzWpd9cSjEts9Px3fCaEBbq3eVCrabgRdqgG8eXNTeoxM5sVWxDxWFhwQyF7Eq36q1NbSDYVQDkHJzmv5ifFvfT1KcTyA8y98EchvD1tbQUoPETrXBWLyzmxj6yeCCYMPAedS8M7jBcAnQqfY5xa3iBNyd86xSVfBAG5BcyF9nxNoqGiTWfcmEpSSCuZResXv1a4DZzjQCyjhTrpjwnNEt7i7tjgcJBG3nxWXW5cvSWjHUtwxpLhYgkdxkUVWWdhcsSJuc'
    DoEncodeDecodeB58Checked(msgToEncode,encodedMsg)
    
    msgToEncode='nChain experienced developers are working on applications that will allow our research insights to be harnessed by business enterprises and in addition to proprietary products and also we will support the Bitcoin SV community by creating open-source and royalty-free software tools that help accelerate blockchain technology'
    encodedMsg='LFuEvP3u4JaaF9EHSJJdJwmF1KA2LJ5xrTNEy26YA3dXhPtkutyafmKgYr1PHSDELpRt2K6CYZWfvrPXxgSJRF8MRhCrSH79Z2p4sDdbXoDzezNm1PBnsWwLA5XehuxwQpJHs8KKgM29SXzX4wTxBFX9ck8NG4GvhH3FuxKPNDJsNGPBykj3frzXTUMRKJ1Mc7bw5tQKK6dJveJvsVEysuktxb7Bc9zUTnFD6iQdB6chGNzKUFHFmf1cS6FN4cMhZaLHhuJL7rkyLSHgqBKcyYaAJ78N1nnsGib4KWP7ZpPnPtEoBF63kV8ChneGpAU3xGT5v8oGBuymMGr342sLpPVXH2e9q4ty6fHuGGxWiLd5KootkJR798gSWYAbyw3BzxDziCjJvraw6eDMnj8dLXaP7tJm7NmFia3iScKVubr4GfpUQepeqd1uNjpqiJTe'
DoEncodeDecodeB58Checked(msgToEncode,encodedMsg)
    
    
    
    
        
    
    
    
    
