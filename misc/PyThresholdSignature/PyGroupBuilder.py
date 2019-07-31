from PyMessage import Message
from PyUUID import UUID
from PyGroup import PlayerItem, PlayerMap, Group
from PyPlayer import Player
from PyGroupState import GroupState
from PyGroupSetupMessage import GroupSetupMessage
from PyGroupResponseMessage import GroupResponseMessage
import json
from threading import RLock


class GroupBuilder:
    #Construct for the given list of players.
    def __init__(self, myUri = None, playerUris = None, setupMsg = None ):
        self.mutex = RLock()
        self.mState  = GroupState.Type.PROPOSED
        self.mMyUri = myUri
        self.mPlayers = PlayerMap()

        if (playerUris == None and setupMsg == None and myUri != None):
            raise ValueError(" Invalid Constructor")

        if (playerUris == None and setupMsg == None and myUri == None):
            return

        if (setupMsg == None):
            self.mGroupID = UUID().getUUIDString()
            self.mProposerUri = myUri
            self.proposerIsPlayer = False
            ordinal = 1
            for uri in playerUris:
                playerItem = PlayerItem(uri, Player(uri, ordinal))
                ordinal = ordinal + 1
                if (self.proposerIsPlayer == False and uri == self.mMyUri):
                    self.proposerIsPlayer = True
                    # Proposer is ACCEPTED right away
                    playerItem.getPlayer().setAccepted()

                #if (self.mPlayers.getCount() > 0 and playerItem in self.mPlayers and playerItem.getPlayer().getURI() == uri):
                for item in self.mPlayers.getItems():
                    if (item.getPlayer().getURI() == uri):
                        raise ValueError("Failed to create and insert player details for " + uri)

                self.mPlayers.add(playerItem)

            if (self.proposerIsPlayer == False):
                raise ValueError("The group proposer must be a member of the proposed group")
        else:
            self.mGroupID = setupMsg.getID()
            self.mPlayers = setupMsg.getPlayers()
            self.mProposerUri = setupMsg.getProposerURI()
            if (self.mPlayers.findPlayer(myUri) == None):
                raise ValueError("The group proposer must be a member of the proposed group")

        if (len(self.mPlayers.getItems()) <= 1):
            raise ValueError("A group should have more than one player ")


    def __eq__(self, other):
        if not isinstance(other, GroupBuilder):
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

    def getGroup(self):
        self.mutex.acquire()
        try:
            if (self.mState != GroupState.Type.ACCEPTED):
                raise RuntimeError("Can't create group in state " + GroupState.getStrByType(self.mState))

            return Group(self.mGroupID, self.mPlayers)
        finally:
            self.mutex.release()

    def getGroupSetupMessage(self):
        self.mutex.acquire()
        try:
            return GroupSetupMessage(uri = self.mProposerUri, groupID = self.mGroupID, playerMap = self.mPlayers)
        finally:
            self.mutex.release()

    def getGroupSetupResponseMessage(self, state):
        if (state != GroupState.Type.ACCEPTED and state != GroupState.Type.REJECTED):
            raise ValueError("Group response must be either ACCEPTED or REJECTED")
        response = GroupResponseMessage(uri = self.mMyUri, groupID = self.mGroupID, GrpState = state)
        self.processGroupSetupResponse(response)
        return response

    def processGroupSetupResponse(self, response):
        self.mutex.acquire()
        try :
            #Is response from someone in our group?
            if (response.getID() != self.mGroupID):
                raise RuntimeError("Group response message is not for our group")

            player = self.mPlayers.findPlayer(response.getResponderURI()) 
            if (player == None):
                raise RuntimeError("Group response message is not from a proposed group member")

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
        return "GroupBuilder"

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

