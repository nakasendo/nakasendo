from PyMessage import Message
from PyUUID import UUID
from PyGroup import PlayerItem, PlayerMap, Group
from PyPlayer import Player
from PyGroupState import GroupState
from PyGroupSecretSetupMessage import GroupSecretSetupMessage
from PyGroupSecretResponseMessage import GroupSecretResponseMessage
import json
from threading import RLock
import random
import Polynomial
from PyPolynomials import PolynomialVals, GroupSecretPolynomialVals
from PyGroupBuilder import GroupBuilder
from PyGroupSetupMessage import GroupSetupMessage, GroupSetupResponseMessage, GroupBroadcastMessage
from PyGroupSecretIntermediary import IntermediaryMessages, GroupSetupIntermediaryMessage, GroupSetupResponseIntermediaryMessage, GroupBroadcastIntermediaryMessage, GroupSignatureShareMessage, GroupSecretPublicMessage
import ecdsa
import FiniteGroup
#import Nakasendo


Generator= ecdsa.SECP256k1.generator
Order = ecdsa.SECP256k1.order

class GroupSecretPlayerBuilder:
    #Construct for the given list of players.
    def __init__(self, myUri = None, playerUris = None, seretSetupMsg = None, groupBuilder = None ):
        self.mutex = RLock()
        self.mState  = GroupState.Type.PROPOSED
        self.mMyUri = myUri
        self.mPlayers = PlayerMap()
        self.ownPolynomialFunctions = {}
        self.coeffEntries = []
        self.signature_shares = {}
        self.intermediary_shares = {}
        self.intermediary_shares_curvePoint = {}
        self.receivedPolynomialFunctions=[] # received polynomials, a list of tuples, where each each tuple is of format - byUrl, forUrl, PolyValue
        self.own_public_secret_a0 = None
        self.public_secret_a0 = {}
        if (playerUris == None and seretSetupMsg == None and myUri != None):
            raise ValueError(" Invalid Constructor")

        if (playerUris == None and seretSetupMsg == None and myUri == None):
            return

        if (seretSetupMsg == None):
            self.mGroupID = UUID().getUUIDString()
            self.mProposerUri = myUri
            self.proposerIsPlayer = False
            ordinal = 1
            for uri in playerUris:
                _ordinal = -1
                if (groupBuilder != None):
                    _ordinal = groupBuilder.findPlayer(uri)
                    if (_ordinal == -1):
                        raise ValueError("given user isn't found in the group")
                    else:
                        ordinal = _ordinal

                playerItem = PlayerItem(uri, Player(uri, ordinal))
                if (self.proposerIsPlayer == False and uri == self.mMyUri):
                    self.proposerIsPlayer = True
                    # Proposer is ACCEPTED right away
                    playerItem.getPlayer().setAccepted()
                else:
                    # make random coefficient for each uri
                    coeffEntry = (uri, ordinal, random.randint(1, 2 ** 256))
                    self.coeffEntries.append(coeffEntry)

                ordinal = ordinal + 1
                #if (self.mPlayers.getCount() > 0 and playerItem in self.mPlayers and playerItem.getPlayer().getURI() == uri):
                for item in self.mPlayers.getItems():
                    if (item.getPlayer().getURI() == uri):
                        raise ValueError("Failed to create and insert player details for " + uri)

                self.mPlayers.add(playerItem)

            if (self.proposerIsPlayer == False):
                raise ValueError("The group proposer must be a member of the proposed group")


        else:

            self.mGroupID = seretSetupMsg.getID()
            self.mPlayers = seretSetupMsg.getPlayers()
            self.mProposerUri = seretSetupMsg.getProposerURI()

            if (self.mPlayers.findPlayer(myUri) == None):
                raise ValueError("The group proposer must be a member of the proposed group")

            for playerItem in self.mPlayers.getItems():
                if (groupBuilder != None):
                    _ordinal = groupBuilder.findPlayer(playerItem.getPlayer().getURI())
                    if (_ordinal == -1):
                        raise ValueError("given user isn't found in the group")

                if (playerItem.getPlayer().getURI() != myUri):
                    # make random coefficient for each  uri
                    coeffEntry = (playerItem.getPlayer().getURI(), playerItem.getPlayer().getOrdinal(), random.randint(1, 2 ** 256))
                    self.coeffEntries.append(coeffEntry)

            # get the received polynomials
            for recvPoly in seretSetupMsg.getPolynomialFunction():
                self.receivedPolynomialFunctions.append(recvPoly)

        if (len(self.mPlayers.getItems()) <= 1):
            raise ValueError("A group should have more than one player ")

        # Make private polynomials
        self.rp = Polynomial.Polynomial()
        for _ ,_ , coeff in self.coeffEntries :
            self.rp.append_coeff(coeff) 
            if (self.own_public_secret_a0 is None):
                self.own_public_secret_a0 = GroupSecretPlayerBuilder.getEcdsa256k1Point(coeff * Generator)

        self.public_secret_a0[self.getPlayer().getOrdinal()] = self.own_public_secret_a0

        # Make functions for all players
        for playerItem in self.mPlayers.getItems():
            self.ownPolynomialFunctions[playerItem.getPlayer().getURI()] = self.rp(playerItem.getPlayer().getOrdinal())

        if (groupBuilder != None):
            groupBuilder.registerGroupSecetShareBuilder(groupSecetBuilder=self, guid=self.mGroupID)

        #print("================"+self.mMyUri+"================")
        # dump own polynomialFunctions
        #for item in self.ownPolynomialFunctions.items():
        #    print(item)

    def dumpCoefficents(self):
        for item in self.coeffEntries:
            print(item)

    def __eq__(self, other):
        if not isinstance(other, GroupSecretPlayerBuilder):
            return False
        return ((self.mState == other.mState) and  
                (self.getID() == other.getID()) and
                (self.mProposerUri == other.mProposerUri) and
                #(self.mMyUri == other.mMyUri) and
                (self.getPlayers().getPlayersSortedByOrdinal() == other.getPlayers().getPlayersSortedByOrdinal())
                )

    def getID(self):
        return self.mGroupID  

    def getProposerURI(self):
        return self.mProposerUri

    def getMyURI(self):
        return self.mMyUri

    def getState(self):
        return self.mState

    def setID(self, mGroupID):
        self.mGroupID = mGroupID

    def getPlayers(self):
        self.mutex.acquire()
        try :
            return self.mPlayers
        finally:
            self.mutex.release()

    def getPlayer(self):
        self.mutex.acquire()
        try :
           return  self.mPlayers.findPlayer(self.getMyURI())
        finally:
            self.mutex.release()

    def getGroup(self):
        self.mutex.acquire()
        try:
            if (self.mState != GroupState.Type.ACCEPTED):
                raise RuntimeError("Can't create group in state " + GroupState.getStrByType(self.mState))

            return Group(self.mGroupID, self.mPlayers)
        finally:
            self.mutex.release()

    def getGroupSecretSetupMessage(self, forUrls = None):
        self.mutex.acquire()
        try:
            grpMsg = GroupSecretSetupMessage(uri = self.mProposerUri, groupID = self.mGroupID, playerMap = self.mPlayers)

            if(forUrls !=None):
                grpMsg.clearPolynomialFunction()
                for forUrl in forUrls:
                    for key, value  in self.ownPolynomialFunctions.items():
                        if (key == forUrl):
                            grpMsg.addPolynomialFunction(self.mMyUri, key, int(value))
              
            return grpMsg
        finally:
            self.mutex.release()

    def getGroupSecretSetupResponseMessage(self, state, forUrls = None):
        if (state != GroupState.Type.ACCEPTED and state != GroupState.Type.REJECTED):
            raise ValueError("Group response must be either ACCEPTED or REJECTED")
        response = GroupSecretResponseMessage(uri = self.mMyUri, groupID = self.mGroupID, GrpState = state)
        self.processGroupSetupResponse(response)
        if(forUrls !=None ):
          response.clearPolynomialFunction()
          if (len(forUrls) > 0):
             for forUrl in forUrls:
               for key, value  in self.ownPolynomialFunctions.items():
                 if (key == forUrl):
                   response.addPolynomialFunction(self.mMyUri, key, int(value))
          else :
              for key, value in self.ownPolynomialFunctions.items():
                if (key != self.mMyUri):
                    response.addPolynomialFunction(self.mMyUri, key, int(value))

        return response

    def getPolyFunctionsOfThePlayer(self, url):
        ''' to be called by proposer ususally'''
        self.mutex.acquire()
        try :
            polynomialFunctionsForTheGivenUrl = GroupSecretPolynomialVals(url)
            for polyfuncTuple in self.receivedPolynomialFunctions:
                if (polyfuncTuple[1] == url):
                    polynomialFunctionsForTheGivenUrl.addPolynomialFunction(polyfuncTuple[0], polyfuncTuple[1], polyfuncTuple[2])
            return polynomialFunctionsForTheGivenUrl
        finally:
            self.mutex.release()

    def processPolyFunctionsOfThePlayer(self, polynomialFunctionsForTheGivenUrl) :
      ''' to be called by proposer ususally'''
      self.mutex.acquire()
      try :
          for polyFuncTuple in polynomialFunctionsForTheGivenUrl.getPolynomialFunction():

              if (self.mMyUri == polyFuncTuple[1]):
                  # if the sending player has constructed current player's polynomial, mark the sender as accepted
                  player = self.mPlayers.findPlayer(polyFuncTuple[0]) 
                  if (player == None):
                      raise RuntimeError("Group response message is not from a proposed group member")
                  player.setAccepted()
                  self.receivedPolynomialFunctions.append(polyFuncTuple)

          # If all players have accepted, update our state
          if (self.mState != GroupState.Type.REJECTED):
              allAccepted = True

          for playerItem in self.mPlayers.getItems():
              if (playerItem.getPlayer().accepted() == False):
                  allAccepted = False
                  break

          if(allAccepted):
              self.mState = GroupState.Type.ACCEPTED

      finally:
         self.mutex.release()

    def getKeyShare(self):
        self.mutex.acquire()
        try:
            keyshare = 0
            keyshare += self.ownPolynomialFunctions[self.mMyUri]

            for polyFuncTuple in self.receivedPolynomialFunctions:
                if (polyFuncTuple[1] == self.mMyUri):
                    keyshare += polyFuncTuple[2]
            return  keyshare
        finally:
            self.mutex.release()

    @staticmethod
    def getIntermediaryKeyShare(ephemeral_key, blinding_key,  modulo=Order):
        return FiniteGroup.normalize_mod((ephemeral_key * blinding_key), modulo) 

    @staticmethod
    def getEcdsa256k1Point(point=None, x = None, y = None):

        # Point is Given
        if (point):
            return ecdsa.ellipticcurve.Point(FiniteGroup.curve_secp256k1, point.x(), point.y(), point.order())
        elif (x is not None and y is not None):
            return ecdsa.ellipticcurve.Point(FiniteGroup.curve_secp256k1, x, y, ecdsa.SECP256k1.order)


    
    def getSetupIntermediaryShareMessage(self, ephemeral_key, blinding_key, modulo=Order):
       # generate it's own intermediary key share
       IntermediaryKeyShare = GroupSecretPlayerBuilder.getIntermediaryKeyShare(ephemeral_key, blinding_key,  modulo)
       ordinal = self.getPlayer().getOrdinal()
       # Save the current share 
       self.intermediary_shares[ordinal] = IntermediaryKeyShare 
       return GroupSetupIntermediaryMessage(ordinal=ordinal, uri=self.mMyUri, groupID=self.mGroupID, intermediary_share=IntermediaryKeyShare)


    def getIntermediaryMessageResponse(self, ephemeral_key, blinding_key,  modulo=Order):

       grpSetupResponseIntermediaryMessage = GroupSetupResponseIntermediaryMessage(uri=self.mMyUri, groupID=self.mGroupID)

       # generate it's own intermediary key share
       IntermediaryKeyShare = GroupSecretPlayerBuilder.getIntermediaryKeyShare(ephemeral_key, blinding_key,  modulo)
       ordinal = self.getPlayer().getOrdinal()

       # Save the current share 
       self.intermediary_shares[ordinal] = IntermediaryKeyShare 

       # iterate through available intermediary key shares
       for _ordinal in self.intermediary_shares.keys():
           grpSetupResponseIntermediaryMessage.getIntermediaryShares().addIntermediaryMessage(ordinal=_ordinal, intermediary_share=self.intermediary_shares[_ordinal])

       return grpSetupResponseIntermediaryMessage

    def getBroadcastIntermediaryMessage(self):
       grpBroadcastIntermediaryMessage = GroupBroadcastIntermediaryMessage(uri=self.mMyUri, groupID=self.mGroupID)

       for _ordinal in self.intermediary_shares.keys():
           grpBroadcastIntermediaryMessage.getIntermediaryShares().addIntermediaryMessage(ordinal=_ordinal, intermediary_share=self.intermediary_shares[_ordinal])
       
       return grpBroadcastIntermediaryMessage

    def processSetupIntermediaryMessages(self, intermediarymessage=None):
        if (intermediarymessage.getGroupID() != self.mGroupID):
            raise RuntimeError("Intermediary message is invalid")

        for _ordinal in intermediarymessage.getIntermediaryShares().getIntermediaryShares().keys():
            if _ordinal not in self.intermediary_shares.keys():
                self.intermediary_shares[_ordinal] = intermediarymessage.getIntermediaryShares().getIntermediaryShares()[_ordinal]

    @staticmethod
    def getIntermediaryKeyShareCurvePoint(blinding_key, GenPoint=Generator):
        return GroupSecretPlayerBuilder.getEcdsa256k1Point(blinding_key*GenPoint)

    def getSetupIntermediaryShareCurvePointMessage(self, blinding_key):
       # generate it's own intermediary key share Of Curve point
       #IntermediaryKeyShareCurvePoint = GroupSecretPlayerBuilder.getIntermediaryKeyShareCurvePoint(blinding_key=blinding_key).x()
       IntermediaryKeyShareCurvePoint = GroupSecretPlayerBuilder.getIntermediaryKeyShareCurvePoint(blinding_key=blinding_key)
       ordinal = self.getPlayer().getOrdinal()
       # Save the current share 
       self.intermediary_shares_curvePoint[ordinal] = IntermediaryKeyShareCurvePoint 
       return GroupSetupIntermediaryMessage(ordinal=ordinal, uri=self.mMyUri, groupID=self.mGroupID, intermediary_share=IntermediaryKeyShareCurvePoint)

    def getIntermediaryCurvePointMessageResponse(self, blinding_key):

       grpSetupResponseIntermediaryMessage = GroupSetupResponseIntermediaryMessage(uri=self.mMyUri, groupID=self.mGroupID)

       # generate it's own intermediary key share of curve point
       #IntermediaryKeyShareCurvePoint = GroupSecretPlayerBuilder.getIntermediaryKeyShareCurvePoint(blinding_key=blinding_key).x()
       IntermediaryKeyShareCurvePoint = GroupSecretPlayerBuilder.getIntermediaryKeyShareCurvePoint(blinding_key=blinding_key)

       ordinal = self.getPlayer().getOrdinal()

       # Save the current share 
       self.intermediary_shares_curvePoint[ordinal] = IntermediaryKeyShareCurvePoint 

       # iterate through available intermediary key shares
       for _ordinal in self.intermediary_shares_curvePoint.keys():
           grpSetupResponseIntermediaryMessage.getIntermediaryShares().addIntermediaryMessage(ordinal=_ordinal, intermediary_share=self.intermediary_shares_curvePoint[_ordinal])

       return grpSetupResponseIntermediaryMessage

    def getBroadcastIntermediaryCurvePointMessage(self):
       grpBroadcastIntermediaryMessage = GroupBroadcastIntermediaryMessage(uri=self.mMyUri, groupID=self.mGroupID)

       for _ordinal in self.intermediary_shares_curvePoint.keys():
           grpBroadcastIntermediaryMessage.getIntermediaryShares().addIntermediaryMessage(ordinal=_ordinal, intermediary_share=self.intermediary_shares_curvePoint[_ordinal])
       
       return grpBroadcastIntermediaryMessage

    def processSetupIntermediaryCurvePointMessages(self, intermediarymessage=None):
        if (intermediarymessage.getGroupID() != self.mGroupID):
            raise RuntimeError("Intermediary message is invalid")

        for _ordinal in intermediarymessage.getIntermediaryShares().getIntermediaryShares().keys():
            if _ordinal not in self.intermediary_shares_curvePoint.keys():
                self.intermediary_shares_curvePoint[_ordinal] = intermediarymessage.getIntermediaryShares().getIntermediaryShares()[_ordinal]

    def dumpSetupIntermediaryMessages(self):
        print("==== Intermediary=====")
        for _ord, shares in self.intermediary_shares.items():
            print(str(_ord) + " : " + str(shares))
        print("=======================================")

    def dumpSetupIntermediaryCurvePointMessages(self):
        print("==== IntermediaryCurvePoint =====")
        for _ord, shares in self.intermediary_shares_curvePoint.items():
            print(str(_ord) + " : " + str(shares))
        print("=======================================") 
        

    def generateIntermediaryShareInterpolation(self):
        intermediary_sharepoints=[]
        for ordinal, intermediary_share in self.intermediary_shares.items():
            intermediary_sharepoints.append([ordinal, intermediary_share])
        interpolator = Polynomial.LagrangeInterpolator(intermediary_sharepoints, group_modulo=Order)
        self.interpolator_of_intermediary_sharepoints = interpolator(0)
        return self.interpolator_of_intermediary_sharepoints

    def generateIntermediaryShareCurveInterpolation(self):
        intermediary_sharepoints_curvepoint=[]
        for ordinal, intermediary_share in self.intermediary_shares_curvePoint.items():
            intermediary_sharepoints_curvepoint.append([ordinal, intermediary_share])
        interpolator = Polynomial.EC_LagrangeInterpolator(intermediary_sharepoints_curvepoint, group_modulo=Order)
        self.interpolator_of_intermediary_sharepoints_curvepoint = interpolator(0)
        return self.interpolator_of_intermediary_sharepoints_curvepoint

    def getSignatureShareMessage(self, signature_share=None):
        return GroupSignatureShareMessage(ordinal=self.getPlayer().getOrdinal(), uri=self.mMyUri, groupID=self.mGroupID, signature_share=signature_share)

    def processSignatureShareMessage(self, signature_share_msg=None):
        if (signature_share_msg and isinstance(signature_share_msg, GroupSignatureShareMessage) and signature_share_msg.getGroupID() == self.mGroupID):
            self.signature_shares[int(signature_share_msg.getOrdinal())] =  signature_share_msg.getSignatureShare()

    def dumpSignatureShareMessage(self):
        for ordinal, signature_share in self.signature_shares.items():
            print(str(ordinal)+ " ==>" + str(signature_share))

    def generateSignatureInterpolation(self):
        signature_shares = []
        for ordinal, signature_share in self.signature_shares.items():
            signature_shares.append([ordinal, signature_share])
        interpolator = Polynomial.LagrangeInterpolator(signature_shares, group_modulo=Order)
        self.interpolator_of_signature_shares = interpolator(0)
        return self.interpolator_of_signature_shares


    def setSignatureShare(self, signature_share=None):
        self.signature_shares[int(self.getPlayer().getOrdinal())] =  signature_share

    def IntermediaryShareInverse(self):
        Inv_interpolator_of_intermediary_sharepoints = FiniteGroup.inv_mod(self.interpolator_of_intermediary_sharepoints, Order)
        return Inv_interpolator_of_intermediary_sharepoints

    def ecdsa_r(self):
        self.r = (self.IntermediaryShareInverse() * self.interpolator_of_intermediary_sharepoints_curvepoint).x()
        return self.r

    def getShareOfSignature(self, msg, key_share=None, ephemeral_key=None,  modulo=Order):
        # get hash of the message
        Hm = FiniteGroup.hash_mod(msg, modulo)

        # share of the signature = k(hm + xr) mod n, where k is ephemeral key, x is key share and r is ecdsa_r
        self.share_of_signature = FiniteGroup.normalize_mod((ephemeral_key*(Hm + (key_share * self.r))), modulo)
        return self.share_of_signature

    def getPublicSecretA0Message(self):
        return GroupSecretPublicMessage(ordinal=self.getPlayer().getOrdinal(), uri=self.mMyUri, groupID=self.mGroupID, player_public_a0=self.public_secret_a0)

    def processPublicSecretA0Message(self, publicSecretA0=None ):
        if ((publicSecretA0 is None) or  publicSecretA0.getGroupID() !=  self.mGroupID):
            raise RuntimeError("Public Secret A0 message is invalid")

        for _ordinal in publicSecretA0.getPublicSecretA0().keys():
            if _ordinal not in self.public_secret_a0.keys():
                self.public_secret_a0[_ordinal] = publicSecretA0.getPublicSecretA0()[_ordinal]

    def calculatePublicKeyUsingSecretA0(self):
        public_key = None
        for ordinal, a0point in self.public_secret_a0.items():
            if (public_key):
                #public_key.__curve = a0point.curve()
                public_key = public_key + a0point
            else:
                public_key = a0point
        return public_key

    def processGroupSetupResponse(self, response):
        self.mutex.acquire()
        try :
            #Is response from someone in our group?
            if (response.getID() != self.mGroupID):
                raise RuntimeError("Group response message is not for our group")

            player = self.mPlayers.findPlayer(response.getResponderURI()) 
            if (player == None):
                raise RuntimeError("Group response message is not from a proposed group member")

            if (response.getMsgType() == Message.Type.GROUP_SECRET_RESPONSE):
                # get the received polynomials
                for recvPolyTuple in response.getPolynomialFunction():
                    self.receivedPolynomialFunctions.append(recvPolyTuple)

            # Mark responding player as accepted or rejected
            if (response.getResponse() == GroupState.Type.ACCEPTED):
                player.setAccepted()

                # If all players have accepted, update our state
                if (self.mState != GroupState.Type.REJECTED):
                    allAccepted = True

                    for playerItem in self.mPlayers.getItems():
                        if (playerItem.getPlayer().accepted() == False):
                            allAccepted = False
                            break

                    if(allAccepted):
                        self.mState = GroupState.Type.ACCEPTED

            elif (response.getResponse()== GroupState.Type.REJECTED): 
                player.setRejected()

                # Any rejection means we are rejected
                self.mState = GroupState.Type.REJECTED
            else :
                raise RuntimeError("Bad state in group response message: " + GroupState.getStrByType(response.getResponse()))
        finally:
            self.mutex.release()

    def jsonObjectType(self):
        return "GroupSecretPlayerBuilder"

    def getResponsesOfAllPlayers(self):
        val = ""
        for playerItem in self.mPlayers.getItems():
            val += playerItem.getPlayer().getURI()
            val += "<==>"
            val += str(playerItem.getPlayer().accepted())
            val += "\n"
        return val




    def to_json(self):
        self.mutex.acquire()
        try :
          gpBDt = {}
          gpBDt["ID"] = self.getID()
          gpBDt["State"] = GroupState.getStrByType(self.mState)
          gpBDt["ProposerURI"] = self.mProposerUri 
          gpBDt["MyURI"] = self.mMyUri 
          playerJson = []
          for playerItem in self.getPlayers().getItems():
              playerJson.append(playerItem.player.to_json())
          gpBDt["Players"] = playerJson
          return json.dumps(gpBDt)
        finally:
            self.mutex.release()

    def from_json(self, gpBDMsgStr ):
        self.mutex.acquire()
        try :
          gpBDt = json.loads(gpBDMsgStr)
          self.setID(gpBDt["ID"]) 
          self.mState = GroupState.getTypeByStr(gpBDt["State"])
          self.mProposerUri  = gpBDt["ProposerURI"] 
          self.mMyUri = gpBDt["MyURI"]
          for playerDict in gpBDt["Players"] :
             p = Player()
             p.from_json(playerDict)
             pi = PlayerItem(p.getURI(), p)
             self.getPlayers().add(pi)

          if (len(self.mPlayers.getItems()) <= 1):
             raise ValueError("A group should have more than one player ")

        finally:
            self.mutex.release()
