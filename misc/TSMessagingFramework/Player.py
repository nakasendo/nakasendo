import json
import Nakasendo
import sys
import ecdsa


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
            + "\n\tlittleK              =  " + str(self.littleK) \
            + "\n\talpha                =  " + str(self.alpha) )

        return string


    def reset( self ) :
        print("JVRSS: reset")
        self.f_x   = None           # f(x): Polynomial evaluated for own ordinal
        self.evals = {}             # dict ordinal:evaluated for each o in ordinallist
        self.publicEvals = {}       # dict of dict: all Players in group evaluations
        self.hiddenEvals = {}       # Polynomial evaluations multiplied by generator point
        self.hiddenPolynomial = []  # coeffs multiplied by generator point
        self.allHiddenPolynomials = {}
        self.littleK = None         # little k (part of ephemeral key calc)
        self.alpha   = None         # blinding value (part of ephemeral key calc)           


# Player Group metadata
class PlayerGroupMetadata :
    

    def __init__ (self, id, ordinal, ordinalList, degree) :
        print ("__init__ PlayerGroupMetadata")
        self.id                     = id            # Group ID
        self.ordinal                = ordinal       # Label assigned by orchestrator
        self.ordinalList            = ordinalList   # labels of other participants in the group
        self.degree                 = degree        # degree of the polynomial
        self.privateKeyPolynomial   = None          # Polynomial for this group
        self.privateKeyShare        = None          # calculated share of secret
        self.ephemeralKeyList       = []            # list of generated ephemeral keys

        self.transientData          = JVRSS()       # transient data - reusable data structure




    def __str__(self):
        string =  ("Player Metadata for " + str(self.id) + " :" \
            + "\n\tordinal              =  " + str(self.ordinal) \
            + "\n\tordinalList          =  " + str(self.ordinalList)  \
            + "\n\tdegree               =  " + str(self.degree)  \
            + "\n\tprivateKeyPolynomial =  " + str(self.privateKeyPolynomial) \
            + "\n\tprivateKeyShare      =  " + str(self.privateKeyShare) \
            + "\n\tephemeralKeyList     =  " + str(self.ephemeralKeyList) \
            + "\n\ttransientData        =  " + str(self.transientData ) )
            
        return string

    # Method to do pre-calculation setup
    # i.e. Using the Polynomial parameter, store calculated data in transient data
    #       - Evaluate the polynomial for own ordinal
    #       - Evaluate the polynomial for other player ordinals
    #       - Hide (encrypt) the polynomial via Generator Point
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

    # reusable code to create a secret - used for privateKeyShare, little-k, alpha
    def createSecret( self, ordinal ) :
        
        res = Nakasendo.BigNum(str( self.transientData.f_x))
        # loop outer dictionary (keyed  on ordinal)
        for outerOrd, dict2 in self.transientData.publicEvals.items() :
            for innerOrd, fx in dict2.items() :
                if innerOrd == ordinal :
                    res += Nakasendo.BigNum(str(fx))
        return res

#-----------------------------------------------------------------
# Error class 
class PlayerError(Exception):
    """This is an Expected Exception. Something bad happened."""
    pass

#-----------------------------------------------------------------
# Player class
class Player :

    modulo = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"
    #modulo = ecdsa.SECP256k1.order


    def __init__(self) :
        print("__init__ Player")
        # dictionary of { groupID:PlayerGroupMetadata }
        self.groups = {}
 
    def checkGroup(self, groupId) :
        if groupId in self.groups :
            return 1
        else :
            return 0


    


    #-------------------------------------------------
    # Add Group: This creates a polynomial, 
    #            Evaluates the polynomial for own ordinal,
    #            Evaluates the polynomial for all other Player ordinals,
    #            Hides (encrypts) the polynomial using the Generator point
    #            Hides (encrypts) the evals using using the Generator point
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
            print("Creating New Polynomial")
            poly = self.createPolynomial(group.degree)
            group.polynomialPreCalculation(poly, Player.modulo, group.ordinal)

        ordinal     = group.ordinal 
        evals       = group.transientData.evals
        hiddenPoly  = group.transientData.hiddenPolynomial
        hiddenEvals = group.transientData.hiddenEvals

        return [groupId, calcType, ordinal, evals, hiddenPoly, hiddenEvals]


    #-------------------------------------------------
    # create a secret - used to create a privateKeyShare, little-k, alpha
    def createSecret(self, groupId, calcType, evals, hiddenPolys, hiddenEvals) :

        print("creating a secret....")
        group = self.groups[groupId]
        
        group.transientData.publicEvals             = evals 
        group.transientData.allHiddenPolynomials    = hiddenPolys

        result = group.createSecret( group.ordinal )
        if calcType == 'PRIVATEKEYSHARE' :
            group.privateKeyShare = result 
        elif calcType == 'LITTLEK' :
            group.transientData.littleK = result
        elif calcType == 'ALPHA' :
            group.transiendData.alpha = result
        else:
            msg = "Player:createSecret.calcType is not recognised: {0}".format(calcType)
            print(msg)
            raise PlayerError(msg)
       
        #print("\nHidden Polynomials = {0}".format(group.transientData.allHiddenPolynomials))
        #print(group)

        self.verificationOfHonesty(groupId, hiddenEvals, hiddenPolys)

        return groupId, result

    #-------------------------------------------------
    # Do verification of honesty step
    def verificationOfHonesty(self, groupId, hiddenEvals, hiddenPolys) :
        group = self.groups[groupId]

        print("\nVerification : groupId = {0}".format(groupId))

        ordinalList = list(group.ordinalList)
        ordinalList.append(group.ordinal)
        for fromPlayerOrdinal in ordinalList :
            for toPlayerOrdinal in ordinalList :
                if (fromPlayerOrdinal != toPlayerOrdinal):
                    if (not self.getVerifyCoefficientForPlayer(groupId, hiddenEvals, hiddenPolys, fromPlayerOrdinal, toPlayerOrdinal)):
                        msg =  ("Verification of honesty "+ str(fromPlayerOrdinal) + " to " + str(toPlayerOrdinal)+ " is failed.")
                        print(msg)
                        raise PlayerError(msg)

    #-------------------------------------------------
    def getVerifyCoefficientForPlayer(self, groupId, hiddenEvals, hiddenPolys, fromOrdinal, toOrdinal):

        # get the coefficients that 'from' player sent to all, where 'from' and 'to' are ordinals
        resCoeffEC = None
        multplier = toOrdinal

        for coeffEC in hiddenPolys[fromOrdinal]:
            coeffEC = Player.getECPoint(coeffEC)
            if (resCoeffEC is None) :
                resCoeffEC = coeffEC
            else:
                resCoeffEC = resCoeffEC + coeffEC.multipleScalar(Nakasendo.BigNum(str(multplier)))
                multplier = (multplier * toOrdinal)

        for ofOrdinal, pubPoly in hiddenEvals[fromOrdinal].items():
            pubPoly = Player.getECPoint(pubPoly)
            if (ofOrdinal == toOrdinal):
               if (pubPoly == resCoeffEC):
                   return True
               else:
                   return False

        return False

    @staticmethod
    def getECPoint(value):
        ecpoint = Nakasendo.ECPoint()
        ecpoint.SetValue(value)
        return ecpoint

            
    #-------------------------------------------------
    def __str__(self):
        string = "Player:\nNumber of groups = " + str(len(self.groups))

        for values in self.groups.values() :
            string += '\n'
            string += str(values)

        return string 

