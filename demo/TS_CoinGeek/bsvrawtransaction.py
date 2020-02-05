import binascii
import hashlib
import ecdsa
import codecs

# secp256k1, http://www.oid-info.com/get/1.3.132.0.10
p_ec = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
r_ec = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141
b_ec = 0x0000000000000000000000000000000000000000000000000000000000000007
a_ec = 0x0000000000000000000000000000000000000000000000000000000000000000
Gx = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
Gy = 0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8
curve_secp256k1 = ecdsa.ellipticcurve.CurveFp(p_ec, a_ec, b_ec)
generator_secp256k1 = ecdsa.ellipticcurve.Point(curve_secp256k1, Gx, Gy, r_ec)
oid_secp256k1 = (1, 3, 132, 0, 10)
SECP256k1 = ecdsa.curves.Curve("SECP256k1", curve_secp256k1, generator_secp256k1, oid_secp256k1)

def get_point_pubkey(point_input):
    if (point_input.y() % 2) == 1:
        result = '03' + '%064x' % point_input.x()
    else:
        result = '02' + '%064x' % point_input.x()
    return binascii.hexlify(codecs.decode(result, 'hex'))


def doubleSHA256(data_input):
    return hashlib.sha256(hashlib.sha256(data_input).digest()).digest()


def getHashPrevouts(txid_input, vout_input):
    result = binascii.unhexlify(txid_input)[::-1]
    result += vout_input.to_bytes(4,'little')
    result = doubleSHA256(result)
    return result

def getHashSequence(sequenceNumber_input):
    sequenceNumber_bytes = sequenceNumber_input.to_bytes(4, 'little')
    result = doubleSHA256(sequenceNumber_bytes)   
    return result

def getHashOutputs(value_input, scriptPubkey_input):
    scriptPubkey_bytes = binascii.unhexlify(scriptPubkey_input)
    scriptLength = len(scriptPubkey_bytes)
    result = value_input.to_bytes(8,'little')
    result += scriptLength.to_bytes((scriptLength.bit_length()+7)//8, 'big')
    result += scriptPubkey_bytes
    #print(binascii.hexlify(result))
    result = doubleSHA256(result)
    return result

def serializeTX(hashPrevouts_input, hashSequence_input, hashOutputs_input, prevTxId_input, outputIndex_input, prevScriptPubkey_input, value_input, sequenceNumber_input):
    tx = b'\x02\x00\x00\x00' #version
    tx += hashPrevouts_input
    tx += hashSequence_input
    tx += binascii.unhexlify(prevTxId_input)[::-1]
    tx += outputIndex_input.to_bytes(4,'little')
    prevScriptPubkey_bytes = binascii.unhexlify(prevScriptPubkey_input)
    scriptLength = len(prevScriptPubkey_bytes)
    tx += scriptLength.to_bytes((scriptLength.bit_length()+7)//8, 'big')
    tx += prevScriptPubkey_bytes
    tx += value_input.to_bytes(8, 'little')
    tx += sequenceNumber_input.to_bytes(4, 'little')
    tx += hashOutputs_input
    tx += b'\x00\x00\x00\x00' # locktime
    tx += b'\x41\x00\x00\x00'
    return tx


def serializeOldTX(prev_hash_input, index_input, scriptSig_input, value_input, scriptPubkey_input):
    tx = b'\x02\x00\x00\x00' #version
    tx += b'\x01' #input count
    tx += binascii.unhexlify(prev_hash_input)[::-1]
    tx += index_input.to_bytes(4,'little')
    scriptSig_bytes = binascii.unhexlify(scriptSig_input)
    script_length = len(scriptSig_bytes)
    tx += script_length.to_bytes((script_length.bit_length()+7)//8, 'big')
    tx += scriptSig_bytes
    if script_length == 0:
        tx += b'\x00'
    tx += b'\xff\xff\xff\xff' #sequence
    tx += b'\x01' #output count
    tx += value_input.to_bytes(8,'little')
    scriptPubkey_bytes = binascii.unhexlify(scriptPubkey_input)
    script_length = len(scriptPubkey_bytes)
    tx += script_length.to_bytes((script_length.bit_length()+7)//8, 'big')
    tx += scriptPubkey_bytes
    tx += b'\x00\x00\x00\x00' # locktime
    #tx += b'\x01\x00\x00\x00' # sighash_all This is not required when submitting raw transaction
    return binascii.hexlify(tx)
