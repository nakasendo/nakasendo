from enum import Enum
import json

class GroupState:

  class Type(Enum):
    UNKNOWN=1
    PROPOSED=2
    ACCEPTED=3
    REJECTED=4

  def __init__(self, type = Type.UNKNOWN):
    self.mType = type

  def getType(self):
    return self.mType

  @staticmethod
  def getTable():
     table = { 
          GroupState.Type.UNKNOWN: "Unknown",
          GroupState.Type.PROPOSED: "Proposed",
          GroupState.Type.ACCEPTED: "Accepted",
          GroupState.Type.REJECTED: "Rejected"
     }
     return table

  def to_json(self):
    j = {}
    j["Type"] = GroupState.getStrByType(self.mType)
    return json.dumps(j)
  
  def from_json(self, typeStr):
    j = json.loads(typeStr)
    self.mType = GroupState.getTypeByStr(j["Type"])
  
  def __str__(self):
    return str(GroupState.getStrByType(self.mType))

  def __eq__(self, other):
    return (GroupState.getStrByType(self.mType) == GroupState.getStrByType(other.getType)) 


  @staticmethod
  def getStrByType(val):
    return GroupState.getTable()[val]

  @staticmethod
  def getTypeByStr(val):
    for _type, _str in GroupState.getTable().items():
        if (_str == val):
            return _type
    return GroupState.Type.UNKNOWN




'''
msg = GroupState(GroupState.Type.PROPOSED)
type = msg.to_json()
msgGS = GroupState(GroupState.Type.REJECTED)
msgGS.from_json(type)
print(msgGS)
'''
