import sys
import json
import string
#Please update the PYTHONPATH or use the sys.path.append with the path to 
#the Nakasendo installation
import PyBigNumbers
import PyECPoint
import PySymEncDec
import PyMessageHash
import PyAsymKey
import PyPolynomial

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
        
        
class ECKey256K1:
    def __init__ (self):
        self.pubKey, self.priKey = PyAsymKey.GenerateKeyPairPEM(); 
    
    def FromPEMStr (self, keyPemForm):
        self.pubKey, self.priKey = PyAsymKey.SetKeyFromPem(keyPemForm);
        pass
        
    def derivePublicKey(self, msg):
        return PyAsymKey.DerivePublic(self.pubKey, msg)
        
    def SplitKey(self,threshold=10,shares=100):
        return PyAsymKey.SplitKey(self.priKey, threshold, shares)
               
    def RecoverKey(self, shares):
        if(len(shares) < 2):
            return ;
        ListAsStr = ";".join(str(x) for x in shares)
        self.pubKey, self.priKey = PyAsymKey.RestoreKey(ListAsStr)
        pass
        
        
    def sign(self, msg):
        return PyAsymKey.Sign ( msg, self.priKey)
        
        
    def CalculateSharedSecret(self, pubkey):
        return PyAsymKey.ShareSecret(self.priKey, pubkey)
    
    def __str__(self):
        return 'Please take caution: Keys are valuable material\n PubKey: {} \t Private key: {}'.format (self.pubKey, self.priKey)
      
def verify(msg, pubkey, rval, sval):
    return PyAsymKey.Verify(msg, pubkey, rval,sval)  
    

class Polynomial:
    def __init__(self, degree, modulo ):
        self.degree = degree;
        self.modulo = modulo;
        self.coefficients = []

    @classmethod
    def initRandom(cls, degree, modulo):
        obj = cls(degree, modulo)
        obj.coefficients = PyPolynomial.randomPolynomial(degree, modulo);   
        return obj

    @classmethod
    def initRandomFixed_a_0(cls, degree, modulo, a_0):
        obj = cls(degree, modulo )
        obj.coefficients = PyPolynomial.randomPolynomialFixed_a_0 \
            (degree, modulo, a_0)
        return obj

    @classmethod
    def initRandomMinMax(cls, degree, modulo, min, max):
        obj = cls( degree, modulo )
        obj.coefficients = PyPolynomial.randomPolynomialMinMax \
            (degree, modulo, min, max)            
        return obj

    @classmethod
    def initFromList( cls, coeffs ) :
        obj = cls(len(coeffs), "0") 
        obj.coefficients = PyPolynomial.initFromList( coeffs ) ;
        return obj
 
    @classmethod
    def initFromListModulo( cls, coeffs, modulo ) :
        obj = cls(len(coeffs), modulo) 
        obj.coefficients = PyPolynomial.initFromListModulo( coeffs, modulo ) ;
        return obj

    def __str__(self):
        prettyStr = ""
        numberCoeffs = len(self.coefficients)
        if (numberCoeffs > 0 ) :
            prettyStr = self.coefficients[0] ;
            if (numberCoeffs > 1 ) :
                prettyStr += " + "
                prettyStr += self.coefficients[ 1 ]
                prettyStr += "x"
                if (numberCoeffs > 2 ):
                    for i in range ( 2, numberCoeffs) :
                        prettyStr += " + "
                        prettyStr += self.coefficients[i]
                        prettyStr += "x^"
                        prettyStr += str(i)

        return "degree: {0}, modulo: {1}, coefficients: {2}".format \
            (self.degree, self.modulo, prettyStr)


    def __call__(self, x):
        return PyPolynomial.evaluate(self.coefficients, x, self.modulo)

class LGInterpolator:
    def __init__(self, xfx, modulo ):
        self.points = xfx
        self.modulo = modulo

    def __call__(self, xValue, basisValue=None) :
        if basisValue is None :
            return PyPolynomial.LGInterpolatorFull( self.points, self.modulo, xValue )

        return PyPolynomial.LGInterpolatorSingle( self.points, self.modulo, xValue, basisValue)
        

    def __str__(self):
        return "points: {0}, modulo: {1}".format (self.points, self.modulo)

