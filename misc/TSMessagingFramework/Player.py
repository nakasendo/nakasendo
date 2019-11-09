import json
import sys

try:
    import PyBigNumbers
    import PyECPoint
    import PySymEncDec
    import PyMessageHash
    import PyAsymKey
    import PyPolynomial
    import Nakasendo
except ImportError as e:
    print('Error while loading SDKLibraries python modules {}'.format(e.message))
    print('Try to define environment variable SDKLIBRARIES_ROOT pointing to the location of installed SDKLibraries or add this to PYTHONPATH')
    raise ImportError('Unable to load SDKLibraries python modules')


# JVRSS class for transient data
class JVRSS :
    def __init__ ( self ) :
        self.reset( )

    def __str__(self):
        string =  ("JVRSS :" \
            + "\n\tf_x              =  " + str(self.f_x)  \
            + "\n\tevals            =  " + str(self.evals)  \
            + "\n\tpublicEvals      =  " + str(self.publicEvals)  \
            + "\n\thiddenEvals      =  " + str(self.hiddenEvals)  \
            + "\n\thiddenPolynomial =  " + str(self.hiddenPolynomial) \
            + "\n\tallHiddenPolynomials =  " + str(self.allHiddenPolynomials) \
            + "\n\tallVWshares          =  " + str(self.allVWshares) )



        return string


    def reset( self ) :
        #print("JVRSS: reset")
        self.f_x   = None           # f(x): Polynomial evaluated for own ordinal
        self.evals = {}             # dict ordinal:evaluated for each o in ordinallist
        self.publicEvals = {}       # dict of dict: all Players in group evaluations
        self.hiddenEvals = {}       # Polynomial evaluations multiplied by generator point
        self.hiddenPolynomial = []  # coeffs multiplied by generator point
        self.allHiddenPolynomials = {}
        self.allVWshares = {}       # dict of V W shares from all players
          


# Player Group metadata
class PlayerGroupMetadata :
    

    def __init__ (self, id, ordinal, ordinalList, degree) :
        self.id                     = id            # Group ID
        self.ordinal                = ordinal       # Label assigned by orchestrator
        self.ordinalList            = ordinalList   # labels of other participants in the group
        self.degree                 = degree        # degree of the polynomial
        self.privateKeyPolynomial   = None          # Polynomial for this group
        self.privateKeyShare        = None          # calculated share of secret
        self.publicKeyOfKeyShare    = None          # calculated public key of share of secret
        self.ephemeralKeyList       = []            # list of generated ephemeral keys
        self.littleK                = None          # little k (part of ephemeral key calc)
        self.alpha                  = None          # blinding value (part of ephemeral key calc)         
        self.presignInitiator       = False         # need to co-ordinate JVRSS     
        self.numberPresigns         = 1             # number presigns left to do (default is 1)

        self.transientData          = JVRSS()       # transient data - reusable data structure




    def __str__(self):
        string =  ("Player Metadata for " + str(self.id) + " :" \
            + "\n\tordinal              =  " + str(self.ordinal) \
            + "\n\tordinalList          =  " + str(self.ordinalList)  \
            + "\n\tdegree               =  " + str(self.degree)  \
            + "\n\tprivateKeyPolynomial =  " + str(self.privateKeyPolynomial) \
            + "\n\tprivateKeyShare      =  " + str(self.privateKeyShare) \
            + "\n\tpublicKeyOfKeyShare  =  " + str(self.publicKeyOfKeyShare) \
            + "\n\tephemeralKeyList     =  " + str(self.ephemeralKeyList) \
            + "\n\tlittleK              =  " + str(self.littleK) \
            + "\n\talpha                =  " + str(self.alpha) \
            + "\n\tpresignInitiator     =  " + str(self.presignInitiator)  \
            + "\n\tnumberPresigns       =  " + str(self.numberPresigns)  \
            + "\n\ttransientData        =  " + str(self.transientData ) )
            
        return string

    # Method to do pre-calculation setup
    # i.e. Using the Polynomial parameter, store calculated data in transient data
    #       - Evaluate the polynomial for own ordinal
    #       - Evaluate the polynomial for other player ordinals
    #       - Hide (encrypt) the polynomial via Generatreor Point
    #       - Hide (encrypt) the evals via Generator Point
    #
    def polynomialPreCalculation(self, poly, mod, ordinal ) :

        self.transientData.reset( )

        # evaluate polynomial for own ordinal
        # polynomial is set to Hex, so convert the ordinal to hex string
        self.transientData.f_x = poly('{:x}'.format(ordinal))
        bignum = Nakasendo.BigNum( self.transientData.f_x, mod )
        self.transientData.hiddenEvals[self.ordinal] = \
            str(Nakasendo.ECPoint().GetGeneratorPoint().multipleScalar(bignum))

        # evaluate polynomials for the group ordinals
        for ord in self.ordinalList :
            self.transientData.evals[ord] = poly('{:x}'.format(ord))
            bignum = Nakasendo.BigNum( self.transientData.evals[ord], mod )
            self.transientData.hiddenEvals[ord] = \
                str(Nakasendo.ECPoint().GetGeneratorPoint().multipleScalar(bignum))

        # hide own polynomial using generator point
        GEN         = Nakasendo.ECPoint()
        GENPOINT    = GEN.GetGeneratorPoint()

        for index in poly.coefficients :
            bignum  = Nakasendo.BigNum( index, mod )
            res     = GENPOINT.multipleScalar(bignum)
            self.transientData.hiddenPolynomial.append(res.value)
    
    #-------------------------------------------------
