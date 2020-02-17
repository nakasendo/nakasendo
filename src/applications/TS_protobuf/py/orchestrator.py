
from __future__ import print_function

import uuid




#==========================================================================
# Example Server Code
# NB: this is example code only.  Does not cover 
#       - authentication, 
#       - security, 
#       - timer events 
#       - maintaining active connections (Player disconnects not monitored)
#       - assumes nobody is being dishonest ('spoofing' other players, etc)
#==========================================================================



class GroupMetadata :

    def __init__ (self, id, proposer, m, n, t) :
        self.id = id
        self.m  = m                  # recombination number
        self.n  = n                  # total number        
        self.t  = t                  # degree of polynomial
        self.numReplies             = 0
        self.participantList        = []
        self.groupIsSet             = False
        self.signatureIsSet         = False
        self.proposer               = proposer
        self.evalsCounter           = 0
        self.collatedHiddenPolys    = {}
        self.collatedHiddenEvals    = {}
        self.collatedVWs            = {}
        self.collatedSignatures     = {}
        self.calculationType        = ''
        self.signer                 = ''
        self.locked                 = False

    def __str__(self):
        string =  ("Group Metadata for " + str(self.id) + " :" \
            + "\n\tm =  " + str(self.m) \
            + "\n\tn =  " + str(self.n) \
            + "\n\tt =  " + str(self.t) \
            + "\n\tgroupIsSet       = " + str(self.groupIsSet) \
            + "\n\tproposer         = " + self.proposer \
            + "\n\tnumReplies = " + str(self.numReplies) \
            + "\n\tparticipantList  = " )
            
        string += (', '.join(self.participantList ))
        string += "\n\tcalculationType = " + str(self.calculationType ) 
        return string



