import json
import Nakasendo
import sys


## return the vector sum of columns of a matrix
def calc_vector_sum_col(vect, modulo=None, dec=False):
    vect_sum_col = Nakasendo.BigNum('0',modulo,dec)
    firstEntry = Nakasendo.BigNum(vect[0][1],modulo,dec)
    vect_sum_col = vect_sum_col + firstEntry
    
    for ords in vect[1:]:
        vectVal = Nakasendo.BigNum(ords[1],modulo,dec)
        vect_sum_col = vect_sum_col + vectVal
        
    return vect_sum_col
    

class Player:

    def __init__(self, URI="", Ordinal=-1, Accepted = False, Rejected = False):
        self.URI = URI
        self.Ordinal = Ordinal
        self.Accepted = Accepted
        self.Rejected = Rejected
        self.KeyShareSecret = None

    def CreatePolynomial(self,degree, mod, decimal=False):
        if(decimal):
            self.polynomial = Nakasendo.Polynomial.initRandomDec(degree,mod)
        else:
            self.polynomial = Nakasendo.Polynomial.initRandomHex(degree,mod)

    
    def __eq__(self, other): 
        if not isinstance(other, Player):
            # don't attempt to compare against unrelated types
            return NotImplemented
        return self.Ordinal == other.Ordinal and self.URI == other.URI
        
    def getURI(self):
        return self.URI

    def setURI(self, URI):
        self.URI = URI

    def getOrdinal(self):
        return self.Ordinal

    def setOrdinal(self, Ordinal):
        self.Ordinal = Ordinal

    def accepted(self):
        return self.Accepted

    def setAccepted(self):
        if (self.Rejected == True):
            raise RuntimeError('Attempt to set player as both accepted and rejected')
        self.Accepted = True

    def rejected(self):
        return self.Rejected

    def setRejected(self):
        if (self.Accepted == True):
            raise RuntimeError('Attempt to set player as both accepted and rejected')
        self.Rejected = True

    def evaluatePolys(self,playerids,modulo=None,isDec=False):
        vect_ord_x=[]
        for i in playerids:
            if(i != self.Ordinal):
                x_i = self.polynomial(str(i))
                vect_ord_x.append((i,x_i))
            
        return vect_ord_x
        
        
    def CreateSecretFromSharedData(self,vect_ord_x,modulo=None,isDec=False):
        #evaluate my own portion
        xStr = self.polynomial(str(self.Ordinal))
        x = Nakasendo.BigNum(xStr,modulo,isDec)
        vect_sum = calc_vector_sum_col(vect_ord_x,modulo,isDec)
        self.KeyShareSecret = vect_sum + x
        
        
    
    def __eq__(self, other):
        if not isinstance(other, Player):
            return False
        return self.URI == other.URI and str(self.Ordinal) == str(other.Ordinal) and str(self.Accepted) == str(other.Accepted) and str(self.Rejected) == str(other.Rejected)


    def to_json(self):
        plDt = {}
        plDt["URI"] = self.getURI()
        plDt["Ordinal"] = self.getOrdinal()
        plDt["Accepted"] = self.accepted()
        plDt["Rejected"] = self.rejected()
        return json.dumps(plDt)

    def __str__(self):
        return  "URI : "+self.URI + "\nOrdinal:" + str(self.Ordinal) +  "\nAccepted:" + str(self.Accepted) + "\nRejected:" + str(self.Rejected)

    def from_json(self, playerStr):
        playerDict = json.loads(playerStr)
        self.setURI(playerDict["URI"])
        self.setOrdinal(playerDict["Ordinal"])
        if (playerDict["Accepted"]):
            self.setAccepted()
        if (playerDict["Rejected"]):
            self.setRejected()
        return self

'''
new_player = Player("p1", 1, False, False)
data=new_player.to_json()
print(data)
p  = Player()
p.from_json(data)
print(p)
'''