<<<<<<< HEAD
    def calculateVWshares( self, mod ) :
        print("in calculateVWshared")

        # littleK * alpha
        littleK = Nakasendo.BigNum( self.littleK, mod )
        alpha   = Nakasendo.BigNum( self.alpha, mod )

        v = littleK * alpha

        # hide own polynomial using generator point
        GEN         = Nakasendo.ECPoint()
        GENPOINT    = GEN.GetGeneratorPoint()        

        w           = GENPOINT.multipleScalar( alpha )        

        return v, w 

=======
>>>>>>> 68eecd5... SL-329 : collated and distributed VW shares
    # reusable code to create a secret - used for privateKeyShare, little-k, alpha
    def createSecret( self, ordinal,mod ) :
        
        res = Nakasendo.BigNum(str( self.transientData.f_x),mod)
        # loop outer dictionary (keyed  on ordinal)
        for outerOrd, dict2 in self.transientData.publicEvals.items() :
            for innerOrd, fx in dict2.items() :
                if innerOrd == ordinal :
                    res += Nakasendo.BigNum(str(fx),mod)
        return res

    #-------------------------------------------------
    def calculateShareOfVW( self, mod ) : 
 
        # littleK * alpha
        v = self.littleK * self.alpha

        # alpha * Generator Point
        GEN         = Nakasendo.ECPoint()
        GENPOINT    = GEN.GetGeneratorPoint()        
        w           = GENPOINT.multipleScalar( self.alpha )  

        return v.value, w.value

    #-------------------------------------------------
    def createPublicKey(self):
        public_key = None
        for ordinal, coeffList in self.transientData.allHiddenPolynomials.items():
            a0Point = Player.getECPoint(coeffList[0])
            if (public_key):
                public_key = public_key + a0Point
            else:
                public_key = a0Point
        return public_key
        

#-----------------------------------------------------------------
# Error class 
class PlayerError(Exception):
    """This is an Expected Exception. Something bad happened."""
    pass

