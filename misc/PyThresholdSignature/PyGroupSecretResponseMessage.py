from PyMessage import Message
from PyUUID import UUID
import json
from PyGroupState import GroupState
from PyPolynomials import PolynomialVals

class GroupSecretResponseMessage:

    def __init__(self, MsgType = Message.Type.GROUP_SECRET_RESPONSE, uri = None, groupID = None, response = None, GrpState = GroupState.Type.UNKNOWN):
        self.base = MsgType
        self.mResponderUri = uri
        self.mGroupID = groupID
        if (groupID is None):
          self.mGroupID = UUID().getUUIDString()
        self.mResponse = GrpState
        self.polynomialFunctions = PolynomialVals()

    def getMsgType(self):
        return self.base

    def setMsgType(self, MsgType):
        self.base = MsgType

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

    def addPolynomialFunction(self, byUrl, forUrl, polyVal):
      self.polynomialFunctions.addPolynomialFunction(byUrl, forUrl, polyVal)

    def removePolynomialFunction(self, byUrl, forUrl, polyVal=None):
        self.polynomialFunctions.removePolynomialFunction(byUrl, forUrl, polyVal)

    def clearPolynomialFunction(self):
        self.polynomialFunctions.clearPolynomialFunction()

    def getPolynomialFunction(self):
        return self.polynomialFunctions.getPolynomialFunction()

    def __str__(self):
        val = ""
        val += str(self.getID())+"\n\n"
        val += str(self.getResponderURI())+"\n\n"
        val += str(self.base)+"\n\n"
        val += str(self.mResponse)+"\n\n"
        val += str(self.polynomialFunctions)
        return val

    def jsonObjectType(self):
        return "GroupSecretResponseMessage"

    def to_json(self):
        gpRsMsgDt = {}
        gpRsMsgDt["Type"] = Message.getStrByType(self.base)
        gpRsMsgDt["Responder"] = self.getResponderURI()
        gpRsMsgDt["GroupID"] = self.getID()
        gpRsMsgDt["Response"] = GroupState.getStrByType(self.getResponse())
        gpRsMsgDt["Polynomials"] = self.polynomialFunctions.to_json()
        return json.dumps(gpRsMsgDt)

    def from_json(self, gpRsMsgStr):
        gpRsMsgDt = json.loads(gpRsMsgStr)
        self.base = Message.getTypeByStr(gpRsMsgDt["Type"])
        self.setResponderURI(gpRsMsgDt["Responder"])
        self.setID(gpRsMsgDt["GroupID"])
        self.setResponse(GroupState.getTypeByStr(gpRsMsgDt["Response"]))
        self.polynomialFunctions.from_json(gpRsMsgDt["Polynomials"])
        '''
        for polyValStr in gpRsMsgDt["Polynomials"]:
          # get into List
          polyValList = polyValStr.split(", ") 
          self.addPolynomialFunction(polyValList[0], polyValList[1], int(polyValList[2])) 
        '''

