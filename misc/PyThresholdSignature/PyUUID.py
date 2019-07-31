import os,binascii,hashlib

class UUID:
    def __init__(self, mUuid ="", mUuidLen=32):
        self.mUuidLen = mUuidLen

        if ((mUuid is None) or (str(mUuid).strip()=="")):
            self.generateUUIDString() 
        else:
            self.mUuid = mUuid

    def getString(self):
        return self.mUuid

    def generateUUIDString(self):
        self.mUuid = hashlib.sha256(binascii.b2a_hex(os.urandom(self.mUuidLen))).hexdigest()

    def getUUIDString(self):
        return self.mUuid 

    def __eq__(self, other):
        if not isinstance(other, UUID):
            return False
        return self.mUuid == other.mUuid

    def __ne__(self, other):
        if not isinstance(other, UUID):
            return False
        return self.mUuid != other.mUuid

    def __str__(self):
        return self.mUuid