#-----------------------------------------------------------------
# Player class
class Player :

    modulo = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"


    def __init__(self) :
        # dictionary of { groupID:PlayerGroupMetadata }
        self.groups = {}
 
    def checkGroup(self, groupId) :
        if groupId in self.groups :
            return 1
        else :
            return 0

    def setPresignInitiator( self, groupId, number ) :
        group = self.groups[groupId]
        
        group.presignInitiator = True 
        group.numberPresigns = number 
        print("setting presignInitiator to: {0}, number of ephemeral keys left to do: {1}".format \
            (group.presignInitiator, group.numberPresigns))

    def isPresignInitiator( self, groupId ) :
        return self.groups[groupId].presignInitiator


    def numberPresignsLeftToDo( self, groupId ) :
        group = self.groups[groupId]
        numberPresigns = group.numberPresigns - 1
        return numberPresigns
        

    


    #-------------------------------------------------
    # Add Group - create the PlayerGroupMetadata
    #           - create the Polynomial, and call pre-calculation
    def addGroup(self, groupId, ordinal, ordinalList, degree) :
        print("GroupIsSet:\n\tgroupId =  {0}\n\tmy ordinal = {1}\n\trest of ordinals = {2}".format \
            (groupId, ordinal, ordinalList))
 
        if groupId in self.groups :             
            errMsg = 'groupId = {0} already exists!'.format(groupId)
            print(errMsg)
            return 0 

        self.groups[groupId] = PlayerGroupMetadata(groupId, ordinal, ordinalList, degree)

        group = self.groups[groupId] 
        
        group.privateKeyPolynomial = self.createPolynomial(degree)
        group.polynomialPreCalculation( group.privateKeyPolynomial, Player.modulo, group.ordinal ) 

        return 1

    #-------------------------------------------------               
    def createPolynomial(self, degree, decimal=False) :
        if (decimal) :
            return Nakasendo.Polynomial.initRandomDec(degree, Player.modulo)
        else:
            return Nakasendo.Polynomial.initRandomHex(degree, Player.modulo)

    #------------------------------------------------- 
    # 1st time through JVRSS don't need to generate new polynomial
    # 2nd and 3rd times through JVRSS need to create new polynomial
    def requestData(self, groupId, calcType) :
        group = self.groups[groupId]

        poly = None
        # Not the 1st time through JVRSS sequence
        if calcType != 'PRIVATEKEYSHARE' :
            poly = self.createPolynomial(group.degree)
            group.polynomialPreCalculation(poly, Player.modulo, group.ordinal)

        ordinal     = group.ordinal 
        evals       = group.transientData.evals
        hiddenPoly  = group.transientData.hiddenPolynomial
        hiddenEvals = group.transientData.hiddenEvals

        return [groupId, ordinal, evals, hiddenPoly, hiddenEvals]


    #-------------------------------------------------
    # create a secret - used to create a privateKeyShare, little-k, alpha
    def createSecret(self, groupId, calcType, evals, hiddenPolys, hiddenEvals) :

        print("creating a secret....")
        group = self.groups[groupId]
        
        group.transientData.publicEvals             = evals 
        group.transientData.allHiddenPolynomials    = hiddenPolys

        result = group.createSecret(group.ordinal,Player.modulo)
        if calcType == 'PRIVATEKEYSHARE' :
            group.privateKeyShare = result 
            group.publicKeyOfKeyShare = group.createPublicKey()
        elif calcType == 'LITTLEK' :
            group.littleK = result
        elif calcType == 'ALPHA' :
            group.alpha = result
        else:
            msg = "Player:createSecret.calcType is not recognised: {0}".format(calcType)
            print(msg)
            raise PlayerError(msg)

        self.verificationOfHonesty(groupId, hiddenEvals, hiddenPolys)
        self.verifyCorrectness(groupId, hiddenEvals, hiddenPolys)

        return groupId, result

    def verifyCorrectness(self, groupId, hiddenEvals, hiddenPolys):

        group = self.groups[groupId]
        ordinalList = list(group.ordinalList)
        ordinalList.append(group.ordinal)
        for Ordinal in ordinalList :
            curvepoint = []
            for ofOrdinal, pubPoly in hiddenEvals[Ordinal].items():
                pubPoly = Player.getECPoint(pubPoly)
                x, y = pubPoly.GetAffineCoOrdinates()
                x, y = str(int(x, 16)), str(int(y, 16))
                curvepoint.append((str(ofOrdinal),  x, y))

            interpolator = Nakasendo.LGECInterpolator(xfx=curvepoint, modulo=str(int(self.modulo, 16)), decimal=1)
            zero_interpolator = interpolator(xValue='0')
            if (str(zero_interpolator) != str(int(hiddenPolys[Ordinal][0], 16))):
                msg =  ("Verification of Correctness "+ str(Ordinal) + " is failed.")
                raise PlayerError(msg)

    #-------------------------------------------------
    # Do verification of honesty step
    def verificationOfHonesty(self, groupId, hiddenEvals, hiddenPolys) :
        group = self.groups[groupId]

        ordinalList = list(group.ordinalList)
        ordinalList.append(group.ordinal)
        for fromPlayerOrdinal in ordinalList :
            for toPlayerOrdinal in ordinalList :
                if (fromPlayerOrdinal != toPlayerOrdinal):
                    if (not self.getVerifyCoefficientForPlayer(groupId, hiddenEvals, hiddenPolys, fromPlayerOrdinal, toPlayerOrdinal)):
                        msg =  ("Verification of honesty "+ str(fromPlayerOrdinal) + " to " + str(toPlayerOrdinal)+ " is failed.")
                        raise PlayerError(msg)

    #-------------------------------------------------
    '''
    def getVerifyCoefficientForPlayer(self, groupId, hiddenEvals, hiddenPolys, fromOrdinal, toOrdinal):

        # get the coefficients that 'from' player sent to all, where 'from' and 'to' are ordinals
        resCoeffEC = None
        multplier = toOrdinal
        
        

        for coeff in hiddenPolys[fromOrdinal]:
            coeffEC = Player.getECPoint(coeff)
            if (resCoeffEC is None) :
                resCoeffEC = coeffEC
            else:
                resCoeffEC = resCoeffEC + coeffEC.multipleScalar(Nakasendo.BigNum('{:x}'.format(multplier) ))
                multplier = (multplier * toOrdinal)

        for ofOrdinal, pubPoly in hiddenEvals[fromOrdinal].items():
            pubPoly = Player.getECPoint(pubPoly)
            if (ofOrdinal == toOrdinal):
               if (pubPoly == resCoeffEC):
                   return True
               else:
                   return False

        return False
    '''
    def getVerifyCoefficientForPlayer(self, groupId, hiddenEvals, hiddenPolys, fromOrdinal, toOrdinal):

        # get the coefficients that 'from' player sent to all, where 'from' and 'to' are ordinals
        resCoeffEC = None
        multplier = toOrdinal
        multplierBN = Nakasendo.BigNum(str(toOrdinal), self.modulo)
        toOrdinalBN = Nakasendo.BigNum(str(toOrdinal), self.modulo)
           
        encryptedCoeffs = hiddenPolys[fromOrdinal]
        resCoeffEC = Player.getECPoint(encryptedCoeffs[0])    
        for coeff in encryptedCoeffs[1:]:           
            coeffEC = Player.getECPoint(coeff)          
            labelTimesPoints = coeffEC.multipleScalar(multplierBN)          
            resCoeffEC = resCoeffEC + labelTimesPoints          
            multplierBN = multplierBN * toOrdinalBN
        

        for ofOrdinal, pubPoly in hiddenEvals[fromOrdinal].items():
            pubPoly = Player.getECPoint(pubPoly)
            if (ofOrdinal == toOrdinal):
               if (pubPoly == resCoeffEC):
                   return True
               else:
                   return False

        return False
    #-------------------------------------------------

    # get the calculate V and W share for this Player
    def getVWshares( self, groupId ) :       
        group = self.groups[groupId] 
 
        res = group.calculateShareOfVW( Player.modulo )
    
        return [group.ordinal, res]  

    # sets the collated VW Data for all players in group
    def setSharedVWData( self, groupId, data ) :
        group = self.groups[groupId]
        group.transientData.allVWshares = data

        self.calculateEphemeralKey( groupId )

    # LGInterplate at 0 to get V
    # ECLGInterplate at 0 to get W
    def calculateEphemeralKey(self, groupId ) :
        
        group = self.groups[groupId]
        print("Calculating Ephemeral Key...")

        xfx_v = []
        xfx_w = []

        allOrdinals = list(group.ordinalList)
        allOrdinals.append(group.ordinal)

        for ord in allOrdinals :
            vw = group.transientData.allVWshares[ord]
            xfx_v.append( (ord, vw[0]) )
            point = Nakasendo.ECPoint()
            point.SetValue( vw[1] )
            w_points = point.GetAffineCoOrdinates()

            xfx_w.append( (str(ord),  w_points[0],w_points[1] ) )
        

        v_interpolator = Nakasendo.LGInterpolator \
            ( xfx_v, Player.modulo, decimal=False)
        w_interpolator = Nakasendo.LGECInterpolator \
            ( xfx_w, Player.modulo, decimal=False)

        vZeroVal = v_interpolator('0')
        wZeroVal = w_interpolator('0')
        
        vZeroValInv = vZeroVal.inverse()

        interpolated_r = wZeroVal.multipleScalar(vZeroValInv)
        if ( interpolated_r.IsPointOnCurve() is not True ) :
            msg = ("Error in Player:calculateEphemeralKey: point not on curve")
            print(msg)
            raise PlayerError(msg)

        interpolated_r_points = interpolated_r.GetAffineCoOrdinates()

        r_bn = Nakasendo.BigNum( interpolated_r_points[0], Player.modulo )
        ephemeralKey = [ group.littleK , r_bn ]
        
        group.ephemeralKeyList.append( ephemeralKey )



    #-------------------------------------------------
    #-------------------------------------------------
    # Static Methods
    #-------------------------------------------------
    @staticmethod
    def getECPoint(value,isDecimal=False):
        ecpoint = Nakasendo.ECPoint()
        ecpoint.SetValue(value)
        return ecpoint

    
    def GroupIDs(self):
        keys = self.groups.keys()
        listToStr = '\n'.join([str(elem) for elem in keys])
        return listToStr 
    #-------------------------------------------------
    def __str__(self):
        string = "Player:\nNumber of groups = " + str(len(self.groups))

        for values in self.groups.values() :
            string += '\n'
            string += str(values)

        return string 

