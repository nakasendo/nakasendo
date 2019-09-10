from PyMessage import Message
import json
import ecdsa
import ecdsa.ellipticcurve
#import Polynomial
import FiniteGroup

def curveToJson(curve=None):
    if (curve is None or (not isinstance(curve, ecdsa.ellipticcurve.CurveFp))):
      return None
    curveDt = {}
    curveDt['p'] = curve.p()
    curveDt['a'] = curve.a()
    curveDt['b'] = curve.b()
    return json.dumps(curveDt)

def curveFromJson(curveDtStr):
    curveDt = json.loads(curveDtStr)
    return ecdsa.ellipticcurve.CurveFp(p=curveDt['p'], a=curveDt['a'], b=curveDt['b'])


def PointToJson(point=None):
    if (point is None or (not isinstance(point, ecdsa.ellipticcurve.Point))):
      return None
    #Point( self.__curve, self.__x, self.__y, self.__order ), where __curve = CurveFp(p, a, b) , __x, __y and __order(could be Null) are of int types
    pointDt = {}
    pointDt['val_type'] = 'Point'
    #pointDt['curve'] = curveToJson(point.curve())
    pointDt['x'] = point.x()
    pointDt['y'] = point.y()
    pointDt['order'] = point.order()
    return json.dumps(pointDt)

def PointFromJson(pointDt):
    if (isinstance(pointDt, str)):
        pointDt = json.loads(pointDt)
    #return ecdsa.ellipticcurve.Point(curveFromJson(pointDt['curve']), pointDt['x'], pointDt['y'], pointDt['order'])
    return ecdsa.ellipticcurve.Point(FiniteGroup.curve_secp256k1, pointDt['x'], pointDt['y'], pointDt['order'])


class IntermediaryMessages:

    def __init__(self):
        self.intermediary_shares = {}

    def addIntermediaryMessage(self, ordinal, intermediary_share):
        self.intermediary_shares[ordinal] = intermediary_share

    def deleteIntermediaryMessage(self, ordinal):
        del self.intermediary_shares[ordinal]

    def clearIntermediaryMessage(self):
        self.intermediary_shares.clear()

    def getIntermediaryShares(self):
        return self.intermediary_shares

    def __str__(self):
        val = ""
        for _ordinal in self.intermediary_shares.keys():
            val += "\n" + str(_ordinal) + " : " + str(self.intermediary_shares[_ordinal])
        return val

    def to_json(self):
        isDt = {}
        for ordinal, share in self.intermediary_shares.items():
            if (isinstance(share, ecdsa.ellipticcurve.Point)):
                isDt[ordinal] = PointToJson(share)
            else:
                isDt[ordinal] = json.dumps({"val_type" : "int", "val" : share})
        return json.dumps(isDt)

    def from_json(self, intermediaryKeys):
        for key, val in json.loads(intermediaryKeys).items():
            val = json.loads(val)
            if(val["val_type"] == "int"):
                self.intermediary_shares[int(key)] = val["val"]
            elif(val["val_type"] == "Point"):
                self.intermediary_shares[int(key)] = PointFromJson(val)

class GroupSetupIntermediaryMessage:

    def __init__(self, ordinal=-1, uri=None, groupID=None, intermediary_share=None, MsgType = Message.Type.PRE_SIGNATURE_INTERMEDIARY):
        self.base = MsgType
        self.myUri = uri
        self.mGroupID = groupID
        self.ordinal = ordinal
        if (self.ordinal != -1):
            self.intermediary_shares = IntermediaryMessages()
            self.intermediary_shares.addIntermediaryMessage(self.ordinal, intermediary_share)

    def getMyUrl(self):
        return self.myUri

    def getOrdinal(self):
        return self.ordinal

    def getGroupID(self):
        return self.mGroupID

    def getIntermediaryShares(self):
        return self.intermediary_shares

    def __str__(self):
        val = ""
        val += str(self.base) + "\n"
        val += str(self.myUri) + "\n"
        val += str(self.mGroupID) + "\n"
        val += str(self.intermediary_shares)
        return val

    def to_json(self):
        imStMsgDt = {}
        imStMsgDt['Type'] = Message.getStrByType(self.base)
        imStMsgDt['uri'] = self.myUri
        imStMsgDt['GroupID'] = self.mGroupID
        imStMsgDt['IntermediaryShares'] = self.intermediary_shares.to_json()
        return json.dumps(imStMsgDt)

    def from_json(self, imStMsgStr):
        imStMsgDt = json.loads(imStMsgStr)
        self.base =  Message.getTypeByStr(imStMsgDt["Type"])
        self.myUri = imStMsgDt['uri']
        self.mGroupID = imStMsgDt['GroupID']
        _is = IntermediaryMessages()
        _is.from_json(imStMsgDt['IntermediaryShares'])
        self.intermediary_shares = _is

