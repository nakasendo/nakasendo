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
import PyBSVAddress

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

    def HashSha256(self):
        return PyMessageHash.HashMsgSHA256(self.message)
        
    def Hash(self,hashfunc):
        return PyMessageHash.HashMsg(self.message,hashfunc)
        
    def ListHash(self):
        return PyMessageHash.ListHash()
        
    def __str__(self):
        return '{}'.format (self.message);
        
def hash256(message,modulo=None, isdecimal=False):
    MH = MessageHash(message)
    hashedVal = MH.HashSha256()
    retVal = BigNum(value=hashedVal,mod=modulo,isDec=isdecimal)
    return retVal
    
def hash(message,hashfunc=None,modulo=None,isdecimal=False):
    MH = MessageHash(message)
    hashedVal = MH.Hash(hashfunc)
    retVal = BigNum(value=hashedVal,mod=modulo,isDec=isdecimal)
    return retVal
    
def ListHashFuncs():
    return PyMessageHash.ListHash()
    
        
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
    def __init__ (self, value=None, mod=None,isDec=False):
        self.mod = mod
        self.isDec = isDec

        if value is None:
            if(self.isDec):
                self.value = PyBigNumbers.GenerateRandDec(256)
            else:
                self.value = PyBigNumbers.GenerateRandHex(256)
        else:
            self.value = value
            
    def __add__(self, obj):
        if(self.isDec):
            if(self.mod is None):
                sumVal = PyBigNumbers.addFromDec(self.value, obj.value) 
            else:
                #check for self.mod == obj.mod           
                sumVal = PyBigNumbers.Add_mod_Dec(self.value, obj.value, self.mod)
            retVal = BigNum(sumVal,self.mod,self.isDec)
            return retVal ;  
        else:
            if(self.mod is None):
                sumVal = PyBigNumbers.addFromHex(self.value, obj.value) 
            else:
                #check for self.mod == obj.mod           
                sumVal = PyBigNumbers.Add_mod_Hex(self.value, obj.value, self.mod)
                
            retVal = BigNum(sumVal,self.mod,self.isDec)
            return retVal ; 
    
    def __sub__(self,obj):
        if(self.isDec):
            if(self.mod is None):
                diffVal = PyBigNumbers.subFromDec(self.value, obj.value) 
            else:
                diffVal = PyBigNumbers.Sub_mod_Dec(self.value, obj.value,self.mod)
            retVal = diffVal(sumVal,self.mod,self.isDec)
            return retVal ; 
            
        else:
            if(self.mod is None):
                diffVal = PyBigNumbers.subFromHex(self.value, obj.value) 
            else:
                diffVal = PyBigNumbers.Sub_mod_Hex(self.value, obj.value,self.mod)
            retVal = BigNum(diffVal,self.mod,self.isDec)
            return retVal ; 
            
    def __mul__(self,obj):
        if(self.isDec):
            if(self.mod is None):
                prodVal = PyBigNumbers.multiplyFromDec(self.value, obj.value)
            else:
                #print ('%s * %s mod %s' % (self.value, obj.value, self.mod))
                prodVal=PyBigNumbers.Mul_mod_Dec(self.value, obj.value, self.mod)
            
            retVal = BigNum(prodVal, self.mod,self.isDec)
            return retVal
        else:
            if(self.mod is None):
                prodVal = PyBigNumbers.multiplyFromHex(self.value, obj.value)
            else:
                #print ('%s * %s mod %s' % (self.value, obj.value, self.mod))
                prodVal=PyBigNumbers.Mul_mod_Hex(self.value, obj.value, self.mod)
            
            retVal = BigNum(prodVal, self.mod,self.isDec)
            return retVal
        
    def __truediv__(self,obj):
        if(self.isDec):
            if (self.mod is None):
                divVal = PyBigNumbers.divideFromDec(self.value,obj.value)
            else:
                divVal = PyBigNumbers.Div_mod_Dec(self.value, obj.value,self.mod)
                
            retVal = BigNum(divVal, self.mod,self.isDec)
        else:
            if (self.mod is None):
                divVal = PyBigNumbers.divideFromHex(self.value,obj.value)
            else:
                divVal = PyBigNumbers.Div_mod_Hex(self.value, obj.value,self.mod)
                
            retVal = BigNum(divVal, self.mod,self.isDec)
            return retVal
            
    def __gt__(self,obj):
        return PyBigNumbers.isGreater(self.value, obj.value, self.isDec)
        
        
    def __eq__(self,obj):
        if (self.isDec == obj.isDec):
            if(PyBigNumbers.isEqual(self.value,obj.value,self.isDec)):
                if(self.mod is None and obj.mod is None):
                    return True
                if((self.mod is None and obj.mod is not None) or (self.mod is not None and obj.mod is None)):
                    return False
                if(PyBigNumbers.isEqual(self.mod, obj.mod, self.isDec)):
                    return True
                else:
                    return False
            else:
                return False
        else:
            return False
    
    def __str__(self):
        return self.value
        
    def __repr__(self):
        return self.value

        
    def inverse(self):
        if(self.isDec):
            if(self.mod is None):
                numerator = BigNumber('1',self.mod,self.isDec)
                inverseVal = numerator / self
            else:
                inverseVal = PyBigNumbers.Inv_mod_Dec(self.value, self.mod)
            retVal = BigNum(inverseVal,self.mod,self.isDec)
            return retVal
        else:
            if(self.mod is None):
                numerator = BigNumber('1',self.mod)
                inverseVal = numberator % self
            else:
                inverseVal = PyBigNumbers.Inv_mod_Hex(self.value, self.mod)
            retVal = BigNum(inverseVal,self.mod,self.isDec)
            return retVal
    

