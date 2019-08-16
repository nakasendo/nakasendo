
from PyUUID import UUID
import json
from PyGroupSetupMessage import GroupSetupMessage, GroupSetupResponseMessage, GroupBroadMessage



class GroupBuilder:

    def __init__(self, uri, ordinal, groupID = None, setupMsg = None):
        if (groupID is None):
            groupID = UUID().getUUIDString()
       
        self.mMyUri = uri
        self.ordinal = ordinal
        self.mGroupID = groupID
        self.playerInfo = {} # ordinal : player_url
        self.secretShares = {} # key : share
        self.playerInfo[ordinal] = uri

    def getInitialSetupMessage(self):
        setupMsg = GroupSetupMessage(uri=self.mMyUri, groupID = self.mGroupID, ordinal=self.ordinal)
        return setupMsg

    def getPlayerInfo(self):
        return self.playerInfo

    def findPlayer(self, uri):
        for _ordinal, _uri in self.getPlayerInfo().items():
            if (_uri == uri):
                return _ordinal
        return -1

    def processInitialSetupMessage(self, setupMsg):
        if (setupMsg != None):
            self.mGroupID = setupMsg.getGroupID()
            self.playerInfo[setupMsg.getOrdinal()] = setupMsg.getMyUrl()

    def getInitialSetupResponseMessage(self):
        resMsg = GroupSetupResponseMessage(uri=self.mMyUri, groupID=self.mGroupID, ordinal=self.ordinal)
        return resMsg

    def processInitialSetupResponseMessage(self, resMsg):
        if (resMsg != None and  resMsg.getGroupID() == self.mGroupID):
            self.playerInfo[resMsg.getOrdinal()] = resMsg.getMyUrl()

    def getBroadcastSetupMessage(self):
        bcastMsg = GroupBroadMessage(uri=self.mMyUri, groupID=self.mGroupID, ordinal=self.ordinal, playerInfo=self.playerInfo)
        return bcastMsg

    def processBroadMessage(self, bcastMsg):
        if (bcastMsg != None and bcastMsg.getGroupID() == self.mGroupID):
            for playerMsg in bcastMsg.getPlayerInfo().items():
                self.playerInfo[int(playerMsg[0])] = playerMsg[1]

    def  addSecretShareEntry(self, key, value):
        self.secretShares[key] = value

    def getSecretShareEntry(self, key):
        return self.secretShares[key]

    def registerGroupSecetShareBuilder(self, groupSecetBuilder, guid=None):

        if (guid == None):
            guid = UUID().getUUIDString()

        self.secretShares[UUID().getUUIDString()] = groupSecetBuilder

    def unRegisterGroupSecetShareBuilder(self, guid):
        del self.secretShares[guid]

    def deleteAllGroupSecetShareBuilders(self):
        self.secretShares.clear()



if __name__ == '__main__':
    gb1 = GroupBuilder(uri="player1@mycorp.com", ordinal=1)
    setupMsg = gb1.getInitialSetupMessage()

    #print(setupMsg.to_join())
    setupMsgFromJson = GroupSetupMessage()
    setupMsgFromJson.from_json(setupMsg.to_join())

    #print(setupMsgFromJson)
    #print(setupMsgFromJson.to_join())

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

