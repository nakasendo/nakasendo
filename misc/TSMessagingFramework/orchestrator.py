
from __future__ import print_function

import uuid

from twisted.spread import pb
from twisted.internet import reactor



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
        return string

#-----------------------------------------------------------------
# Error class 
class OrchestratorError(pb.Error):
    """This is an Expected Exception. Something bad happened."""
    pass

#-----------------------------------------------------------------
# Exception class 
class OrchestraterException(Exception):
    """This is an Unexpected Exception. Something really bad happened."""
    pass

#-----------------------------------------------------------------
# Orchestrator class 
class Orchestrator( pb.Root ) :

    #-------------------------------------------------
    def __init__ (self) :
        print("Orchestrator Running...")
        # dictionary of { user:remoteReference }
        self.users  = {}
        # dictionary of { groupID:GroupMetaData }
        self.groups = {}


    #-------------------------------------------------
    # Register user with Orchestrator
    def remote_register(self, username, ref):

        if username not in self.users:
            self.users[username] = ref 
        
        print ('{0} registered '.format(username))



    #-------------------------------------------------
    # m = recombination number of private key
    # n = total number in group
    # t = degree of polynomial (dervied from m-1 )
    def remote_createGroup(self, proposer, m, n):
        print('createGroup: {0} of {1}'.format(m, n ))

        # check parameters are valid, pull this out to BL
        t = int(m) - 1
        if not (2 * t) + 1 <= int(n) :
            errMsg = 'Parameters are not valid! Failed check: 2t + 1 <= n'
            print( errMsg )
            raise OrchestratorError( errMsg )

        id = str(uuid.uuid1())
        self.groups[id] = GroupMetadata(id, proposer, int(m), int(n), t)

        for keys,values in self.groups.items():
            print(values)

        # proposer automatically gets into group.  Invite rest of users
        self.groups[id].participantList.append(proposer)
        for user in self.users :
            if  user != proposer :
                self.users[user].callRemote("invite", id).addCallback \
                    (self.acceptInvite)
        
        return id 

    #-------------------------------------------------
    def acceptInvite(self, data ) :
        
        user        = data[0]
        groupID     = data[1]
        acceptance  = data[2]        
        
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

                for keys,values in self.groups.items():
                    print(values)

                self.groupIsSet(groupID)


    #-------------------------------------------------
    def groupIsSet(self, groupID):        

        group = self.groups[groupID]

        # create list of ordinals 
        start = 1
        ordinalList = list( range( start, start + len(group.participantList ) ) )

        # send message to all users who are part of this group 
        for user in group.participantList :
            index = group.participantList.index( user ) + 1
            listExcludingUser = list(ordinalList)
            listExcludingUser.remove(index)
                  
            print ('sending groupIsSet message to {0}'.format(user))
            self.users[user].callRemote \
                ( "groupIsSet", groupID.encode(), index, listExcludingUser ) \
                    .addCallback(self.shareSecret)
        return

    #-------------------------------------------------
    def shareSecret(self, data): 
        user        = data[0]
        groupID     = (data[1]).decode()
        message     = data[2]

        print('shareSecret reply from {0} ({1} : {2})'.format(user, groupID, data))        
        
        group       = self.groups[groupID]
        target      = group.m

        group.numSecretReplies += 1
        print('number of replies = {0}'.format(group.numSecretReplies))

        if group.numSecretReplies == target :
            self.collateCoefficients(groupID)

    #-------------------------------------------------
    def collateCoefficients(self, groupID) :
        print("Collating coefficients...")
        print("...finished")

        group = self.groups[groupID]

        # distribute coefficients to all users in group
        print ('sending listCoefficients message to: ' + ', '.join(group.participantList))
        for user in group.participantList :
            
            self.users[user].callRemote \
                ("listCoefficients", groupID.encode(), self.groups[groupID].participantList.index(user)) \
                    #.addCallback(self.shareSecret)
        return


#-----------------------------------------------------------------

if __name__ == '__main__':

    reactor.listenTCP(8790, pb.PBServerFactory(Orchestrator()))
    reactor.run()


