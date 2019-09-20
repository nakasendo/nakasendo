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
from PyGroupSecretIntermediary import IntermediaryMessages, GroupSetupIntermediaryMessage, GroupSetupResponseIntermediaryMessage, GroupBroadcastIntermediaryMessage, GroupSignatureShareMessage, GroupSecretPublicMessage, EncryptedECPointsMessages, EncryptedFunctionECPointsMessages
import ecdsa
import FiniteGroup
import utils
import datetime


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
        self.ownPolynomialFunctions_ecpoints = {}
        self.coeffEntries = []
        self.signature_shares = {}
        self.intermediary_shares = {}
        self.intermediary_shares_curvePoint = {}
        self.receivedPolynomialFunctions=[] # received polynomials, a list of tuples, where each each tuple is of format - byUrl, forUrl, PolyValue
        self.own_public_secret_a0 = None
        self.public_secret_a0 = {}

        self.encrypted_coefficients = {} # {ordinal --> [G*coeff, G*coeff....]}

        if (playerUris == None and seretSetupMsg == None and myUri != None):
            raise ValueError(" Invalid Constructor")

        if (playerUris == None and seretSetupMsg == None and myUri == None):
            return

        if (groupBuilder != None):
            self.threashold_value = groupBuilder.getThresholdValue()

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
                '''
                else:
                    # make random coefficient for each uri
                    coeffEntry = (uri, ordinal, random.randint(1, 2 ** 256))
                    self.coeffEntries.append(coeffEntry)
                '''

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

                '''
                if (playerItem.getPlayer().getURI() != myUri):
                    # make random coefficient for each  uri
                    coeffEntry = (playerItem.getPlayer().getURI(), playerItem.getPlayer().getOrdinal(), random.randint(1, 2 ** 256))
                    self.coeffEntries.append(coeffEntry)
                '''

            # get the received polynomials
            for recvPoly in seretSetupMsg.getPolynomialFunction():
                self.receivedPolynomialFunctions.append(recvPoly)

        if (len(self.mPlayers.getItems()) <= 1):
            raise ValueError("A group should have more than one player ")

        for item in range(0, self.threashold_value + 1):
            self.coeffEntries.append(random.randint(1, 2 ** 256))

        # Make private polynomials
        self.rp = Polynomial.Polynomial()
        for coeff in self.coeffEntries :
            self.rp.append_coeff(coeff) 
            if (self.own_public_secret_a0 is None):
                self.own_public_secret_a0 = GroupSecretPlayerBuilder.getEcdsa256k1Point(coeff * Generator)

        # Get encrypted Coefficients
        coeff_ec = []
        for coeff in self.coeffEntries :
            coeff_ec.append(GroupSecretPlayerBuilder.getEcdsa256k1Point(coeff * Generator))
        self.encrypted_coefficients[self.getPlayer().getOrdinal()] = coeff_ec

        self.public_secret_a0[self.getPlayer().getOrdinal()] = self.own_public_secret_a0

        # Make functions for all players
        functional_ec = []
        for playerItem in self.mPlayers.getItems():
            polyVal = self.rp(playerItem.getPlayer().getOrdinal())
            self.ownPolynomialFunctions[playerItem.getPlayer().getURI()] = polyVal
            functional_ec.append( (playerItem.getPlayer().getOrdinal(),  GroupSecretPlayerBuilder.getEcdsa256k1Point(polyVal * Generator)))
        self.ownPolynomialFunctions_ecpoints[self.getPlayer().getOrdinal()] = functional_ec

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

    def getEncryptedCoefficents(self):
        return self.encrypted_coefficients

    def getCoefficientMessages(self):
        encrypted_coefficients_msgs = EncryptedECPointsMessages()
        encrypted_coefficients_msgs.addEncryptedECPointsMessage(groupId=self.mGroupID, ordinal=self.getPlayer().getOrdinal(), encrypted_ecpoints=self.encrypted_coefficients[self.getPlayer().getOrdinal()])
        return encrypted_coefficients_msgs

    def processEncryptedCoefficients(self, encrypted_coefficients_msgs=None):

        # Make sure that incoming messages are of the same group
        if (encrypted_coefficients_msgs.getGroupID() != self.mGroupID):
            raise RuntimeError("Message is invalid")

        # parse the EC coefficients
        for ordinal, ec_coefficeints in encrypted_coefficients_msgs.getEncryptedECPointsMessage().items():
            if ordinal not in self.encrypted_coefficients.keys():
                self.encrypted_coefficients[ordinal] = ec_coefficeints

    def getEncryptedECPoints(self):
        return self.ownPolynomialFunctions_ecpoints

    def getPolyFunctionsEncryptedPoints(self):
        encrypted_polyFuncs_enc_msgs = EncryptedFunctionECPointsMessages()
        encrypted_polyFuncs_enc_msgs.addEncryptedECPointsMessage(groupId=self.mGroupID, ordinal=self.getPlayer().getOrdinal(), encrypted_ecpoints=self.ownPolynomialFunctions_ecpoints[self.getPlayer().getOrdinal()])
        return encrypted_polyFuncs_enc_msgs

    def processPolyFunctionsEncryptedPoints(self, encrypted_ecpoints_msgs = None):
        # Make sure that incoming messages are of the same group
        if (encrypted_ecpoints_msgs.getGroupID() != self.mGroupID):
            raise RuntimeError("Message is invalid")

        # parse the EC points
        for ordinal, ec_points in encrypted_ecpoints_msgs.getEncryptedECPointsMessage().items():
            if ordinal not in self.ownPolynomialFunctions_ecpoints.keys():
                self.ownPolynomialFunctions_ecpoints[ordinal] = ec_points


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

    @staticmethod
    def verifyThresholdValue(total_players_numbers, threshold_value):

        if (((2 * threshold_value) + 1) > total_players_numbers):
            return False
        return True