class ECPoint:

    # class static - Value for NID_secp256k1 
    defaultNID = 714


    def __init__ (self, nid=0, isDec=False, isCompressed=True ) :
        self.isDec          = isDec
        self.isCompressed   = isCompressed

        if( nid == 0) :
            self.nid = self.defaultNID 
        else :
            self.nid = nid

        self.isDec = isDec
        self.value = PyECPoint.GenerateRandomEC( self.nid, self.isDec, self.isCompressed )            
    
        # self-check for valid value
        if isDec :
            if not ECPoint.isValidDec( self.value ) :
                raise Exception( 'ECPoint value is not a valid decimal string.  value: {}'.format( self.value) )                            
        else :
            if not ECPoint.isValidHex( self.value ) :
                raise Exception( 'ECPoint value is not a valid hexadecimal string.  value: {}'.format( self.value) )                            

    @staticmethod
    def isValidHex (s) :
        return all(c in string.hexdigits for c in s)

    @staticmethod
    def isValidDec (s) :
        return all(c in string.digits for c in s)

    def __add__(self, obj):
        if (self.nid != obj.nid):
            print ("Points not on the same curve %i and %i" % (self.nid, obj.nid))
            return None

        sumVal = PyECPoint.Add( self.value, obj.value, self.nid, self.isDec, self.isCompressed )
        ecpRetVal = ECPoint(self.nid,self.isDec)
        ecpRetVal.SetValue(sumVal)
        return ecpRetVal 

    def __eq__ (self, obj):
        if ( self.nid == obj.nid):
            return ( PyECPoint.Compare( self.value, obj.value, self.nid, self.isDec ) )  
        else:
            return False

    def multipleScalar(self,objm):
        mulVal = PyECPoint.MultiplyScalarM( self.value, objm.value, self.nid, self.isDec, self.isCompressed )
        ecpRetVal = ECPoint(self.nid,self.isDec)
        ecpRetVal.SetValue(mulVal)
        return ecpRetVal

    def multipltScalarEx(self, objm, objn):
        mulVal = PyECPoint.MultiplyScalarMN( self.value, objm.value, objn.value, self.nid, self.isDec, self.isCompressed )        
        ecpRetVal = ECPoint(self.nid)
        ecpRetVal.SetValue (mulVal)
        return ecpRetVal

    def GetAffineCoOrdinates(self):
        points = PyECPoint.GetAffineCoOrdinates( self.value, self.nid, self.isDec )
        return points
        
    def x(self):
        points = self.GetAffineCoOrdinates( )
        return points[0]
        
    def SetValue(self, value) :
        # self-check for valid value
        if self.isDec :
            if not ECPoint.isValidDec( value ) :
                raise Exception( 'Value is not a valid decimal string.  value: {}'.format( self.value) )                            
        else :
            if not ECPoint.isValidHex( value ) :
                raise Exception( 'Value is not a valid hexadecimal string.  value: {}'.format( self.value) )                            

        self.value = value; 

    def IsPointOnCurve (self):
        if(PyECPoint.CheckOnCurve(self.value, self.nid, self.isDec ) ) :
            return True
        return False
        
    def GetGeneratorPoint(self):
        genPoint = ECPoint(self.nid,self.isDec)
        genPoint.value = PyECPoint.GetGenerator(self.value, self.nid, self.isDec, self.isCompressed)
        return genPoint
        
    def GetOrder (self):
        order = PyECPoint.GetGroupOrder(self.value, self.nid, self.isDec )
        return order
    
    def GetDegree(self):
        deg = PyECPoint.GetGroupDegree(self.value, self.nid, self.isDec )
        return deg
        
    def __str__(self):
        return self.value
        
    def __repr__(self):
        points = self.GetAffineCoOrdinates()
        return '({},{})'.format (points[0],points[1])

