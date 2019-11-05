import json
import Nakasendo
import sys
import ecdsa

# Player Group metadata
class PlayerGroupMetadata :

    def __init__ (self, id, ordinal, ordinalList, degree) :
        print ("__init__ PlayerGroupMetadata")
        self.id = id
        self.ordinal = ordinal             
        self.ordinalList = ordinalList  
        self.degree = degree
        self.polynomial = None      # own Polynomial for this group
        self.f_x   = None           # f(x): Polynomial evaluated for own ordinal
        self.evals = {}             # dict ordinal:evaluated for each o in ordinallist
        self.publicEvals = {}       # dict of dict: all Players in group evaluations
        self.hiddenEvals = {}  # Polynomial evaluated multiplied by generator point
        self.shareOfSecret = None
        self.hiddenPolynomial = []  # coeffs multiplied by generator point
        self.allHiddenPolynomials = None    # All Players Hidden coeffs

    def __str__(self):
        string =  ("Player Metadata for " + str(self.id) + " :" \
            + "\n\tordinal          =  " + str(self.ordinal) \
            + "\n\tordinalList      =  " + str(self.ordinalList)  \
            + "\n\tdegree           =  " + str(self.degree)  \
            + "\n\tpolynomial       =  " + str(self.polynomial)  \
            + "\n\tf_x              =  " + str(self.f_x)  \
            + "\n\tevals            =  " + str(self.evals)  \
            + "\n\tshareOfSecret    =  " + str(self.shareOfSecret)  \
            + "\n\thiddenPolynomial =  " + str(self.hiddenPolynomial))

          
            
        return string

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
    def addGroup(self, groupId, ordinal, ordinalList, degree) :
        print("GroupIsSet:\n\tgroupId =  {0}\n\tmy ordinal = {1}\n\trest of ordinals = {2}".format \
            (groupId, ordinal, ordinalList))
 
        if groupId in self.groups :             
            errMsg = 'groupId = {0} already exists!'.format(groupId)
            print(errMsg)
            return 0 

        self.groups[groupId] = PlayerGroupMetadata(groupId, ordinal, ordinalList, degree)

        group = self.groups[groupId] 
        group.polynomial = self.createPolynomial(degree)

        # evaluate polynomial for own ordinal
        # polynomial is set to Hex, so convert the ordinal to hex string
        group.f_x = group.polynomial('{:x}'.format(group.ordinal))
        bignum = Nakasendo.BigNum(group.f_x, Player.modulo)
        #group.hiddenEvals[group.ordinal] = Nakasendo.MultiplyByGenerator(bignum).value
        group.hiddenEvals[group.ordinal] = str(Nakasendo.ECPoint().GetGeneratorPoint().multipleScalar(bignum))

        # evaluate polynomials for the group ordinals
        for ord in group.ordinalList :
            group.evals[ord] = group.polynomial('{:x}'.format(ord))
            bignum = Nakasendo.BigNum(group.evals[ord], Player.modulo)
            #group.hiddenEvals[ord] = Nakasendo.MultiplyByGenerator(bignum).value
            group.hiddenEvals[ord] = str(Nakasendo.ECPoint().GetGeneratorPoint().multipleScalar(bignum))

        # hide own polynomial using generator point
        GEN         = Nakasendo.ECPoint()
        GENPOINT = GEN.GetGeneratorPoint()

        for index in group.polynomial.coefficients :
            bignum = Nakasendo.BigNum(index, Player.modulo)
            res = GENPOINT.multipleScalar(bignum)
            group.hiddenPolynomial.append(res.value)

        return 1

    #-------------------------------------------------               
    def createPolynomial(self, degree, decimal=False) :
        if (decimal) :
            return Nakasendo.Polynomial.initRandomDec(degree, Player.modulo)
        else:
            return Nakasendo.Polynomial.initRandomHex(degree, Player.modulo)

    #-------------------------------------------------
    def getOrdinal(self, groupId) :
        return self.groups[groupId].ordinal

    #-------------------------------------------------
    def getEvaluatedData(self, groupId) :
        group = self.groups[groupId]
        data = group.evals
        data[group.ordinal] = 0
        return data

    #-------------------------------------------------
    def getEvaluatedHiddenData(self, groupId) :
        group = self.groups[groupId]
        data = group.hiddenEvals
        return data
    
    #-------------------------------------------------
    def getHiddenPoly(self, groupId) :
        group = self.groups[groupId]
        data = group.hiddenPolynomial
        return data

    #-------------------------------------------------
    # change name to calucluate private key share ?
    def calculatePrivateKeyShare(self, groupId, evals, hidden, hiddenEvals) :
        group = self.groups[groupId]
        group.publicEvals = evals 

        myOrdinal = group.ordinal

        res = Nakasendo.BigNum(str(group.f_x))
        # loop outer dictionary (keyed  on ordinal)
        for outerOrd, dict2 in evals.items() :
            for innerOrd, fx in dict2.items() :
                if innerOrd == myOrdinal :
                    res += Nakasendo.BigNum(str(fx))

        group.shareOfSecret = res
        group.allHiddenPolynomials = hidden
       
        print("\nHidden Polynomials = {0}".format(group.allHiddenPolynomials))
        print(group)

        msg = self.verificationOfHonesty(groupId, hiddenEvals, hidden)

        return msg

    #-------------------------------------------------
    # Do verification of honesty step
    def verificationOfHonesty(self, groupId, hiddenEvals, hidden) :
        group = self.groups[groupId]

        print("\nVerification : groupId = {0}".format(groupId))

        ordinalList = group.ordinalList
        ordinalList.append(group.ordinal)
        for fromPlayerOrdinal in ordinalList :
            for toPlayerOrdinal in ordinalList :
                if (fromPlayerOrdinal != toPlayerOrdinal):
                    if (not self.getVerifyCoefficientForPlayer(groupId, hiddenEvals, hidden, fromPlayerOrdinal, toPlayerOrdinal)):
                        msg =  ("Verification of honesty "+ str(fromPlayerOrdinal) + " to " + str(toPlayerOrdinal)+ " is failed.")
                        return msg
        return ""

    def getVerifyCoefficientForPlayer(self, groupId, hiddenEvals, hidden, fromOrdinal, toOrdinal):

        # get the coefficients that 'from' player sent to all, where 'from' and 'to' are ordinals
        resCoeffEC = None
        multplier = toOrdinal

        for coeffEC in hidden[fromOrdinal]:
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

