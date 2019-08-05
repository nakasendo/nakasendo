from enum import Enum
import json


class Message:

  class Type(Enum):
    UNKNOWN=1
    GROUP_SETUP=2
    GROUP_RESPONSE=3
    SHARED_SECRET_SETUP=4
    PRE_SIGNATURE_SETUP=5
    POOL_BUILDER_SETUP=6
    PRE_SIGNATURE_INTERMEDIARY=7
    POOL_BUILDER_INTERMEDIARY=8

  def __init__(self, type = Type.UNKNOWN):
    self.mType = type

  def getType(self):
    return self.mType

  def to_json(self):
    j = {}
    j["Type"] = Message.getStrByType(self.mType)
    return json.dumps(j)
  
  def from_json(self, typeStr):
    j = json.loads(typeStr)
    self.mType = Message.getTypeByStr(j["Type"])
  
  def __str__(self):
    return str(Message.getStrByType(self.mType))

  def __eq__(self, other):
    return (Message.getStrByType(self.mType) == Message.getStrByType(other.getType)) 

  @staticmethod
  def getTable():
    table = { 
      Message.Type.UNKNOWN: "Unknown",
      Message.Type.GROUP_SETUP:"GroupSetup",
      Message.Type.GROUP_RESPONSE:"GroupResponse",
      Message.Type.SHARED_SECRET_SETUP:"SharedSecretSetup",
      Message.Type.PRE_SIGNATURE_SETUP:"PreSignatureSetup",
      Message.Type.POOL_BUILDER_SETUP:"PoolBuilderSetup",
      Message.Type.PRE_SIGNATURE_INTERMEDIARY:"PreSignatureIntermediary",
      Message.Type.POOL_BUILDER_INTERMEDIARY:"PoolBuilderIntermediary"
    }
    return table

  @staticmethod
  def getStrByType(val):
    return Message.getTable()[val]

  @staticmethod
  def getTypeByStr(val):
    for _type, _str in Message.getTable().items():
        if (_str == val):
            return _type
    return Message.Type.UNKNOWN

  


'''
msg = Message(Message.Type.GROUP_RESPONSE)
type = msg.to_json()
msgGS = Message(Message.Type.GROUP_SETUP)
msgGS.from_json(type)
print(msgGS)
'''