def MultiplyByGenerator( m,isDec=False, nid=ECPoint.defaultNID, compressed=False ) :
    pt = ECPoint(nid=nid,isDec=isDec)
    pt.value = PyECPoint.MultiplyByGenerator( m.value, nid, isDec, compressed)
    return pt


    
        
class ECKey256K1:
    def __init__ (self,asHex=False):
        if(asHex):
            self.pubKey, self.priKey = PyAsymKey.GenerateKeyPairHEX(); 
        else:
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

def verifyPubKeyHex(msg,pubkey,rval,sval):
    pt = ECPoint()
    pt.value = pubkey
    coords = pt.GetAffineCoOrdinates()
    pempubkey = pubKeyHexPtasPem(coords[0],coords[1])
    return PyAsymKey.Verify(msg,pempubkey,rval,sval)
    

def verifyDER(msg, pubkey, DERSig,isDec=False):
    return PyAsymKey.VerifyDER(msg, pubkey,DERSig,isDec )
    
def createDERFormat(rValue, sValue):
    assert(rValue.isDec == sValue.isDec)
    hexSig = PyAsymKey.DERSignature(rValue.value, sValue.value, rValue.isDec)
    hexSigBN = BigNum(hexSig, rValue.mod, rValue.isDec)
    return hexSigBN

def pubKeyPEMasHex(pubkey, compressed=False):
    return PyAsymKey.PubKeyPEMToHexPt(pubkey, compressed)
    
def pubKeyPEMasHex(pubkey, compressed=False):
    return PyAsymKey.PubKeyPEMToHexPt(pubkey, compressed)

def pubKeyHexPtasPem(xPt,yPt, nid=714):
    return PyAsymKey.PubKeyHexPtToPEM(xPt,yPt,nid)


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
            obj.modulo = modulo
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
            obj.modulo = modulo
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
            val = PyPolynomial.LGInterpolatorFull( self.points, self.modulo, xValue, self.isDec )
            retVal = BigNum(val,self.modulo,self.isDec)
            return retVal
            #return PyPolynomial.LGInterpolatorFull( self.points, self.modulo, xValue, self.isDec )
        
        val = PyPolynomial.LGInterpolatorSingle( self.points, self.modulo, xValue, basisValue, self.isDec )
        retVal = BigNum(val,self.modulo,self.isDec)
        return retVal
        #return PyPolynomial.LGInterpolatorSingle( self.points, self.modulo, xValue, basisValue, self.isDec )
        

    def __str__(self):
        return "points: {0}, modulo: {1}".format (self.points, self.modulo)
        
class LGECInterpolator:
    def __init__(self, xfx, modulo,decimal):
        self.points = xfx
        self.modulo = modulo
        self.decimal = decimal

    def __call__(self, xValue, basisValue=None) :
        retObj = ECPoint()
        retObj.isDec = self.decimal
        retObj.value = PyPolynomial.LGECInterpolatorFull(self.points, self.modulo, xValue,self.decimal)
        return retObj
        

    def __str__(self):
        return "points: {0}, modulo: {1}".format (self.points, self.modulo)


class BSVAddress:
    def __init__(self, key, version, l=None ):
        self.key = key
        self.version = version

        if l : list = l
        else : list = PyBSVAddress.createAddress(key, version)

        self.address            = list[0] 
        self.valid              = list[1] 
        self.decodedAddress     = list[2]
        self.prefix             = list[3]
        self.NetworkType        = list[4]


    @classmethod
    def initFromAddress( cls, BSVAddress ) :
        list = PyBSVAddress.importAddress( BSVAddress )

        if list :
            obj = cls( "", "", list )
            return obj
  
        else :
            raise Exception( 'Address: ' + BSVAddress + ' is not valid'  )  


    def __str__(self):
        prettyStr = PyBSVAddress.print( self.address )
        return prettyStr