'''
gb = GroupBuilder(playerUris=["p1", "p2", "p3", "p4"], myUri="p2")
bpStr = gb.to_json()

#Make sure that the proposer should be the member of the group.
try:
  gb = GroupBuilder(playerUris=["p1", "p2", "p3", "p4"], myUri="p5")
except ValueError:
    pass

# Make sure that the single player group isnt allowed
try:
  gb = GroupBuilder(playerUris=["p1"], myUri="p1")
except ValueError:
    pass

# Make sure that the players with same URIs can't be allowed in the group.
try:
    gb = GroupBuilder(playerUris=["p1", "p1", "p3", "p4"], myUri="p1")
except ValueError:
    pass


try:
  p1 = Player("p1", 1, False, False)
  p11 = PlayerItem("player1@mycorp.com", p1)
  playersMap = PlayerMap()
  playersMap.add(p11)
  gp = GroupSetupMessage( uri="player1@mycorp.com", playerMap=playersMap)
  gb = GroupBuilder(setupMsg = gp, myUri="p1")
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

gp = GroupSetupMessage( uri="player1@mycorp.com", playerMap=playersMap)
gb = GroupBuilder(setupMsg = gp, myUri="p2")
gbStr = gb.to_json()

gbFJson = GroupBuilder()
gbFJson.from_json(gbStr);
# make sure that to_json and from_json works fine.
assert (gbFJson == gb)


# LifeCycle tests 

###################################################################################################
# Test lifecycle of group acceptance

player1Builder = GroupBuilder(playerUris=["player1@mycorp.com", "player2@otherplace.org", "player3@secret.org"], myUri="player1@mycorp.com")
assert player1Builder.getPlayers().getCount() == 3
assert player1Builder.getProposerURI() == "player1@mycorp.com"
assert player1Builder.getMyURI() == "player1@mycorp.com"
assert player1Builder.getState() == GroupState.Type.PROPOSED
assert player1Builder.getPlayers().findPlayer("player1@mycorp.com").accepted() == True 
#print(player1Builder.getResponsesOfAllPlayers())

#Get group setup message

setupMsg = player1Builder.getGroupSetupMessage()
assert setupMsg != None
setupMsgData = setupMsg.to_json()
#print(setupMsg)
setupMsgForPlayer2 = GroupSetupMessage()
setupMsgForPlayer2.from_json(setupMsgData)
setupMsgForPlayer3 = GroupSetupMessage()
setupMsgForPlayer3.from_json(setupMsgData)

#Create builders from setup message for each non-proposer player
player2Builder = GroupBuilder(setupMsg=setupMsgForPlayer2, myUri="player2@otherplace.org")
assert player2Builder != None
player3Builder = GroupBuilder(setupMsg=setupMsgForPlayer3, myUri="player3@secret.org")
assert player3Builder != None

assert player1Builder == player2Builder
assert player1Builder == player3Builder

nonMemberResponse = GroupResponseMessage(uri="weirdo@who.ru", groupID = player1Builder.getID(), GrpState = GroupState.Type.ACCEPTED)
try:
  player1Builder.processGroupSetupResponse(response=nonMemberResponse)
except RuntimeError:
    pass

#Test acceptance responses from each player in turn
player2Response = player2Builder.getGroupSetupResponseMessage(state=GroupState.Type.ACCEPTED)
player3Response = player3Builder.getGroupSetupResponseMessage(state=GroupState.Type.ACCEPTED)

#Player 2 responses
player1Builder.processGroupSetupResponse(response=player2Response)
player3Builder.processGroupSetupResponse(response=player2Response);
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
player1Builder = GroupBuilder(playerUris=["player1@mycorp.com", "player2@otherplace.org", "player3@secret.org"], myUri="player1@mycorp.com")
assert player1Builder.getPlayers().getCount() == 3
assert player1Builder.getProposerURI() == "player1@mycorp.com"
assert player1Builder.getMyURI() == "player1@mycorp.com"
assert player1Builder.getState() == GroupState.Type.PROPOSED
assert player1Builder.getPlayers().findPlayer("player1@mycorp.com").accepted() == True 
setupMsg = player1Builder.getGroupSetupMessage()
assert setupMsg != None
setupMsgData = setupMsg.to_json()
#print(setupMsg)
setupMsgForPlayer2 = GroupSetupMessage()
setupMsgForPlayer2.from_json(setupMsgData)
setupMsgForPlayer3 = GroupSetupMessage()
setupMsgForPlayer3.from_json(setupMsgData)

#Create builders from setup message for each non-proposer player
player2Builder = GroupBuilder(setupMsg=setupMsgForPlayer2, myUri="player2@otherplace.org")
assert player2Builder != None
player3Builder = GroupBuilder(setupMsg=setupMsgForPlayer3, myUri="player3@secret.org")
assert player3Builder != None

assert player1Builder == player2Builder
assert player1Builder == player3Builder

#Test acceptance responses from each player in turn
player2Response = player2Builder.getGroupSetupResponseMessage(state=GroupState.Type.ACCEPTED)
player3Response = player3Builder.getGroupSetupResponseMessage(state=GroupState.Type.REJECTED)

#Player 2 responses
player1Builder.processGroupSetupResponse(response=player2Response)
player3Builder.processGroupSetupResponse(response=player2Response);
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
player1Builder = GroupBuilder(playerUris=["player1@mycorp.com", "player2@otherplace.org", "player3@secret.org"], myUri="player1@mycorp.com")
setupMsg = player1Builder.getGroupSetupMessage() 
builder2 = GroupBuilder(setupMsg =setupMsg, myUri="player2@otherplace.org")
try:
  builder2.getGroupSetupResponseMessage(GroupState.Type.PROPOSED)
except ValueError:
  pass
'''