#############################################
###################################################################################################
if __name__ == '__main__':

    total_players_numbers = int(input("Enter the number of total number of players: ")) 
    threshold_value = int(input("Enter the threshold value: ")) 
 
    if (GroupSecretPlayerBuilder.verifyThresholdValue(total_players_numbers=total_players_numbers, threshold_value=threshold_value) == False):
        print("Incorrect Threshold value ")
        exit(0)

    # Get the list of players in the group
    player_grp_list = [] 
    for ordinal, player_url in utils.getPlayers(total_players_numbers):
        player_grp_list.append(GroupBuilder(uri=player_url, ordinal=ordinal))

    # ASSUME that player1 is the PROPOSER
    # Let proposer sends the setup messages to other players
    # get the initial group setup message of proposer
    setupMsg = player_grp_list[0].getInitialSetupMessage(threshold_value=threshold_value)
    setupMsgFromJson = GroupSetupMessage()
    setupMsgFromJson.from_json(setupMsg.to_join())

    for playerNumber in range(1, len(player_grp_list)):
        # Let other players process setup Message from Player1
        player_grp_list[playerNumber].processInitialSetupMessage(setupMsgFromJson)

    for playerNumber in range(1, len(player_grp_list)):
        #Let other players get its response
        gbSetupRes = player_grp_list[playerNumber].getInitialSetupResponseMessage()
        SetupResFromJson = GroupSetupResponseMessage()
        SetupResFromJson.from_json(gbSetupRes.to_json())

        # Player 1 will process response from other player
        player_grp_list[0].processInitialSetupResponseMessage(SetupResFromJson)

    # Player1 will broadcast the group setup message to other players
    bcMsg = player_grp_list[0].getBroadcastSetupMessage()
    bcMsgFromJson = GroupBroadcastMessage()
    bcMsgFromJson.from_json(bcMsg.to_json())

    for playerNumber in range(1, len(player_grp_list)):
        #Other players will process the broadcasted message from proposer
        player_grp_list[playerNumber].processBroadMessage(bcMsgFromJson)

    key_shares = []
    ephemeral_key_shares = []
    blinding_key_shares = []

    playerUris = []
    for ordinal, player_url in utils.getPlayers(total_players_numbers):
        playerUris.append(player_url) 

    # Joint Random Secret Sharing ( JRSS )
    # Make key share, ephemeral and blinding secrets
    for key in ['KEY_SHARE', 'EPHEMERAL', 'BLINDING']:

        # Make Group secret builder for proposer
        proposerBuilder = GroupSecretPlayerBuilder(playerUris=playerUris, myUri=playerUris[0], groupBuilder=player_grp_list[0])

        gpSecPlayerBuilder = []
        gpSecPlayerBuilder.append(proposerBuilder)

        for playerNumber in range(1, len(playerUris)):
            #Get group setup message for player2
            setupMsgForPlayer = proposerBuilder.getGroupSecretSetupMessage(forUrls=[playerUris[playerNumber]])
            setupMsgDataForPlayer = GroupSecretSetupMessage()
            setupMsgDataForPlayer.from_json(setupMsgForPlayer.to_json())
            playerBuilder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgDataForPlayer, myUri=playerUris[playerNumber], groupBuilder=player_grp_list[playerNumber])
            gpSecPlayerBuilder.append(playerBuilder)

        player_response_mgs = []
        for playerNumber in range(1, len(playerUris)):
            #Get acceptance responses from each player
            playerResponse = gpSecPlayerBuilder[playerNumber].getGroupSecretSetupResponseMessage(state=GroupState.Type.ACCEPTED, forUrls=[])
            player_response_mgs.append(playerResponse.to_json())

        for playerNumber in range(1, len(playerUris)):
            playerResponseFromJson =  GroupSecretResponseMessage()
            playerResponseFromJson.from_json(player_response_mgs[playerNumber - 1])
            #Let Player1 process responses from Other players
            proposerBuilder.processGroupSetupResponse(response=playerResponseFromJson)

        # Get the polynomials for non corrdinating players
        for playerNumber in range(1, len(playerUris)):
            playerPolyFuncs = proposerBuilder.getPolyFunctionsOfThePlayer(playerUris[playerNumber])
            playerPolyFuncsFromProposer = GroupSecretPolynomialVals()
            playerPolyFuncsFromProposer.from_json(playerPolyFuncs.to_json())
            gpSecPlayerBuilder[playerNumber].processPolyFunctionsOfThePlayer(playerPolyFuncsFromProposer)

            '''
            key_share_player = []
            ephemeral_player = []
            blinding_player = []
            '''
        for playerNumber in range(0, len(playerUris)):
            if (key == 'EPHEMERAL'):
                ephemeral_key_shares.append(gpSecPlayerBuilder[playerNumber].getKeyShare())
            elif (key == 'BLINDING'):
                blinding_key_shares.append(gpSecPlayerBuilder[playerNumber].getKeyShare())
            elif (key == 'KEY_SHARE'):
               key_shares.append(gpSecPlayerBuilder[playerNumber].getKeyShare())
            assert gpSecPlayerBuilder[playerNumber].getState() == GroupState.Type.ACCEPTED

        # SETUP VERIFICATION DATA
        # Let the proposer send their verification data(Coefficients) to the non-coordinating players
        proposerCoefficientMessages = proposerBuilder.getCoefficientMessages()
        proposerCoefficientMessagesFromJson = EncryptedECPointsMessages()
        proposerCoefficientMessagesFromJson.from_json(proposerCoefficientMessages.to_json())

        # All non-cooperating players must process the incoming Coefficients from proposer
        for playerNumber in range(1, len(playerUris)):
            # Send non-cooperating players Coefficients to the proposer
            playersCoefficientMessages = gpSecPlayerBuilder[playerNumber].getCoefficientMessages()
            playersCoefficientMessagesFromJson = EncryptedECPointsMessages()
            playersCoefficientMessagesFromJson.from_json(playersCoefficientMessages.to_json())
            proposerBuilder.processEncryptedCoefficients(playersCoefficientMessagesFromJson)

       # Broadcast messages to non-cooperating players
        for playerNumber in range(1, len(playerUris)):
            proposerCoefficientMessages = proposerBuilder.getCoefficientMessages()
            proposerCoefficientMessagesFromJson = EncryptedECPointsMessages()
            proposerCoefficientMessagesFromJson.from_json(proposerCoefficientMessages.to_json())
            gpSecPlayerBuilder[playerNumber].processEncryptedCoefficients(proposerCoefficientMessagesFromJson)

        # Let the proposer send their verification data(Encrypted EC Points) to the non-coordinating players
        proposerEncryptedECPoints = proposerBuilder.getPolyFunctionsEncryptedPoints()
        proposerEncryptedECPointsFromJson = EncryptedFunctionECPointsMessages()
        proposerEncryptedECPointsFromJson.from_json(proposerEncryptedECPoints.to_json())

        # All non-cooperating players must process the incoming Functiona ECPoints from proposer
        for playerNumber in range(1, len(playerUris)):
            # Send non-cooperating players ECPoints to the proposer
            playersECPointsMessages = gpSecPlayerBuilder[playerNumber].getPolyFunctionsEncryptedPoints()
            playersECPointsMessagesFromJson = EncryptedFunctionECPointsMessages()
            playersECPointsMessagesFromJson.from_json(playersECPointsMessages.to_json())
            proposerBuilder.processPolyFunctionsEncryptedPoints(playersECPointsMessagesFromJson)

       # Broadcast messages to non-cooperating players
        for playerNumber in range(1, len(playerUris)):
            proposerECPointsMessages = proposerBuilder.getPolyFunctionsEncryptedPoints()
            proposerECPointsMessagesFromJson = EncryptedFunctionECPointsMessages()
            proposerECPointsMessagesFromJson.from_json(proposerECPointsMessages.to_json())
            gpSecPlayerBuilder[playerNumber].processPolyFunctionsEncryptedPoints(proposerECPointsMessagesFromJson)


    print("=====================EPHEMERAL=====================")
    for keyShare in ephemeral_key_shares:
      print(keyShare)
    print("=====================BLINDING=====================")
    for keyShare in blinding_key_shares:
      print(keyShare)
    print("=====================KEY_SHARE=====================")
    for keyShare in key_shares:
      print(keyShare)

    ### Verification of Data


    # STEP : Generate Ephemeral Key and Pre-Signature Data
    ''' Send Intermediary shares as below 
        step P1GEN : proposer generates Intermediary shares 
        step ISC1: proposer(proposer) ---> non-coordinating Player
        step ISC2: non-coordinating player -->its share --> proposer
        step ISC3: proposer --broadcast aggregrated Intermediary shares-> non-coordinating player
     '''
    #step P1GEN
    setupIntermediaryMsg = proposerBuilder.getSetupIntermediaryShareMessage(ephemeral_key=ephemeral_key_shares[0], blinding_key=blinding_key_shares[0], modulo=Order)

    # step ISC1
    setupIntermediaryMsgFromJson = GroupSetupIntermediaryMessage()
    setupIntermediaryMsgFromJson.from_json(setupIntermediaryMsg.to_json())

    for playerNumber in range(1, len(playerUris)):
        gpSecPlayerBuilder[playerNumber].processSetupIntermediaryMessages(intermediarymessage=setupIntermediaryMsgFromJson)
        # step ISC2
        playergrpSetupResponseIntermediaryMessage = gpSecPlayerBuilder[playerNumber].getIntermediaryMessageResponse(ephemeral_key=ephemeral_key_shares[playerNumber], blinding_key=blinding_key_shares[playerNumber], modulo=Order)
        playergrpSetupResponseIntermediaryMessageFromJson = GroupSetupResponseIntermediaryMessage()
        playergrpSetupResponseIntermediaryMessageFromJson.from_json(playergrpSetupResponseIntermediaryMessage.to_json())
        proposerBuilder.processSetupIntermediaryMessages(intermediarymessage=playergrpSetupResponseIntermediaryMessageFromJson)

    # step ISC3
    proposerBroadcastIntermediaryMsg = proposerBuilder.getBroadcastIntermediaryMessage()
    proposerBroadcastIntermediaryMsgFromJson = GroupBroadcastIntermediaryMessage()
    proposerBroadcastIntermediaryMsgFromJson.from_json(proposerBroadcastIntermediaryMsg.to_json())

    for playerNumber in range(1, len(playerUris)):
        gpSecPlayerBuilder[playerNumber].processSetupIntermediaryMessages(intermediarymessage=proposerBroadcastIntermediaryMsgFromJson)

    #Step : dump Intermediary shares
    proposerBuilder.dumpSetupIntermediaryMessages()

    #Calculate Intermediate share : Curve point
    ''' Send Intermediary shares(curvePoint) as below 
        step P1GEN : proposer generates Intermediary shares 
        step ISC1: proposer(proposer) ---> Player2

        step ISC3: Player2 -->its share --> proposer
        
        step ISC5: proposer(proposer) --broadcast aggregrated Intermediary shares-> Player2
     '''
    #step P1GEN 
    setupCurvePointIntermediaryMsg = proposerBuilder.getSetupIntermediaryShareCurvePointMessage(blinding_key=blinding_key_shares[0])

    # step ISC1
    setupCurvePointIntermediaryMsgFromJson = GroupSetupIntermediaryMessage()
    setupCurvePointIntermediaryMsgFromJson.from_json(setupCurvePointIntermediaryMsg.to_json())

    for playerNumber in range(1, len(playerUris)):
        gpSecPlayerBuilder[playerNumber].processSetupIntermediaryCurvePointMessages(intermediarymessage=setupCurvePointIntermediaryMsgFromJson)
        # step ISC3
        playergrpSetupResponseCurvePointIntermediaryMessage = gpSecPlayerBuilder[playerNumber].getIntermediaryCurvePointMessageResponse(blinding_key=blinding_key_shares[playerNumber])
        playergrpSetupResponseCurvePointIntermediaryMessageFromJson = GroupSetupResponseIntermediaryMessage()
        playergrpSetupResponseCurvePointIntermediaryMessageFromJson.from_json(playergrpSetupResponseCurvePointIntermediaryMessage.to_json())
        proposerBuilder.processSetupIntermediaryCurvePointMessages(intermediarymessage=playergrpSetupResponseCurvePointIntermediaryMessageFromJson)

    # step ISC5
    proposerBroadcastCurvePointIntermediaryMsg = proposerBuilder.getBroadcastIntermediaryCurvePointMessage()
    proposerBroadcastCurvePointIntermediaryMsgFromJson = GroupBroadcastIntermediaryMessage()
    proposerBroadcastCurvePointIntermediaryMsgFromJson.from_json(proposerBroadcastCurvePointIntermediaryMsg.to_json())

    for playerNumber in range(1, len(playerUris)):
        gpSecPlayerBuilder[playerNumber].processSetupIntermediaryCurvePointMessages(intermediarymessage=proposerBroadcastCurvePointIntermediaryMsgFromJson)

    #Step : dump Intermediary shares curve points
    proposerBuilder.dumpSetupIntermediaryCurvePointMessages()

    # STEP : Threshold Signature Generation
    msg = 'A quick brown fox jumps over lazy the dog'

    # Generate share of Signature by each player
    players_signature_share_list = []

    # Generate share of Signature by each player
    t1 = datetime.datetime.now()
    for playerNumber in range(0, len(playerUris)):
        # For each player, generate Intermediary Share Interpolation
        gpSecPlayerBuilder[playerNumber].generateIntermediaryShareInterpolation()
        # For each player, generate Intermediary Share Curve Point Interpolation
        gpSecPlayerBuilder[playerNumber].generateIntermediaryShareCurveInterpolation()
        # For each player, generate ECDSA R
        gpSecPlayerBuilder[playerNumber].ecdsa_r()
        players_signature_share_list.append(gpSecPlayerBuilder[playerNumber].getShareOfSignature(msg=msg, key_share=key_shares[playerNumber], ephemeral_key=ephemeral_key_shares[playerNumber], modulo=Order))
    print("Took "+str(datetime.datetime.now() -t1)+ " seconds to finish Interpolation activity by all "+ str(len(playerUris)) +" players")

    # Set the current share of signature of proposer
    proposerBuilder.setSignatureShare(signature_share=players_signature_share_list[0])

    for playerNumber in range(1, len(playerUris)):
        # Let non coordinating players send their respective signature shares to the proposer
        SignatureShareMsg = gpSecPlayerBuilder[playerNumber].getSignatureShareMessage(signature_share=players_signature_share_list[playerNumber])
        SignatureShareMsgFromJson = GroupSignatureShareMessage()
        SignatureShareMsgFromJson.from_json(SignatureShareMsg.to_json())
        proposerBuilder.processSignatureShareMessage(signature_share_msg=SignatureShareMsgFromJson)

    '''
    print("==== Signature Shares that Player 1 received ======")
    proposerBuilder.dumpSignatureShareMessage()
    print("=======================================") 
    '''

    # Player1 will calculate interpolation of all signatures, and get at 0
    print("============================================================================================")
    print("Signature Interpolate value ==> " + str(proposerBuilder.generateSignatureInterpolation()))
    print("============================================================================================")

    player_a0_msgs = []
    for playerNumber in range(0, len(playerUris)):
        # Let each player get his/her PublicSecretA0
        playera0Msg = gpSecPlayerBuilder[playerNumber].getPublicSecretA0Message()
        playera0MsgFromJson = GroupSecretPublicMessage()
        playera0MsgFromJson.from_json(playera0Msg.to_json())
        player_a0_msgs.append(playera0MsgFromJson)

    for playerNumber in range(1, len(playerUris)):
        proposerBuilder.processPublicSecretA0Message(player_a0_msgs[playerNumber-1])

    # get the broadcast message from proposer
    proposera0BcastMsg = proposerBuilder.getPublicSecretA0Message()
    proposera0BcastMsgFromJson = GroupSecretPublicMessage()
    proposera0BcastMsgFromJson.from_json(proposera0BcastMsg.to_json())

    for playerNumber in range(1, len(playerUris)):
        # let  non-corrdinating players know about the broadcasted message from proposer
        gpSecPlayerBuilder[playerNumber].processPublicSecretA0Message(publicSecretA0=proposera0BcastMsgFromJson)

    msgEncoded =  bytes(msg, 'utf-8')

    FiniteGroup.ec_sig_verify(msg=msgEncoded,
                              pubKey=proposerBuilder.calculatePublicKeyUsingSecretA0(),
                              r_input=proposerBuilder.ecdsa_r(),
                              s_input=proposerBuilder.generateSignatureInterpolation()
                              )
    '''
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

    verifyIt = Nakasendo.verify(msg, player1Builder.calculatePublicKeyUsingSecretA0(), sig[0], sig[1])
    if (verifyIt == True):
        print ('msg verified')
    #Hm = FiniteGroup.hash_mod(msg, Order)
    msgEncoded =  bytes(msg, 'utf-8')

    FiniteGroup.ec_sig_verify(msg=msgEncoded,
                              pubKey=player1Builder.calculatePublicKeyUsingSecretA0(), 
                              r_input=player1Builder.ecdsa_r(), 
                              s_input=player1Builder.generateSignatureInterpolation()
                             )
    '''



###################################################################################################
