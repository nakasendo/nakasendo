import sys
import json
import string
#Plese add the path to the library or update the PYTHONPATH
#sys.path.append ('PATH TO INSTALLATION LIB')
sys.path.append ('/Users/j.murphy/nchain/SDK/build/x64/release')
import PyBigNumbers
import PyECPoint
import PySymEncDec
import PyMessageHash

class MessageHash:
    def __init__(self, msg):
        self.message = msg;
    def Base64Encode(self):
        return PyMessageHash.EncodeBase64(self.message);
    def Bas64Decode(self,msg):
        return PyMessageHash.DecodeBase64(msg);

    def Base58Encode(self):
        return PyMessageHash.EncodeBase58(self.message);

    def Base58Decode(self,msg):
        return PyMessageHash.DecodeBase58(msg);

    def Base58CheckedEncode(self):
        return PyMessageHash.EncodeBase58Checked(self.message);

    def Base58CheckedDecode(self,msg):
        return PyMessageHash.DecodeBase58Checked(msg);

    def __str__(self):
        return '{}'.format (self.message);
        
class SymEncDec:
    def __init__ (self,UserPass):
        self.pw = UserPass;
        self.iv = PySymEncDec.GenerateNounce()
        self.KeyAsHex = self.GenerateKey ()
    def GenerateKey(self):
        self.KeyAsHex = PySymEncDec.GenerateKey(self.pw,self.iv);
    def GetKey(self):
        return self.KeyAsHex;
    def Encode(self, secret):
        return PySymEncDec.encodeAES(secret, self.KeyAsHex, self.iv);        
    def Decode(self,encodedAsHex):
        return PySymEncDec.decodeAES(encodedAsHex,self.KeyAsHex, self.iv);
    def __str__(self):
        return '{}, {}, {}'.format(self.pw, self.iv, self.KeyAsHex)
        
class BigNum:
    def __init__ (self):
        self.value = PyBigNumbers.GenerateRandHex(256)
    def __add__(self, obj):
        sumVal = PyBigNumbers.addFromHex(self.value, obj.value) 
        return sumVal ; 
    def __str__(self):
        return self.value

class ECPoint:
    def __init__ (self, nid=0):
        if(nid==0):
            self.value = PyECPoint.GenerateRandomECHex()
            self.nid = 714 # Value for NID_secp256k1 (This needs a bit of though)
        else:
            self.nid = nid
            self.value = PyECPoint.GenerateRandomECHexOnCurve(nid)


    def __add__(self, obj):
        if (self.nid != obj.nid):
            print ("Points not on the same curve %i and %i" % (self.nid, obj.nid))
            return none

        sumVal = PyECPoint.addFromHexWithCurveID(self.value, obj.value, self.nid)
        ecpRetVal = ECPoint(self.nid)
        ecpRetVal.SetValue(sumVal)
        return ecpRetVal ; 

    def __eq__ (self, obj):
        if ( self.nid == obj.nid):
            return (PyECPoint.compareCurve(self.value, obj.value,self.nid))  
        else:
            return False

    def multipleScalar(self,objm):
        mulVal = PyECPoint.MultiplyScalarMOnCurve(self.value, objm.value,self.nid)
        ecpRetVal = ECPoint(self.nid)
        ecpRetVal.SetValue(mulVal)
        return ecpRetVal;

    def multipltScalarEx(self, objm,objn):
        mulVal = PyECPoint.MultiplyScalarMNOnCurve(self.value, objm.value, objn.value,self.nid)        
        ecpRetVal = ECPoint(self.nid)
        ecpRetVal.SetValue (mulVal); 
        return ecpRetVal

    def GetAffineCoOrdinates(self):
        points = PyECPoint.GetAffineCoOrdinatesOnCurve(self.value,self.nid)
        return points

    def SetValue(self, value):        
        self.value = value; 

    def IsPointOnCurve (self):
        if(PyECPoint.checkOnCurveFromHexOnCurve(self.value,self.nid)):
            return True
        return False
    def __str__(self):
        return self.value