#-----------------------------------------------------------------
# Orchestrator class 
# Contains core logic for orchestrating (co-ordination) Threshold Signature
class Orchestrator( ) :

    #-------------------------------------------------
    def __init__ (self) :
        print("starting orchestrator")
        # dictionary of { user:remoteReference }
        self.users  = {}
        # dictionary of { groupId:GroupMetaData }
        self.groups = {}


    #-------------------------------------------------
    # Register user with Orchestrator
    def register(self, username, ref):

        if username not in self.users:
            self.users[username] = ref 
        
        print ('{0} registered '.format(username))


    def isLocked( self, groupId ) :
        return self.groups[groupId].locked


    def lock( self, groupId ) :
        print("Group LOCKED (groupId = {0}".format(groupId))
        self.groups[groupId].locked = True

    def unLock( self, groupId ) :
        print("Group UN-LOCKED (groupId = {0}".format(groupId))
        self.groups[groupId].locked = False

    def setCalcType( self, groupId, calcType ) :
        self.groups[groupId].calculationType = calcType

    def calcType( self, groupId ) :
        return self.groups[groupId].calculationType

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
    def acceptInvite(self, user, groupId, acceptance ) :
           
        group       = self.groups[groupId]
        target      = group.n - 1           # participant is already part of group

        print('invitation reply from {0} ({1} : {2})'.format(user, acceptance, groupId))

        if not group.groupIsSet :
            if acceptance :
                group.numReplies += 1
                # assume 'user' already exist do not allow into group
                if user in group.participantList :
                    print("{0} already exists in group".format(user))
                    return False
                
                group.participantList.append(user)
                
            
            print('number of replies = {0}'.format(group.numReplies))

            if group.numReplies == target :
                group.groupIsSet = True
                self.receivedAllReplies(groupId)
                return True

        return False

    
    #-------------------------------------------------
    def getParticipants(self, groupId ) :
        return self.groups[groupId].participantList 

    #-------------------------------------------------
    def validGroup(self, groupId) :
        if groupId in self.groups :
            return True
        return False 
    
    #-------------------------------------------------
    def getUserReferences(self, groupId ) :
        participants = self.getParticipants(groupId) 
        print('participants = {0}'.format(participants))
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
    def getGroupIsSetParameters(self, user, groupId):        

        group = self.groups[groupId]

        # create list of ordinals 
        start = 1
        playerIds = list( range( start, start + len(group.participantList ) ) )

        ordinal = group.participantList.index( user ) + 1
        listExcludingUser = list(playerIds)
        listExcludingUser.remove(ordinal)
                
        return ordinal, listExcludingUser, group.t

    def getDegree(self, groupId):
        group = self.groups[groupId]

        return group.t


    #-------------------------------------------------
    # get list of users ordinal : reference
    def getPtpReferences(self, user, groupId ) :
        print('gePtpReferences')
        group           = self.groups[groupId]
        participants    = list(group.participantList )
        participants.remove(user)

        print('user= {0}'.format(user))
        print('participants= {0}'.format(participants))

        ret = {}

        for p in participants :
            ordinal = group.participantList.index( p ) + 1
            reference = self.users[p]
            ret[ordinal] = reference
        print('ret from getPtpReferences = {0}'.format(ret))
        return ret


    #-------------------------------------------------
    # collate data
    def collateData(self,  groupId, ordinal, hiddenPoly, hiddenEvals) :

        group = self.groups[groupId]

        group.collatedHiddenPolys[ordinal] = hiddenPoly
        group.collatedHiddenEvals[ordinal] = hiddenEvals
        group.numReplies += 1

        print("number encrypted coefficient replies = {0}".format(group.numReplies))
        if group.numReplies == group.n :
            print("Received all encrypted coefficient data, distribute")
            self.receivedAllReplies( groupId )
            return True 
        return False


    #-------------------------------------------------    
    def allEvalsReceived( self, groupId, ordinal ) :
        group = self.groups[groupId]

        group.evalsCounter += 1
        print("evalsCounter = {0}".format(group.evalsCounter ))
        if group.evalsCounter == group.n :
            print("All Players have received their evals, continue!")
            return True
        return False

    
    #-------------------------------------------------
    def getCollatedData(self, groupId) :
        group = self.groups[groupId ]
        return group.collatedHiddenPolys, group.collatedHiddenEvals

    #-------------------------------------------------
    # collate V and W data
    def collateVWData(self,  groupId, ordinal, data) : 
        group = self.groups[groupId]

        group.collatedVWs[ordinal] = data
        group.numReplies += 1

        print("number VW data replies = {0}".format(group.numReplies))
        if group.numReplies == group.n :
            print("Received all VW data, distribute")
            self.receivedAllReplies( groupId )
            return True 
        return False
    
    #-------------------------------------------------
    # get collated V and W data
    def getCollatedVWData(self, groupId) :
        group = self.groups[groupId ]
        return group.collatedVWs


    #-------------------------------------------------
    # collate signature data
    def signature( self, groupId, ordinal, sig) :
        print("groupId = {0}, ordinal={1}, signature={2}".format(groupId, ordinal, sig))
        group = self.groups[groupId]

        if not group.signatureIsSet :

            group.collatedSignatures[ordinal] = sig
            group.numReplies += 1 

            print("number signature replies = {0}".format(group.numReplies))
            two_t_plus_1 = (group.t * 2) + 1

            if group.numReplies == two_t_plus_1 :
                group.signatureIsSet = True
                print("Received 2t+1 replies")
                self.receivedAllReplies(groupId)                
                return True

        return False



    #-------------------------------------------------
    # get collated signature data
    def getSignatureData(self, groupId) :
        group = self.groups[groupId ]
        return group.collatedSignatures


    #-------------------------------------------------
    # This is here as a placeholder
    def secretVerification(self, user, groupId) :

        group = self.groups[groupId]
        group.numReplies += 1


        print("Secret verification from: {0}, number replies = {1}".format(user, group.numReplies))
        if group.numReplies == group.n :
            print("Received all secret verfications")
            self.receivedAllReplies( groupId )
            return True

        return False

    def groupError(self, message):
        print ("Error: {0}".format(message))
        # do some stuff, delete the group / mark as being errored
        # tell clients this group is not good


    def allEphemeralKeysCompleted( self, user, groupId) :
        group = self.groups[groupId]
        group.numReplies += 1


        print("Ephemeral Keys Completed from: {0}, number replies = {1}".format(user, group.numReplies))
        if group.numReplies == group.n :
            self.receivedAllReplies( groupId )
            return True

        return False


    def receivedAllReplies( self, groupId) :
        print("resetting replies counter")
        self.groups[groupId].numReplies = 0     
        self.groups[groupId].evalsCounter = 0     
        
    def clearSignatureSetFlag(self, groupid):
        print ("Signature is complete ... reset the flag signatureIsSet")
        self.groups[groupid].signatureIsSet = False

        
    def setSigner( self, groupId, signer ) :
        self.groups[groupId].signer = signer 

    def getSignerReference( self, groupId ) :
        participants = self.getParticipants(groupId) 
        signer = self.groups[groupId].signer
        print("signer = {0}".format(signer))
        print("participants = {0}".format(participants))
        
        userref = self.users[signer] 

        return userref 

       

        