#############################################
'''
gb = GroupSecretPlayerBuilder(playerUris=["p1", "p2", "p3", "p4"], myUri="p2")
bpStr = gb.to_json()

#Make sure that the proposer should be the member of the group.
try:
  gb = GroupSecretPlayerBuilder(playerUris=["p1", "p2", "p3", "p4"], myUri="p5")
except ValueError:
    pass

# Make sure that the single player group isnt allowed
try:
  gb = GroupSecretPlayerBuilder(playerUris=["p1"], myUri="p1")
except ValueError:
    pass

# Make sure that the players with same URIs can't be allowed in the group.
try:
    gb = GroupSecretPlayerBuilder(playerUris=["p1", "p1", "p3", "p4"], myUri="p1")
except ValueError:
    pass


try:
  p1 = Player("p1", 1, False, False)
  p11 = PlayerItem("player1@mycorp.com", p1)
  playersMap = PlayerMap()
  playersMap.add(p11)
  gp = GroupSecretSetupMessage( uri="player1@mycorp.com", playerMap=playersMap)
  gb = GroupSecretPlayerBuilder(seretSetupMsg = gp, myUri="p1")
except ValueError:
    pass

p1 = Player("p1", 1, False, False)
p2 = Player("p2", 2, False, False)
p3 = Player("p3", 3, False, False)
p4 = Player("p4", 4, False, False)


p11 = PlayerItem("player1@mycorp.com", p1)
p22 = PlayerItem("player2@mycorp.com", p2)
p33 = PlayerItem("player3@mycorp.com", p3)
p44 = PlayerItem("player4@mycorp.com", p4)

playersMap = PlayerMap()
playersMap.add(p11)
playersMap.add(p22)
playersMap.add(p33)
playersMap.add(p44)

gp = GroupSecretSetupMessage( uri="player1@mycorp.com", playerMap=playersMap)
gb = GroupSecretPlayerBuilder(seretSetupMsg = gp, myUri="p2")
gbStr = gb.to_json()

gbFJson = GroupSecretPlayerBuilder()
gbFJson.from_json(gbStr)
# make sure that to_json and from_json works fine.
assert (gbFJson == gb)


# LifeCycle tests 
'''
###################################################################################################
if __name__ == '__main__':

    # Make Group Builder for Player1
    gb1 = GroupBuilder(uri="player1@mycorp.com", ordinal=1)

    # get the initial group setup message of player1
    setupMsg = gb1.getInitialSetupMessage()
    setupMsgFromJson = GroupSetupMessage()
    setupMsgFromJson.from_json(setupMsg.to_json())

    # Make Group builder for player2 and player3
    gb2 = GroupBuilder(uri="player2@otherplace.org", ordinal=2)
    gb3 = GroupBuilder(uri="player3@secret.org", ordinal=3)

    # Player2 and Player3 will process setup Message from Player1
    gb2.processInitialSetupMessage(setupMsgFromJson)
    gb3.processInitialSetupMessage(setupMsgFromJson)

    # Player 2 and player 3 will get its response
    gb2SetupRes = gb2.getInitialSetupResponseMessage()
    gb3SetupRes = gb3.getInitialSetupResponseMessage()

    gb2SetupResFromJson = GroupSetupResponseMessage()
    gb3SetupResFromJson = GroupSetupResponseMessage()

    gb2SetupResFromJson.from_json(gb2SetupRes.to_json())
    gb3SetupResFromJson.from_json(gb3SetupRes.to_json())


    # Player 1 will process response from Player2 and Player3
    gb1.processInitialSetupResponseMessage(gb2SetupResFromJson)
    gb1.processInitialSetupResponseMessage(gb3SetupResFromJson)

    # Player1 will broadcast the group setup message to Player2 and Player3
    bcMsg = gb1.getBroadcastSetupMessage()
    bcMsgFromJson = GroupBroadcastMessage()
    bcMsgFromJson.from_json(bcMsg.to_json())

    # Player 2 and Player3 will process the broadcasted message from player1
    gb2.processBroadMessage(bcMsgFromJson)
    gb3.processBroadMessage(bcMsgFromJson)

    key_share_player1 = 0
    key_share_player2 = 0
    key_share_player3 = 0

    ephemeral_player1 = 0
    ephemeral_player2 = 0
    ephemeral_player3 = 0

    blinding_player1 = 0
    blinding_player2 = 0
    blinding_player3 = 0

    # At this point, gb1, gb2 and gb3 represents group player at player player1@mycorp.com", "player2@otherplace.org", "player3@secret.org sites, repectively.
    # Joint Random Secret Sharing ( JRSS )
    # Make key share, ephemeral and blinding secrets
    for key in ['KEY_SHARE', 'EPHEMERAL', 'BLINDING']:
        print("===================="+ key + "========================")
        # Make Group builder for player1
        player1Builder = GroupSecretPlayerBuilder(playerUris=["player1@mycorp.com", "player2@otherplace.org", "player3@secret.org"], myUri="player1@mycorp.com", groupBuilder=gb1)
        '''
        assert player1Builder.getPlayers().getCount() == 3
        assert player1Builder.getProposerURI() == "player1@mycorp.com"
        assert player1Builder.getMyURI() == "player1@mycorp.com"
        assert player1Builder.getState() == GroupState.Type.PROPOSED
        assert player1Builder.getPlayers().findPlayer("player1@mycorp.com").accepted() == True 
        #print(player1Builder.getResponsesOfAllPlayers())
        '''
    
        #Get group setup message for player2
        setupMsgForPlayer2 = player1Builder.getGroupSecretSetupMessage(forUrls=["player2@otherplace.org"])
        assert setupMsgForPlayer2  != None
        setupMsgDataForPlayer2 = setupMsgForPlayer2.to_json()
        #print(setupMsgDataForPlayer2)
        setupMsgForPlayer2 = GroupSecretSetupMessage()
        setupMsgForPlayer2.from_json(setupMsgDataForPlayer2)

        #Get group setup message for player3
        setupMsgForPlayer3 = player1Builder.getGroupSecretSetupMessage(forUrls=["player3@secret.org"])
        assert setupMsgForPlayer3  != None
        setupMsgDataForPlayer3 = setupMsgForPlayer3.to_json()
        #print(setupMsgDataForPlayer3)
        setupMsgForPlayer3 = GroupSecretSetupMessage()
        setupMsgForPlayer3.from_json(setupMsgDataForPlayer3)
    
        #Create builders from setup message for each non-proposer player(player2 and player3) using its corrsponding setup messages
        player2Builder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgForPlayer2, myUri="player2@otherplace.org", groupBuilder=gb2)
        player3Builder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgForPlayer3, myUri="player3@secret.org", groupBuilder=gb3)

        '''
        assert player2Builder != None
        assert player3Builder != None
        assert player1Builder == player2Builder
        assert player1Builder == player3Builder
        nonMemberResponse = GroupSecretResponseMessage(uri="weirdo@who.ru", groupID = player1Builder.getID(), GrpState = GroupState.Type.ACCEPTED)
        try:
          player1Builder.processGroupSetupResponse(response=nonMemberResponse)
        except RuntimeError:
            pass
        '''

        # Get acceptance responses from each player(player2 and player3) in turn
        player2Response = player2Builder.getGroupSecretSetupResponseMessage(state=GroupState.Type.ACCEPTED, forUrls=[])
        player3Response = player3Builder.getGroupSecretSetupResponseMessage(state=GroupState.Type.ACCEPTED, forUrls=[])
    
        player2ResponseFromJson =  GroupSecretResponseMessage()
        player2ResponseFromJson.from_json(player2Response.to_json())
    
        player3ResponseFromJson =  GroupSecretResponseMessage()
        player3ResponseFromJson.from_json(player3Response.to_json())
    
        ## Ideally, player2Response and player3Response be converted into JSON and be back to JSON as in the real world.
        # Let Player1 process responses from player2 and player3
        player1Builder.processGroupSetupResponse(response=player2ResponseFromJson)
        player1Builder.processGroupSetupResponse(response=player3ResponseFromJson)

        # Get the polynomials for player(player2 and player3)
        player2PolyFuncs = player1Builder.getPolyFunctionsOfThePlayer("player2@otherplace.org")
        player3PolyFuncs = player1Builder.getPolyFunctionsOfThePlayer("player3@secret.org")
    
        #print("++++++++++++++++++++++++++++++++++")
        #print(player2PolyFuncs) 
        #print(player2PolyFuncs.to_json()) 
        player2PolyFuncsFromPlayer1= GroupSecretPolynomialVals()
        player2PolyFuncsFromPlayer1.from_json(player2PolyFuncs.to_json()) 
        #print("++++++++++++++++++++++++++++++++++")
        #print(player3PolyFuncs) 
        #print(player3PolyFuncs.to_json()) 
        player3PolyFuncsFromPlayer1= GroupSecretPolynomialVals()
        player3PolyFuncsFromPlayer1.from_json(player3PolyFuncs.to_json())
        '''
        #print("++++++++++++++++++++++++++++++++++")
        assert player1Builder.getState() == GroupState.Type.ACCEPTED
        assert player2Builder.getState() == GroupState.Type.PROPOSED
        assert player3Builder.getState() == GroupState.Type.PROPOSED
        '''
        # let player2 and player3 process it's corrsponding messages from player1
        player2Builder.processPolyFunctionsOfThePlayer(player2PolyFuncsFromPlayer1)
        player3Builder.processPolyFunctionsOfThePlayer(player3PolyFuncsFromPlayer1)

        print(player1Builder.getKeyShare())
        print(player2Builder.getKeyShare())
        print(player3Builder.getKeyShare())

        if (key == 'EPHEMERAL'):
            # Get the Ephemeral key
            ephemeral_player1 = player1Builder.getKeyShare()
            ephemeral_player2 = player2Builder.getKeyShare()
            ephemeral_player3 = player3Builder.getKeyShare()
        elif (key == 'BLINDING'):
            # Get the Blinding secret
            blinding_player1 = player1Builder.getKeyShare()
            blinding_player2 = player2Builder.getKeyShare()
            blinding_player3 = player3Builder.getKeyShare()
        elif (key == 'KEY_SHARE'):
            # Get the  key shares
            key_share_player1 = player1Builder.getKeyShare()
            key_share_player2 = player1Builder.getKeyShare()
            key_share_player3 = player1Builder.getKeyShare()
        
        assert player1Builder.getState() == GroupState.Type.ACCEPTED
        assert player2Builder.getState() == GroupState.Type.ACCEPTED
        assert player3Builder.getState() == GroupState.Type.ACCEPTED

    #print("++++++++++++++++++++++++++++++++++")
    #print(GroupSecretPlayerBuilder.getIntermediaryKeyShare(ephemeral_key=ephemeral_player1, blinding_key=blinding_player1, modulo=Order))
    #print(GroupSecretPlayerBuilder.getIntermediaryKeyShare(ephemeral_key=ephemeral_player2, blinding_key=blinding_player2, modulo=Order))
    #print(GroupSecretPlayerBuilder.getIntermediaryKeyShare(ephemeral_key=ephemeral_player3, blinding_key=blinding_player3, modulo=Order))

    # STEP : Generate Ephemeral Key and Pre-Signature Data
    ''' Send Intermediary shares as below 
        step P1GEN : player1 generates Intermediary shares 
        step ISC1: player1(proposer) ---> Player2
        step ISC2: player1(proposer) ---> Player3

        step ISC3: Player2 -->its share --> player1
        step ISC4: Player3 -->its share --> player1
        
        step ISC5: player1(proposer) --broadcast aggregrated Intermediary shares-> Player2
        step ISC6: player1(proposer) --broadcast aggregrated Intermediary shares-> Player3
     '''
    #step P1GEN 
    setupIntermediaryMsg = player1Builder.getSetupIntermediaryShareMessage(ephemeral_key=ephemeral_player1, blinding_key=blinding_player1, modulo=Order)

    # step ISC1 and ISC2
    setupIntermediaryMsgFromJson = GroupSetupIntermediaryMessage()
    setupIntermediaryMsgFromJson.from_json(setupIntermediaryMsg.to_json())

    player2Builder.processSetupIntermediaryMessages(intermediarymessage=setupIntermediaryMsg)
    player3Builder.processSetupIntermediaryMessages(intermediarymessage=setupIntermediaryMsg)

    # step ISC3 and ISC4
    player2grpSetupResponseIntermediaryMessage = player2Builder.getIntermediaryMessageResponse(ephemeral_key=ephemeral_player2, blinding_key=blinding_player2, modulo=Order)
    player3grpSetupResponseIntermediaryMessage = player3Builder.getIntermediaryMessageResponse(ephemeral_key=ephemeral_player3, blinding_key=blinding_player3, modulo=Order)

    player2grpSetupResponseIntermediaryMessageFromJson = GroupSetupResponseIntermediaryMessage()
    player3grpSetupResponseIntermediaryMessageFromJson = GroupSetupResponseIntermediaryMessage()

    player2grpSetupResponseIntermediaryMessageFromJson.from_json(player2grpSetupResponseIntermediaryMessage.to_json())
    player3grpSetupResponseIntermediaryMessageFromJson.from_json(player3grpSetupResponseIntermediaryMessage.to_json())

    player1Builder.processSetupIntermediaryMessages(intermediarymessage=player2grpSetupResponseIntermediaryMessageFromJson)
    player1Builder.processSetupIntermediaryMessages(intermediarymessage=player3grpSetupResponseIntermediaryMessageFromJson)

    # step ISC5 and ISC6
    player1BroadcastIntermediaryMsg = player1Builder.getBroadcastIntermediaryMessage()
    player1BroadcastIntermediaryMsgFromJson = GroupBroadcastIntermediaryMessage() 
    player1BroadcastIntermediaryMsgFromJson.from_json(player1BroadcastIntermediaryMsg.to_json())
    
    player2Builder.processSetupIntermediaryMessages(intermediarymessage=player1BroadcastIntermediaryMsgFromJson)
    player3Builder.processSetupIntermediaryMessages(intermediarymessage=player1BroadcastIntermediaryMsgFromJson)

    #Step : dump Intermediary shares
    player1Builder.dumpSetupIntermediaryMessages()

    #Calculate Intermediate share : Curve point
    ''' Send Intermediary shares(curvePoint) as below 
        step P1GEN : player1 generates Intermediary shares 
        step ISC1: player1(proposer) ---> Player2
        step ISC2: player1(proposer) ---> Player3

        step ISC3: Player2 -->its share --> player1
        step ISC4: Player3 -->its share --> player1
        
        step ISC5: player1(proposer) --broadcast aggregrated Intermediary shares-> Player2
        step ISC6: player1(proposer) --broadcast aggregrated Intermediary shares-> Player3
     '''
    #step P1GEN 
    setupIntermediaryMsg = player1Builder.getSetupIntermediaryShareCurvePointMessage(blinding_key=blinding_player1)

    # step ISC1 and ISC2
    setupIntermediaryMsgFromJson = GroupSetupIntermediaryMessage()
    setupIntermediaryMsgFromJson.from_json(setupIntermediaryMsg.to_json())

    player2Builder.processSetupIntermediaryCurvePointMessages(intermediarymessage=setupIntermediaryMsg)
    player3Builder.processSetupIntermediaryCurvePointMessages(intermediarymessage=setupIntermediaryMsg)

    # step ISC3 and ISC4
    player2grpSetupResponseIntermediaryMessage = player2Builder.getIntermediaryCurvePointMessageResponse(blinding_key=blinding_player2)
    player3grpSetupResponseIntermediaryMessage = player3Builder.getIntermediaryCurvePointMessageResponse(blinding_key=blinding_player3)

    player2grpSetupResponseIntermediaryMessageFromJson = GroupSetupResponseIntermediaryMessage()
    player3grpSetupResponseIntermediaryMessageFromJson = GroupSetupResponseIntermediaryMessage()

    player2grpSetupResponseIntermediaryMessageFromJson.from_json(player2grpSetupResponseIntermediaryMessage.to_json())
    player3grpSetupResponseIntermediaryMessageFromJson.from_json(player3grpSetupResponseIntermediaryMessage.to_json())

    player1Builder.processSetupIntermediaryCurvePointMessages(intermediarymessage=player2grpSetupResponseIntermediaryMessageFromJson)
    player1Builder.processSetupIntermediaryCurvePointMessages(intermediarymessage=player3grpSetupResponseIntermediaryMessageFromJson)

    # step ISC5 and ISC6
    player1BroadcastIntermediaryMsg = player1Builder.getBroadcastIntermediaryCurvePointMessage()
    player1BroadcastIntermediaryMsgFromJson = GroupBroadcastIntermediaryMessage() 
    player1BroadcastIntermediaryMsgFromJson.from_json(player1BroadcastIntermediaryMsg.to_json())
    
    player2Builder.processSetupIntermediaryCurvePointMessages(intermediarymessage=player1BroadcastIntermediaryMsgFromJson)
    player3Builder.processSetupIntermediaryCurvePointMessages(intermediarymessage=player1BroadcastIntermediaryMsgFromJson)

    #Step : dump Intermediary shares curve points
    player1Builder.dumpSetupIntermediaryCurvePointMessages()

    print("========IntermediaryShareInterpolation======================")
    print("Player 1 ==> " + str(player1Builder.generateIntermediaryShareInterpolation()))
    print("Player 2 ==> " + str(player2Builder.generateIntermediaryShareInterpolation()))
    print("Player 3 ==> " + str(player3Builder.generateIntermediaryShareInterpolation()))

    print("========IntermediaryShareCurvePointInterpolation======================")
    print("Player 1 ==> " + str(player1Builder.generateIntermediaryShareCurveInterpolation()))
    print("Player 2 ==> " + str(player2Builder.generateIntermediaryShareCurveInterpolation()))
    print("Player 3 ==> " + str(player3Builder.generateIntermediaryShareCurveInterpolation()))

    print("===========ECDSA R===================")
    print("Player 1 ==> " + str(player1Builder.ecdsa_r()))
    print("Player 2 ==> " + str(player2Builder.ecdsa_r()))
    print("Player 3 ==> " + str(player3Builder.ecdsa_r()))

    # STEP : Threshold Signature Generation
    msg = 'A quick brown fox jumps over lazy the dog'

    # Generate share of Signature by each player
    print("===========Share of Signature===================")
    player1SignatureShare = player1Builder.getShareOfSignature(msg=msg, key_share=key_share_player1, ephemeral_key=ephemeral_player1,  modulo=Order)
    player2SignatureShare = player2Builder.getShareOfSignature(msg=msg, key_share=key_share_player2, ephemeral_key=ephemeral_player2,  modulo=Order)
    player3SignatureShare = player3Builder.getShareOfSignature(msg=msg, key_share=key_share_player3, ephemeral_key=ephemeral_player3,  modulo=Order)

    print("Player 1 ==>"  + str(player1SignatureShare))
    print("Player 2 ==>" + str(player2SignatureShare))
    print("Player 3 ==>" + str(player3SignatureShare))

    # Set the current share of signature of Player1
    player1Builder.setSignatureShare(signature_share=player1SignatureShare)

    # Player 2 and player 3 will send their respective signature shares to the player1
    SignatureShareMsg2 =   player2Builder.getSignatureShareMessage(signature_share=player2SignatureShare)
    SignatureShareMsg2FromJson = GroupSignatureShareMessage() 
    SignatureShareMsg2FromJson.from_json(SignatureShareMsg2.to_json())

    SignatureShareMsg3 =   player3Builder.getSignatureShareMessage(signature_share=player3SignatureShare)
    SignatureShareMsg3FromJson = GroupSignatureShareMessage() 
    SignatureShareMsg3FromJson.from_json(SignatureShareMsg3.to_json())

    # Player 1 process the signature shares sent by player2 and player3
    player1Builder.processSignatureShareMessage(signature_share_msg=SignatureShareMsg2FromJson)
    player1Builder.processSignatureShareMessage(signature_share_msg=SignatureShareMsg3FromJson)

    '''
    print("==== Signature Shares that Player 1 received ======")
    player1Builder.dumpSignatureShareMessage()
    print("=======================================") 
    '''

    # Player1 will calculate interpolation of all signatures, and get at 0
    print("============================================================================================")
    print("Signature Interpolate value ==> " + str(player1Builder.generateSignatureInterpolation()))
    print("============================================================================================")
    #print("====Player1 coefficents=====")
    #print(player1Builder.dumpCoefficents())

    #print("====Player2 coefficents=====")
    #print(player2Builder.dumpCoefficents())

    #print("====Player3 coefficents=====")
    #print(player3Builder.dumpCoefficents())
    #print("============================================================================================")


    
    # Let player1 get his/her PublicSecretA0
    player1a0Msg = player1Builder.getPublicSecretA0Message()
    #print("Player 1 ==>" + player1a0Msg.to_json())
    player1a0MsgFromJson = GroupSecretPublicMessage() 
    player1a0MsgFromJson.from_json(player1a0Msg.to_json()) 

    # Let player2 get his/her PublicSecretA0
    player2a0Msg = player2Builder.getPublicSecretA0Message()
    #print("Player 2 ==>" + player2a0Msg.to_json())
    player2a0MsgFromJson = GroupSecretPublicMessage() 
    player2a0MsgFromJson.from_json(player2a0Msg.to_json()) 

    # Let player3 get his/her PublicSecretA0
    player3a0Msg = player3Builder.getPublicSecretA0Message()
    #print("Player 3 ==>" + player3a0Msg.to_json())
    player3a0MsgFromJson = GroupSecretPublicMessage() 
    player3a0MsgFromJson.from_json(player3a0Msg.to_json()) 

    # let player2 and player3 send their PublicSecretA0 to Player1
    player1Builder.processPublicSecretA0Message(publicSecretA0=player2a0MsgFromJson)
    player1Builder.processPublicSecretA0Message(publicSecretA0=player3a0MsgFromJson)

    # get the broadcast message from player1
    player1a0BcastMsg = player1Builder.getPublicSecretA0Message()
    #print("Player 1 ==>" + player1a0BcastMsg.to_json())
    player1a0BcastMsgFromJson = GroupSecretPublicMessage() 
    player1a0BcastMsgFromJson.from_json(player1a0BcastMsg.to_json()) 

    # let  player2 and player3 know about the broadcasted message from player1
    player2Builder.processPublicSecretA0Message(publicSecretA0=player1a0BcastMsgFromJson)
    player3Builder.processPublicSecretA0Message(publicSecretA0=player1a0BcastMsgFromJson)

    print("Player 1 Public Key ==>" + str(player1Builder.calculatePublicKeyUsingSecretA0()))
    print("Player 2 Public Key ==>" + str(player2Builder.calculatePublicKeyUsingSecretA0()))
    print("Player 3 Public Key ==>" + str(player3Builder.calculatePublicKeyUsingSecretA0()))

    '''
    verifyIt = Nakasendo.verify(msg, player1Builder.calculatePublicKeyUsingSecretA0(), sig[0], sig[1])
    if (verifyIt == True):
        print ('msg verified')
    '''
    #Hm = FiniteGroup.hash_mod(msg, Order)
    msgEncoded =  bytes(msg, 'utf-8')

    FiniteGroup.ec_sig_verify(msg=msgEncoded,
                              pubKey=player1Builder.calculatePublicKeyUsingSecretA0(), 
                              r_input=player1Builder.ecdsa_r(), 
                              s_input=player1Builder.generateSignatureInterpolation()
                             )



