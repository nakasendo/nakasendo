from enum import Enum

class GroupState(Enum):
   UNKNOWN=1
   PROPOSED=2
   ACCEPTED=3
   REJECTED=4
 
   @staticmethod
   def enumTable(val):
     table = { 
               GroupState.UNKNOWN: "Unknown",
               GroupState.PROPOSED: "Proposed",
               GroupState.ACCEPTED: "Accepted",
               GroupState.REJECTED: "Rejected"
        }
     return table[val]


#print(GroupState.enumTable(GroupState.REJECTED))

