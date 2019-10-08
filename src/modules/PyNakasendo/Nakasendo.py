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
    def __init__ (self, value=None):
        if value is None:
            self.value = PyBigNumbers.GenerateRandHex(256)
        else:
            self.value = value
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
        
    def GetGeneratorPoint(self):
        genPoint = ECPoint(self.nid)
        genPoint.value = PyECPoint.GetGenerator(self.value, self.nid);
        return genPoint
        
    def multiplyByGenerator(self, m):
        GenPoint = self.GetGeneratorPoint();
        ecPointvalue = GenPoint.multipleScalar(m)
        return ecPointvalue
        
    def __str__(self):
        return self.value

def MultiplyByGenerator(m, nid=714):
    pt = ECPoint(nid)
    return pt.multiplyByGenerator(m)
    
        
class ECKey256K1:
    def __init__ (self):
        self.pubKey, self.priKey = PyAsymKey.GenerateKeyPairPEM(); 
    
    def FromPEMStr (self, keyPemForm):
        self.pubKey, self.priKey = PyAsymKey.ImportFromPem(keyPemForm);
        pass

    def FromEncryptedPEMStr (self, keyPemForm, passPhrase):
        self.pubKey, self.priKey = PyAsymKey.ImportFromEncryptedPEM(keyPemForm, passPhrase);
        pass

    def ToEncryptedPEMStr (self, passPhrase):
        return PyAsymKey.ExportFromEncryptedPEM(passPhrase);
            

    def derivePrivateKey(self,msg):
        return PyAsymKey.DerivePrivate(self.priKey, msg)
        
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
        self.degree = degree
        self.modulo = modulo
        self.coefficients = []
        self.isDec = True

    @staticmethod
    def isValidHex (s) :
        return all(c in string.hexdigits for c in s)

    @staticmethod
    def isValidDec (s) :
        return all(c in string.digits for c in s)

    @classmethod
    def initRandomDec(cls, degree, modulo):
        if Polynomial.isValidDec( modulo ) :
            obj = cls(degree, modulo)
            obj.isDec = True
            obj.coefficients = PyPolynomial.randomPolynomial(degree, modulo, True);   
            return obj
        else :
            raise Exception( 'modulo is not a valid decimal string.  modulo: {}'.format(modulo) )   
    
    @classmethod
    def initRandomHex(cls, degree, modulo):
        if Polynomial.isValidHex( modulo ) :
            obj = cls(degree, modulo)
            obj.isDec = False
            obj.coefficients = PyPolynomial.randomPolynomial(degree, modulo, False);   
            return obj
        else :
            raise Exception( 'modulo is not a valid hexadecimal string.  modulo: {}'.format(modulo) )            

    @classmethod
    def initRandomFixed_a_0_Dec(cls, degree, modulo, a_0):
        if Polynomial.isValidDec( modulo ) and Polynomial.isValidDec( a_0 ) :
            obj = cls(degree, modulo )
            obj.isDec = True
            obj.coefficients = PyPolynomial.randomPolynomialFixed_a_0 \
                (degree, modulo, a_0, True)
            return obj
        else :
            raise Exception( 'modulo and/or a_0 are not valid decimal strings.  modulo: {0}, a_0 {1}'.format(modulo, a_0)  )

    @classmethod
    def initRandomFixed_a_0_Hex(cls, degree, modulo, a_0):
        if Polynomial.isValidHex( modulo ) and Polynomial.isValidHex( a_0 ) :
            obj = cls(degree, modulo )
            obj.isDec = False
            obj.coefficients = PyPolynomial.randomPolynomialFixed_a_0 \
                (degree, modulo, a_0, False)
            return obj
        else :
            raise Exception( 'modulo and/or a_0 are not valid decimal strings.  modulo: {0}, a_0 {1}'.format(modulo, a_0)  )       

    @classmethod
    def initRandomMinMaxDec(cls, degree, modulo, min, max):
        if Polynomial.isValidDec( modulo ) and Polynomial.isValidDec( min ) and Polynomial.isValidDec( max ) :
            obj = cls( degree, modulo )
            obj.isDec = True
            obj.coefficients = PyPolynomial.randomPolynomialMinMax \
                (degree, modulo, min, max, True)            
            return obj
        else :
            raise Exception( 'modulo and/or min and/or max are not valid decimal strings.  modulo: {}, min: {}, max: {}'.format(modulo, min, max) )            

    @classmethod
    def initRandomMinMaxHex(cls, degree, modulo, min, max):
        if Polynomial.isValidHex( modulo ) and Polynomial.isValidHex( min ) and Polynomial.isValidHex( max ) :
            obj = cls( degree, modulo )
            obj.isDec = False
            obj.coefficients = PyPolynomial.randomPolynomialMinMax \
                (degree, modulo, min, max, False)            
            return obj
        else :
            raise Exception( 'modulo and/or min and/or max are not valid hexadecimal strings.  modulo: {}, min: {}, max: {}'.format(modulo, min, max) ) 

    @classmethod
    def initFromListDec( cls, coeffs ) :
        valid = True 
        for coefficient in coeffs :
            if not Polynomial.isValidDec( coefficient ) :
                valid = False 
        if valid :
            obj = cls(len(coeffs)-1, "0") 
            obj.coefficients = coeffs ; 
            obj.isDec = True
            return obj
        else :
            raise Exception( 'one (or more) coefficients is not a valid decimal string. coefficients: {}'.format(coeffs) )

    @classmethod
    def initFromListHex( cls, coeffs ) :
        valid = True 
        for coefficient in coeffs :
            if not Polynomial.isValidHex( coefficient ) :
                valid = False 
        if valid :
            obj = cls(len(coeffs)-1, "0") 
            obj.coefficients = coeffs ; 
            obj.isDec = False
            return obj
        else :
            raise Exception( 'one (or more) coefficients is not a valid hexadecimal string. coefficients: {}'.format(coeffs) )

    @classmethod
    def initFromListModuloDec( cls, coeffs, modulo ) :
        valid = True 
        for coefficient in coeffs :
            if not Polynomial.isValidDec( coefficient ) :
                valid = False 
        if valid :
            obj = cls(len(coeffs)-1, modulo) 
            obj.coefficients = coeffs ; 
            obj.isDec = True
            return obj
        else :
            raise Exception( 'one (or more) coefficients is not a valid hexadecimal string. coefficients: {}'.format(coeffs) )

    @classmethod
    def initFromListModuloHex( cls, coeffs, modulo ) :
        valid = True 
        if not Polynomial.isValidHex( modulo ) :
            valid = False
        for coefficient in coeffs :
            if not Polynomial.isValidHex( coefficient ) :
                valid = False 
        if valid :
            obj = cls(len(coeffs)-1, modulo) 
            obj.coefficients = coeffs ; 
            obj.isDec = False
            return obj
        else :
            raise Exception( 'one (or more) parameters is not a valid hexadecimal string. coefficients: {}, modulo: {}'.format(coeffs, modulo) )


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
        if self.isDec :
            if Polynomial.isValidDec( x ) :
                return PyPolynomial.evaluate(self.coefficients, x, self.modulo, True)
            else :
                raise Exception( 'Polynomial is set to be Decicimal, but x is not a valid decimal string: {}'.format(x) )      
        else :
            if Polynomial.isValidHex( x ) :
                return PyPolynomial.evaluate(self.coefficients, x, self.modulo, False)
            else :
                raise Exception( 'Polynomial is set to be Hexadecimal, but x is not a valid hexadecimal string: {}'.format(x) )      

class LGInterpolator:
    def __init__(self, xfx, modulo, decimal ):
        self.points = xfx
        self.modulo = modulo
        self.isDec = decimal

    def __call__(self, xValue, basisValue=None) :
        if basisValue is None :
            return PyPolynomial.LGInterpolatorFull( self.points, self.modulo, xValue, self.isDec )

        return PyPolynomial.LGInterpolatorSingle( self.points, self.modulo, xValue, basisValue, self.isDec )
        

    def __str__(self):
        return "points: {0}, modulo: {1}".format (self.points, self.modulo)
        
class LGECInterpolator:
    def __init__(self, xfx, modulo ):
        self.points = xfx
        self.modulo = modulo

    def __call__(self, xValue, basisValue=None) :
        retval= PyPolynomial.LGECInterpolatorFull(self.points, self.modulo, xValue)
        retObj = ECPoint()
        retObj.value = PyPolynomial.LGECInterpolatorFull(self.points, self.modulo, xValue)
        return retObj
        

    def __str__(self):
        return "points: {0}, modulo: {1}".format (self.points, self.modulo)

