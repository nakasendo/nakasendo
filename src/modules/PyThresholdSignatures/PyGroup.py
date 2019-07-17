import json
from PyUUID import UUID
from PyPlayer import Player

class PlayerItem:
    def __init__(self, playerID = None,  player = None):
      self.playerID = playerID 
      self.player = player

    def getPlayer(self):
        return self.player

    def getPlayerID(self):
        return self.playerID

    def __eq__(self, other):
        if not isinstance(other, PlayerItem):
            return False
        return self.playerID == other.playerID and self.getPlayer() == other.getPlayer()

class PlayerMap:
    def __init__(self):
        self.items = []

    def add(self, playeritem):
        self.items.append(playeritem)

    def getItems(self):
        return self.items

    def getPlayersSortedByOrdinal(self):
        if (len(self.items) == 0):
            return None

        self.items.sort(key=lambda x:x.getPlayer().getOrdinal())
        sortedPlayers = [item.getPlayer() for item in self.items]
        return sortedPlayers 

    def getPlayerFromOrdinal(self, ordinal = 0):
        if (ordinal == 0 or ordinal > len(self.items)):
            return None
        self.getPlayersSortedByOrdinal()
        for item in self.items:
            if (item.getPlayer().getOrdinal() == ordinal):
                return item
        return None

class Group:

    def __init__(self, mGroupID = None, playerMap = None):
        self.mGroupID = mGroupID 
        if(self.mGroupID  is None):
            self.mGroupID = UUID() 
        self.mPlayers = playerMap
        if (playerMap is None):
            self.mPlayers = PlayerMap()

    def getPlayerFromOrdinal(self):
        return self.mPlayers.getPlayerFromOrdinal()

    def getID(self):
        return self.mGroupID  

    def setID(self, mGroupID):
        self.mGroupID = mGroupID

    def getPlayers(self):
        return self.mPlayers

    def clearPlayers(self):
        self.mPlayers = None

    def size(self):
        return len(self.mPlayers.getItems())

    def __eq__(self, other):
        if not isinstance(other, Group):
            return False
        return (self.getID() == other.getID()) and (self.getPlayers().getPlayersSortedByOrdinal() == other.getPlayers().getPlayersSortedByOrdinal())


    def to_json(self):
        gpDt = {}
        gpDt["ID"] = self.getID()
        playerJson = []
        for playerItem in self.getPlayers().getItems():
            playerJson.append(playerItem.player.to_json())

        gpDt["Players"] = playerJson
        return json.dumps(gpDt)

    def from_json(self, gpDtStr):
        gpDt = json.loads(gpDtStr)
        self.setID(gpDt["ID"])
        for playerDict in gpDt["Players"] :
            p = Player()
            p.from_json(playerDict)
            pi = PlayerItem(p.getURI(), p)
            self.getPlayers().add(pi)

    def __str__(self):
        val = ""
        val += self.getID()+"\n\n"
        for playerItem in self.getPlayers().getItems():
            val +=  str(playerItem.player)+"\n\n"
        return val



'''
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

_uuid = UUID().getUUIDString()
gp = Group(_uuid, playersMap)
#print(gp.to_json())
data=gp.to_json()
#print(data)
gpJson = Group()
gpJson.from_json(data)
#print(gpJson)
print(gpJson == gp)
'''
