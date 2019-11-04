
from __future__ import print_function

import uuid





#==========================================================================
# Example Server Code
# NB: this is example code only.  Does not cover 
#       - authentication, 
#       - security, 
#       - timer events 
#     These are all available under Twisted libraries. 
#       - maintaining active connections (Player disconnects not monitored)
#       - assumes nobody is being dishonest ('spoofing' other players, etc)
#==========================================================================



class GroupMetadata :

    def __init__ (self, id, proposer, m, n, t) :
        self.id = id
        self.m = m                  # recombination number
        self.n = n                  # total number        
        self.t = t                  # degree of polynomial
        self.numPlayerReplies = 0
        self.numSecretReplies = 0
        self.participantList = []
        self.groupIsSet = False
        self.proposer = proposer
        self.collatedEvals = {}
        self.collatedHidden = {}
        self.numCollatedReplies = 0

    def __str__(self):
        string =  ("Group Metadata for " + str(self.id) + " :" \
            + "\n\tm =  " + str(self.m) \
            + "\n\tn =  " + str(self.n) \
            + "\n\tt =  " + str(self.t) \
            + "\n\tgroupIsSet =  " + str(self.groupIsSet) \
            + "\n\tproposer =  " + self.proposer \
            + "\n\tnumPlayerReplies = " + str(self.numPlayerReplies) \
            + "\n\tnumSecretReplies = " + str(self.numSecretReplies) \
            + "\n\tparticipantList = " )
            
        string += (', '.join(self.participantList ))
        string += "\n\tnumCollatedReplies = " + str(self.numCollatedReplies) 
        return string



#-----------------------------------------------------------------
# Orchestrator class 
# Contains core logic for orchestrating (co-ordination) Threshold Signature
class Orchestrator( ) :

    #-------------------------------------------------
    def __init__ (self) :
        print("__init__ Orchestrator")
        # dictionary of { user:remoteReference }
        self.users  = {}
        # dictionary of { groupID:GroupMetaData }
        self.groups = {}


    #-------------------------------------------------
    # Register user with Orchestrator
    def register(self, username, ref):

        if username not in self.users:
            self.users[username] = ref 
        
        print ('{0} registered '.format(username))



    #-------------------------------------------------
    # m = recombination number of private key
    # n = total number in group
    # t = degree of polynomial (dervied from m-1 )
    def createGroup(self, proposer, m, n):
        print('createGroup: {0} of {1}'.format(m, n ))


        # check parameters are valid
        t = int(m) - 1

        if t < 1 :
            errMsg = 'Parameters are not valid! Failed check: t (degree of Polynomial) < 1'
            print( errMsg )
            raise Exception( errMsg )

        if not (2 * t) + 1 <= int(n) :
            errMsg = 'Parameters are not valid! Failed check: 2t + 1 <= n'
            print( errMsg )
            raise Exception( errMsg )

        # check the #players is > 2
        if int(n) <= 2 :
            errMsg = 'Parameters are not valid! Failed check: group_size > 1'
            print( errMsg )
            raise Exception( errMsg )

        groupId = str(uuid.uuid1())
        self.groups[groupId] = GroupMetadata(groupId, proposer, int(m), int(n), t)

        # proposer automatically gets into group.  Invite rest of users
        self.groups[groupId].participantList.append(proposer)

        invitees = []
        for user in self.users :
            if  user != proposer :
                    invitees.append(self.users[user])
  
        return groupId, invitees




    #-------------------------------------------------
    def acceptInvite(self, user, groupID, acceptance ) :
           
        group       = self.groups[groupID]
        target      = group.n - 1           # participant is already part of group

        print('invitation reply from {0} ({1} : {2})'.format(user, acceptance, groupID))

        if not group.groupIsSet :
            if acceptance :
                group.numPlayerReplies += 1
                # assume 'user' is valid
                group.participantList.append(user)
            
            print('number of replies = {0}'.format(group.numPlayerReplies))

            if group.numPlayerReplies == target :
                group.groupIsSet = True
                return True

        return False

    
    #-------------------------------------------------
    def getParticipants(self, groupID ) :
        return self.groups[groupID].participantList 

    #-------------------------------------------------
    def validGroup(self, groupID) :
        if groupID in self.groups :
            return True
        return False 
    
    #-------------------------------------------------
    def getUserReferences(self, groupID ) :
        participants = self.getParticipants(groupID) 
        references = []
        for participant in participants :
            references.append(self.users[participant])
        return references 

    #-------------------------------------------------
    def getUserRef(self, participant) :
        return self.users[participant]

    #-------------------------------------------------
    # Returns a list of users which is the participant List, 
    # without the ordinal
    def getGroupIsSetParameters(self, user, groupID):        

        group = self.groups[groupID]

        # create list of ordinals 
        start = 1
        playerIds = list( range( start, start + len(group.participantList ) ) )

        ordinal = group.participantList.index( user ) + 1
        listExcludingUser = list(playerIds)
        listExcludingUser.remove(ordinal)
                
        return ordinal, listExcludingUser, group.t

    #-------------------------------------------------
    # collate data
    def collateData(self,  groupId, ordinal, evals, hiddenPoly ) :

        group = self.groups[groupId]

        group.collatedEvals[ordinal] = evals
        group.collatedHidden[ordinal] = hiddenPoly
        group.numCollatedReplies += 1

        print("number encrypted coefficient replies = {0}".format(group.numCollatedReplies))
        if group.numCollatedReplies == group.n :
            print("Received all encrypted coefficient data, distribute")
            return True 
        return False
    
    #-------------------------------------------------
    def getCollatedData(self, groupId ) :
        group = self.groups[groupId ]
        return group.collatedEvals, group.collatedHidden

    #-------------------------------------------------
    # This is here as a placeholder
    # todo - Chandra?
    def secretVerification(self, data) : 

        print("secretVerification complete")
        

    def groupError(self, message):
        print ("Error: {0}".format(message))
        # do some stuff, delete the group / mark as being errored
        # tell clients this group is not good