class GroupSetupResponseIntermediaryMessage:

    def __init__(self, uri=None, groupID=None, MsgType = Message.Type.PRE_SIGNATURE_INTERMEDIARY_RESPONSE):
        self.base = MsgType
        self.myUri = uri
        self.mGroupID = groupID
        self.intermediary_shares = IntermediaryMessages()


    def getMyUrl(self):
        return self.myUri

    def getGroupID(self):
        return self.mGroupID

    def getIntermediaryShares(self):
        return self.intermediary_shares

    def __str__(self):
        val = ""
        val += str(self.base) + "\n"
        val += str(self.myUri) + "\n"
        val += str(self.mGroupID) + "\n"
        val += str(self.intermediary_shares)
        return val


    def to_json(self):
        imRpMsgDt = {}
        imRpMsgDt['Type'] = Message.getStrByType(self.base)
        imRpMsgDt['uri'] = self.myUri
        imRpMsgDt['GroupID'] = self.mGroupID
        imRpMsgDt['IntermediaryShares'] = self.intermediary_shares.to_json()
        return json.dumps(imRpMsgDt)

    def from_json(self, imRpMsgStr):
        imRpMsgDt = json.loads(imRpMsgStr)
        self.base =  Message.getTypeByStr(imRpMsgDt["Type"])
        self.myUri = imRpMsgDt['uri']
        self.mGroupID = imRpMsgDt['GroupID']
        _is = IntermediaryMessages()
        _is.from_json(imRpMsgDt['IntermediaryShares'])
        self.intermediary_shares = _is


class GroupBroadcastIntermediaryMessage(GroupSetupResponseIntermediaryMessage):
    pass


class GroupSignatureShareMessage:

    def __init__(self, ordinal=-1, uri=None, groupID=None, signature_share=None):
        self.myUri = uri
        self.mGroupID = groupID
        self.ordinal = ordinal
        self.signature_share = signature_share

    def getMyUrl(self):
        return self.myUri

    def getOrdinal(self):
        return self.ordinal

    def getGroupID(self):
        return self.mGroupID

    def getSignatureShare(self):
        return self.signature_share

    def __str__(self):
        val = ""
        val += str(self.myUri) + "\n"
        val += str(self.mGroupID) + "\n"
        val += str(self.signature_share)
        return val

    def to_json(self):
        imStMsgDt = {}
        imStMsgDt['uri'] = self.myUri
        imStMsgDt['ordinal'] = self.ordinal
        imStMsgDt['GroupID'] = self.mGroupID
        imStMsgDt['SignatureShare'] = self.signature_share
        return json.dumps(imStMsgDt)

    def from_json(self, imStMsgStr):
        imStMsgDt = json.loads(imStMsgStr)
        self.myUri = imStMsgDt['uri']
        self.mGroupID = imStMsgDt['GroupID']
        self.ordinal = int(imStMsgDt['ordinal'])
        self.signature_share = int(imStMsgDt['SignatureShare'])

class GroupSecretPublicMessage:

    def __init__(self, ordinal=-1, uri=None, groupID=None, player_public_a0=None):
        self.myUri = uri
        self.mGroupID = groupID
        self.ordinal = ordinal
        if (player_public_a0 is None):
            self.player_public_a0 = {}
        else:
            self.player_public_a0 = player_public_a0

    def getMyUrl(self):
        return self.myUri

    def getOrdinal(self):
        return self.ordinal

    def getGroupID(self):
        return self.mGroupID

    def getPublicSecretA0(self):
        return self.player_public_a0

    def to_json(self):
        gspmDt = {}
        gspmDt['uri'] = self.myUri
        gspmDt['ordinal'] = self.ordinal
        gspmDt['GroupID'] = self.mGroupID
        isDt = {}
        for ordinal, public_a0 in self.player_public_a0.items():
            if (isinstance(public_a0, ecdsa.ellipticcurve.Point)):
                isDt[ordinal] = PointToJson(public_a0)
        gspmDt['publicShare'] = json.dumps(isDt)
        return json.dumps(gspmDt)

    def from_json(self, gspmDtStr):
        gspmDt = json.loads(gspmDtStr)
        self.myUri = gspmDt['uri']
        self.mGroupID = gspmDt['GroupID']
        self.ordinal = int(gspmDt['ordinal'])
        for ordinal, val in json.loads(gspmDt['publicShare']).items():
            if (isinstance(val, str)):
                val = json.loads(val)
            if(val["val_type"] == "Point"):
                self.player_public_a0[int(ordinal)] = PointFromJson(val)