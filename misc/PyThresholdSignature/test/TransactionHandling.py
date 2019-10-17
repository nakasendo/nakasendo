#¡/usr/bin/python


import json
import sys
import bsvrawtransaction
import binascii
import hashlib

def ParseInfoFromTX(RawTX):

    RawTXAsStr = ''.join(RawTX)
    TXJson = json.loads(RawTXAsStr)
    txid = TXJson["txid"]
    value = TXJson["vout"][0]["value"]
    scriptKeyHash = TXJson["vout"][0]["scriptPubKey"]["hex"]
    
    value = int(float(value) * (10**8))
    return (txid, value, scriptKeyHash)
    
    
    

def CreateTransaction(prevTxId,prevScriptPubkey,prevValue,value,scriptPubkey,pkCompressed,prevIndex=0,sequenceNumber=4294967295):
    hashPrevouts = bsvrawtransaction.getHashPrevouts(prevTxId,prevIndex)
#print("The hash of the outpoint that is being spent: {}".format(binascii.hexlify(hashPrevouts)))
    
    
    
    #Please remember to multiple these values by the correct number for satoshis
    hashSequence = bsvrawtransaction.getHashSequence(sequenceNumber)
#print("The hash of the sequence number: {}".format(binascii.hexlify(hashSequence)))

    hashOutput = bsvrawtransaction.getHashOutputs(value, scriptPubkey)
#print("The hash of the outputs: {}".format(binascii.hexlify(hashOutput)))

    transaction = bsvrawtransaction.serializeTX(hashPrevouts, hashSequence, hashOutput, prevTxId, prevIndex, prevScriptPubkey, prevValue, sequenceNumber)

    #print("The transaction is {} \n".format(binascii.hexlify(transaction)))
    
    hashTX = hashlib.sha256(transaction).digest()
    return hashTX
    
    
    
def SerialiseFinalTx(prevtxid, scriptSig_final, value,scriptKeyHash):
    signedTX = bsvrawtransaction.serializeOldTX(prevtxid, 0, scriptSig_final, value, scriptKeyHash)
    return signedTX
    
