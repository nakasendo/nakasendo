from PyMessage import Message
from PyUUID import UUID
from PyGroup import PlayerItem, PlayerMap, Group
from PyPlayer import Player
import json

class GroupSetupMessage:

    def __init__(self, threshold_value=-1, MsgType = Message.Type.GROUP_SETUP, ordinal=-1, uri = None, groupID = None):
        self.base = MsgType
        self.myUri = uri
        if (groupID is None):
            groupID = UUID().getUUIDString()
        self.mGroupID = groupID
        self.ordinal = ordinal
        self.threshold_value = threshold_value


    def getMyUrl(self):
        return self.myUri

    def getOrdinal(self):
        return self.ordinal

    def getGroupID(self):
        return self.mGroupID

    def getThresholdValue(self):
        return self.threshold_value

    def __str__(self):
        val = ""
        val += str(self.base) + "\n"
        val += str(self.myUri) + "\n"
        val += str(self.ordinal) + "\n"
        val += str(self.mGroupID) + "\n"
        val += str(self.threshold_value) + "\n"
        return val

    def to_json(self):
        gpStMsgDt = {}
        gpStMsgDt['Type'] = Message.getStrByType(self.base)
        gpStMsgDt['uri'] = self.myUri
        gpStMsgDt['ordinal'] = self.ordinal
        gpStMsgDt['GroupID'] = self.mGroupID
        gpStMsgDt['ThresholdValue'] = self.threshold_value
        return json.dumps(gpStMsgDt)

    def from_json(self, gpStMsgStr):
        gpStMsgDt = json.loads(gpStMsgStr)
        self.base =  Message.getTypeByStr(gpStMsgDt["Type"])
        self.myUri = gpStMsgDt['uri']
        self.mGroupID = gpStMsgDt['GroupID']
        self.ordinal = gpStMsgDt['ordinal']
        self.threshold_value = gpStMsgDt['ThresholdValue']

class GroupSetupResponseMessage:

    def __init__(self, MsgType = Message.Type.GROUP_SETUP_RESPONSE, uri = None, groupID = None, ordinal = -1):
        self.base = MsgType
        self.myUri = uri
        if (groupID is None):
            groupID = UUID().getUUIDString()
        self.mGroupID = groupID
        self.ordinal = ordinal

    def getMyUrl(self):
            return self.myUri

    def getOrdinal(self):
            return self.ordinal

    def getGroupID(self):
            return self.mGroupID
    def __str__(self):
        val = ""
        val += str(self.base) + "\n"
        val += str(self.myUri) + "\n"
        val += str(self.ordinal) + "\n"
        val += str(self.mGroupID) + "\n"


    def to_json(self):
        gpRpMsgDt = {}
        gpRpMsgDt['Type'] = Message.getStrByType(self.base)
        gpRpMsgDt['uri'] = self.myUri
        gpRpMsgDt['ordinal'] = self.ordinal
        gpRpMsgDt['GroupID'] = self.mGroupID
        return json.dumps(gpRpMsgDt)

    def from_json(self, gpRpMsgStr):
        gpRpMsgDt = json.loads(gpRpMsgStr)
        self.base =  Message.getTypeByStr(gpRpMsgDt["Type"])
        self.myUri = gpRpMsgDt['uri']
        self.mGroupID = gpRpMsgDt['GroupID']
        self.ordinal = gpRpMsgDt['ordinal']


class GroupBroadcastMessage(GroupSetupMessage):

    def __init__(self, MsgType = Message.Type.GROUP_SETUP_RESPONSE, uri = None, groupID = None, ordinal = -1, playerInfo = None):
        self.base = MsgType
        self.myUri = uri
        if (groupID is None):
            groupID = UUID().getUUIDString()
        self.mGroupID = groupID
        self.ordinal = ordinal
        self.playerInfo = playerInfo

    def getPlayerInfo(self):
        return self.playerInfo

    def __str__(self):
        val = ""
        val += str(self.base) + "\n"
        val += str(self.myUri) + "\n"
        val += str(self.ordinal) + "\n"
        val += str(self.mGroupID) + "\n"
        for item in self.playerInfo.items():
            val += str(item)

    def to_json(self):
        gpStMsgDt = {}
        gpStMsgDt['Type'] = Message.getStrByType(self.base)
        gpStMsgDt['uri'] = self.myUri
        gpStMsgDt['ordinal'] = self.ordinal
        gpStMsgDt['GroupID'] = self.mGroupID
        gpStMsgDt['players'] = json.dumps(self.playerInfo)
        return json.dumps(gpStMsgDt)

    def from_json(self, gpStMsgStr):
        gpStMsgDt = json.loads(gpStMsgStr)
        self.base =  Message.getTypeByStr(gpStMsgDt['Type'])
        self.myUri = gpStMsgDt['uri']
        self.mGroupID = gpStMsgDt['GroupID']
        self.ordinal = gpStMsgDt['ordinal']
        self.playerInfo = json.loads(gpStMsgDt['players'])