###################################################################################################
'''
###################################################################################################
# Test lifecycle of group acceptance

player1Builder = GroupSecretPlayerBuilder(playerUris=["player1@mycorp.com", "player2@otherplace.org", "player3@secret.org"], myUri="player1@mycorp.com")
assert player1Builder.getPlayers().getCount() == 3
assert player1Builder.getProposerURI() == "player1@mycorp.com"
assert player1Builder.getMyURI() == "player1@mycorp.com"
assert player1Builder.getState() == GroupState.Type.PROPOSED
assert player1Builder.getPlayers().findPlayer("player1@mycorp.com").accepted() == True 
#print(player1Builder.getResponsesOfAllPlayers())

#Get group setup message

setupMsgForPlayer2 = player1Builder.getGroupSecretSetupMessage(forUrls=["player2@otherplace.org"])
assert setupMsgForPlayer2  != None
setupMsgDataForPlayer2 = setupMsgForPlayer2.to_json()
print(setupMsgDataForPlayer2)
setupMsgForPlayer2 = GroupSecretSetupMessage()
setupMsgForPlayer2.from_json(setupMsgDataForPlayer2)

setupMsgForPlayer3 = player1Builder.getGroupSecretSetupMessage(forUrls=["player3@secret.org"])
assert setupMsgForPlayer3  != None
setupMsgDataForPlayer3 = setupMsgForPlayer3.to_json()
print(setupMsgDataForPlayer3)
setupMsgForPlayer3 = GroupSecretSetupMessage()
setupMsgForPlayer3.from_json(setupMsgDataForPlayer3)

#Create builders from setup message for each non-proposer player
player2Builder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgForPlayer2, myUri="player2@otherplace.org")
assert player2Builder != None
player3Builder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgForPlayer3, myUri="player3@secret.org")
assert player3Builder != None

assert player1Builder == player2Builder
assert player1Builder == player3Builder

nonMemberResponse = GroupSecretResponseMessage(uri="weirdo@who.ru", groupID = player1Builder.getID(), GrpState = GroupState.Type.ACCEPTED)
try:
  player1Builder.processGroupSetupResponse(response=nonMemberResponse)
except RuntimeError:
    pass

#Test acceptance responses from each player in turn
player2Response = player2Builder.getGroupSecretSetupResponseMessage(state=GroupState.Type.ACCEPTED, forUrls=[])
player3Response = player3Builder.getGroupSecretSetupResponseMessage(state=GroupState.Type.ACCEPTED, forUrls=[])
print(player2Response.to_json())
print(player3Response.to_json())

## Ideally, player2Response and player3Response be converted into JSON and be back to JSON in the real world.

#Player 2 responses
player1Builder.processGroupSetupResponse(response=player2Response)
player3Builder.processGroupSetupResponse(response=player2Response)
assert player1Builder.getState() == GroupState.Type.PROPOSED
assert player2Builder.getState() == GroupState.Type.PROPOSED
assert player3Builder.getState() == GroupState.Type.ACCEPTED

#Player 3 responses
player1Builder.processGroupSetupResponse(response=player3Response)
player2Builder.processGroupSetupResponse(response=player3Response)
assert player1Builder.getState() == GroupState.Type.ACCEPTED
assert player2Builder.getState() == GroupState.Type.ACCEPTED
assert player3Builder.getState() == GroupState.Type.ACCEPTED

#Can now fetch created Group.
player1Group = player1Builder.getGroup()
player2Group = player2Builder.getGroup()
player3Group = player3Builder.getGroup()
assert player1Group == player2Group
assert player2Group == player3Group
###################################################################################################

#Test lifecycle of group rejection
player1Builder = GroupSecretPlayerBuilder(playerUris=["player1@mycorp.com", "player2@otherplace.org", "player3@secret.org"], myUri="player1@mycorp.com")
assert player1Builder.getPlayers().getCount() == 3
assert player1Builder.getProposerURI() == "player1@mycorp.com"
assert player1Builder.getMyURI() == "player1@mycorp.com"
assert player1Builder.getState() == GroupState.Type.PROPOSED
assert player1Builder.getPlayers().findPlayer("player1@mycorp.com").accepted() == True 
seretSetupMsg = player1Builder.getGroupSecretSetupMessage()
assert seretSetupMsg != None
setupMsgData = seretSetupMsg.to_json()
#print(seretSetupMsg)
setupMsgForPlayer2 = GroupSecretSetupMessage()
setupMsgForPlayer2.from_json(setupMsgData)
setupMsgForPlayer3 = GroupSecretSetupMessage()
setupMsgForPlayer3.from_json(setupMsgData)

#Create builders from setup message for each non-proposer player
player2Builder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgForPlayer2, myUri="player2@otherplace.org")
assert player2Builder != None
player3Builder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgForPlayer3, myUri="player3@secret.org")
assert player3Builder != None

assert player1Builder == player2Builder
assert player1Builder == player3Builder

#Test acceptance responses from each player in turn
player2Response = player2Builder.getGroupSecretSetupResponseMessage(state=GroupState.Type.ACCEPTED)
player3Response = player3Builder.getGroupSecretSetupResponseMessage(state=GroupState.Type.REJECTED)

#Player 2 responses
player1Builder.processGroupSetupResponse(response=player2Response)
player3Builder.processGroupSetupResponse(response=player2Response)
assert player1Builder.getState() == GroupState.Type.PROPOSED
assert player2Builder.getState() == GroupState.Type.PROPOSED
assert player3Builder.getState() == GroupState.Type.REJECTED


#Player 3 responses
player1Builder.processGroupSetupResponse(response=player3Response)
player2Builder.processGroupSetupResponse(response=player3Response)
assert player1Builder.getState() == GroupState.Type.REJECTED
assert player2Builder.getState() == GroupState.Type.REJECTED
assert player3Builder.getState() == GroupState.Type.REJECTED
################
player1Builder = GroupSecretPlayerBuilder(playerUris=["player1@mycorp.com", "player2@otherplace.org", "player3@secret.org"], myUri="player1@mycorp.com")
seretSetupMsg = player1Builder.getGroupSecretSetupMessage() 
builder2 = GroupSecretPlayerBuilder(seretSetupMsg =seretSetupMsg, myUri="player2@otherplace.org")
try:
  builder2.getGroupSecretSetupResponseMessage(GroupState.Type.PROPOSED)
except ValueError:
  pass
'''
