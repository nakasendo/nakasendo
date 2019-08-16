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
from PyGroupSetupMessage import GroupSetupMessage, GroupSetupResponseMessage, GroupBroadMessage


class GroupSecretPlayerBuilder:
    #Construct for the given list of players.
    def __init__(self, myUri = None, playerUris = None, seretSetupMsg = None, groupBuilder = None ):
        self.mutex = RLock()
        self.mState  = GroupState.Type.PROPOSED
        self.mMyUri = myUri
        self.mPlayers = PlayerMap()
        self.ownPolynomialFunctions = {}
        self.coeffEntries = []
        self.receivedPolynomialFunctions=[] # received polynomials, a list of tuples, where each each tuple is of format - byUrl, forUrl, PolyValue

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

        # Make functions for all players
        for playerItem in self.mPlayers.getItems():
            self.ownPolynomialFunctions[playerItem.getPlayer().getURI()] = self.rp(playerItem.getPlayer().getOrdinal())

        if (groupBuilder != None):
            groupBuilder.registerGroupSecetShareBuilder(groupSecetBuilder=self, guid=self.mGroupID)

        print("================"+self.mMyUri+"================")
        # dump own polynomialFunctions
        #for item in self.ownPolynomialFunctions.items():
        #    print(item)

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
    # Test lifecycle of group acceptance
    gb1 = GroupBuilder(uri="player1@mycorp.com", ordinal=1)
    setupMsg = gb1.getInitialSetupMessage()
    setupMsgFromJson = GroupSetupMessage()
    setupMsgFromJson.from_json(setupMsg.to_join())

    gb2 = GroupBuilder(uri="player2@otherplace.org", ordinal=2)
    gb3 = GroupBuilder(uri="player3@secret.org", ordinal=3)

    gb2.processInitialSetupMessage(setupMsgFromJson)
    gb3.processInitialSetupMessage(setupMsgFromJson)

    gb2SetupRes = gb2.getInitialSetupResponseMessage()
    gb3SetupRes = gb3.getInitialSetupResponseMessage()

    gb2SetupResFromJson = GroupSetupResponseMessage()
    gb3SetupResFromJson = GroupSetupResponseMessage()

    gb2SetupResFromJson.from_json(gb2SetupRes.to_json())
    gb3SetupResFromJson.from_json(gb3SetupRes.to_json())


    gb1.processInitialSetupResponseMessage(gb2SetupResFromJson)
    gb1.processInitialSetupResponseMessage(gb3SetupResFromJson)

    bcMsg = gb1.getBroadcastSetupMessage()
    bcMsgFromJson = GroupBroadMessage()
    bcMsgFromJson.from_json(bcMsg.to_json())

    gb2.processBroadMessage(bcMsgFromJson)
    gb3.processBroadMessage(bcMsgFromJson)

    # At this point, gb1, gb2 and gb3 represents group player at player player1@mycorp.com", "player2@otherplace.org", "player3@secret.org sites, repectively.


    player1Builder = GroupSecretPlayerBuilder(playerUris=["player1@mycorp.com", "player2@otherplace.org", "player3@secret.org"], myUri="player1@mycorp.com", groupBuilder=gb1)
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
    player2Builder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgForPlayer2, myUri="player2@otherplace.org", groupBuilder=gb2)
    assert player2Builder != None
    player3Builder = GroupSecretPlayerBuilder(seretSetupMsg=setupMsgForPlayer3, myUri="player3@secret.org", groupBuilder=gb3)
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

    player2ResponseFromJson =  GroupSecretResponseMessage()
    player2ResponseFromJson.from_json(player2Response.to_json())

    player3ResponseFromJson =  GroupSecretResponseMessage()
    player3ResponseFromJson.from_json(player3Response.to_json())

    ## Ideally, player2Response and player3Response be converted into JSON and be back to JSON as in the real world.

    player1Builder.processGroupSetupResponse(response=player2ResponseFromJson)
    player1Builder.processGroupSetupResponse(response=player3ResponseFromJson)

    player2PolyFuncs = player1Builder.getPolyFunctionsOfThePlayer("player2@otherplace.org")
    player3PolyFuncs = player1Builder.getPolyFunctionsOfThePlayer("player3@secret.org")

    print("++++++++++++++++++++++++++++++++++")
    print(player2PolyFuncs) 
    print(player2PolyFuncs.to_json()) 
    player2PolyFuncsFromPlayer1= GroupSecretPolynomialVals()
    player2PolyFuncsFromPlayer1.from_json(player2PolyFuncs.to_json()) 
    print("++++++++++++++++++++++++++++++++++")
    print(player3PolyFuncs) 
    print(player3PolyFuncs.to_json()) 
    player3PolyFuncsFromPlayer1= GroupSecretPolynomialVals()
    player3PolyFuncsFromPlayer1.from_json(player3PolyFuncs.to_json()) 
    print("++++++++++++++++++++++++++++++++++")
    assert player1Builder.getState() == GroupState.Type.ACCEPTED
    assert player2Builder.getState() == GroupState.Type.PROPOSED
    assert player3Builder.getState() == GroupState.Type.PROPOSED
    player2Builder.processPolyFunctionsOfThePlayer(player2PolyFuncsFromPlayer1)
    player3Builder.processPolyFunctionsOfThePlayer(player3PolyFuncsFromPlayer1)
    print(player1Builder.getKeyShare())
    print(player2Builder.getKeyShare())
    print(player3Builder.getKeyShare())
    assert player1Builder.getState() == GroupState.Type.ACCEPTED
    assert player2Builder.getState() == GroupState.Type.ACCEPTED
    assert player3Builder.getState() == GroupState.Type.ACCEPTED
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
