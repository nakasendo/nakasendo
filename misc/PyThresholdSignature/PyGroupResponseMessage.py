from PyMessage import Message
from PyUUID import UUID
import json
from PyGroupState import GroupState

class GroupResponseMessage:

    def __init__(self, MsgType = Message.Type.GROUP_RESPONSE, uri = None, groupID = None, response = None, GrpState = GroupState.Type.UNKNOWN):
        self.base = MsgType
        self.mResponderUri = uri
        self.mGroupID = groupID
        if (groupID is None):
          self.mGroupID = UUID().getUUIDString()
        self.mResponse = GrpState

    def getResponderURI(self):
        return self.mResponderUri

    def setResponderURI(self, uri):
        self.mResponderUri = uri

    def getID(self):
        return self.mGroupID

    def setID(self, mGroupID):
        self.mGroupID = mGroupID

    def getResponse(self):
        return self.mResponse

    def setResponse(self, GrpState):
        self.mResponse = GrpState

    def __str__(self):
        val = ""
        val += str(self.getID())+"\n\n"
        val += str(self.getResponderURI())+"\n\n"
        val += str(self.base)+"\n\n"
        val += str(self.mResponse)+"\n\n"
        return val

    def jsonObjectType(self):
        return "GroupResponseMessage"

    def to_json(self):
        gpRsMsgDt = {}
        gpRsMsgDt["Type"] = Message.getStrByType(self.base)
        gpRsMsgDt["Responder"] = self.getResponderURI()
        gpRsMsgDt["GroupID"] = self.getID()
        gpRsMsgDt["Response"] = GroupState.getStrByType(self.getResponse())
        return json.dumps(gpRsMsgDt)

    def from_json(self, gpRsMsgStr):
        gpRsMsgDt = json.loads(gpRsMsgStr)
        self.base = Message.getTypeByStr(gpRsMsgDt["Type"])
        self.setResponderURI(gpRsMsgDt["Responder"])
        self.setID(gpRsMsgDt["GroupID"])
        self.setResponse(GroupState.getTypeByStr(gpRsMsgDt["Response"]))

