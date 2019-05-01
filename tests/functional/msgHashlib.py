import PyMessageHash

def hashMsgSHA256_method(msg):
    #Generate SHA256 hash on a given input
    return PyMessageHash.HashMsgSHA256(msg)

def hashMsg_method(msg, hashTool_type):
    #Generate a Hash on a given input using the hashing algorithim specified
    return PyMessageHash.HashMsg(msg, hashTool_type)

def encodeBase64_method(msg):
    #Encode given string in base64
    return  PyMessageHash.EncodeBase64(msg)

def decodeBase64_method(msg_hash64):
    #Decode given string in base64
    return  PyMessageHash.DecodeBase64(msg_hash64)