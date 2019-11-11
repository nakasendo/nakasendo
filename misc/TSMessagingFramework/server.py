
from twisted.spread import pb
from twisted.internet import reactor
from twisted.python import log
from orchestrator import Orchestrator
import sys

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
# Orchestrator Protocol 
# Contains functionality for transmitting and receiving data,
# using Twisted libaray (See Twisted Perspective Broker for more information) 
class OrchestratorProtocol( pb.Root ) :

    #-------------------------------------------------
    def __init__ (self) :
        log.startLogging(sys.stdout)
        log.msg("Server Running...")
        self.orchestrator = Orchestrator( )


    #-------------------------------------------------
    # Register user with Orchestrator
    def remote_register(self, username, ref):
        self.orchestrator.register( username, ref )


    #-------------------------------------------------
    def remote_createGroup(self, proposer, m, n):
        try :
            results =  self.orchestrator.createGroup(proposer, m, n)
        except Exception as inst:
            raise OrchestratorError( inst.args )

        groupId = results[0]
        inviteList = results[1]

        for invitee in inviteList :
            invitee.callRemote("invite", groupId).addCallback \
                    (self.acceptInvite)

        return groupId 

    #-------------------------------------------------
    def acceptInvite(self, data ) :
        
        user        = data[0]
        groupID     = data[1]
        acceptance  = data[2]    

        if self.orchestrator.acceptInvite(user, groupID, acceptance) :
            self.groupIsSet(groupID)

    #-------------------------------------------------
    def groupIsSet(self, groupID):        

        # send message to all users who are part of this group 
        participants = self.orchestrator.getParticipants(groupID)
        for user in participants :
            params = self.orchestrator.getGroupIsSetParameters( user, groupID )

            print ('sending groupIsSet message to {0}'.format(user))
            
            self.orchestrator.getUserRef(user).callRemote \
                ( "groupIsSet", groupID.encode(), params[0], params[1], params[2] ) 
        return

    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def remote_sharePublicKey( self, user, groupId, calcType ) :
        groupId = groupId.decode()
        self.orchestrator.calcType = calcType.decode()         

        if not self.orchestrator.validGroup(groupId) :
            errMsg = 'Group Id is not valid: {0}'.format(groupId)
            raise OrchestratorError( errMsg )

        participants = self.orchestrator.getParticipants(groupId)

        if user not in participants :
            errMsg = 'user is not in the group: {0}'.format(user)
            raise OrchestratorError( errMsg ) 
        
        userRefs = self.orchestrator.getUserReferences(groupId)
        for ref in userRefs :
            ref.callRemote("requestData", groupId, self.orchestrator.calcType).addCallback \
                (self.collateData)        
        return 

    #-------------------------------------------------
    # collate data
    def collateData(self,  data ) :
        groupId     = data[0]
        ordinal     = data[1]
        evals       = data[2] 
        hiddenPoly  = data[3]
        hiddenEvals = data[4]

        # if True then ready to distribute data
        if self.orchestrator.collateData( groupId, ordinal, evals, hiddenPoly, hiddenEvals) :
            collatedData = self.orchestrator.getCollatedData(groupId) 
            
            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( groupId )
            for ref in userRefs :
                ref.callRemote( "createSecret", groupId, self.orchestrator.calcType, collatedData[0], collatedData[1], collatedData[2])\
                    .addCallbacks(self.secretVerification, self.verificationError)

    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def remote_presigning( self, user, groupId, calcType ) :
        
        groupId = groupId.decode()
        self.orchestrator.calcType = calcType.decode() 
       
        print("presigning: user={0}, groupId={1}, calcType={2}".format(user, groupId, self.orchestrator.calcType))

        if not self.orchestrator.validGroup(groupId) :
            errMsg = 'Group Id is not valid: {0}'.format(groupId)
            raise OrchestratorError( errMsg )

        participants = self.orchestrator.getParticipants(groupId)

        if user not in participants :
            errMsg = 'user is not in the group: {0}'.format(user)
            raise OrchestratorError( errMsg ) 

        userRefs = self.orchestrator.getUserReferences(groupId)
        for ref in userRefs :
            ref.callRemote("requestData", groupId, self.orchestrator.calcType).addCallback \
                (self.collateData)        
        return

    #-------------------------------------------------
    # Receives verification from all group members
    def secretVerification(self, data):

        user        = data[0]
        groupId     = data[1]

        if self.orchestrator.secretVerification(user, groupId) :
            print("secretVerification complete for: {0}".format(self.orchestrator.calcType))

            # contact all the group participants with verification success
            userRefs = self.orchestrator.getUserReferences(groupId)
            for ref in userRefs :
                ref.callRemote("groupIsVerified", groupId, self.orchestrator.calcType)


    def verificationError(self, reason):
        print ("Error: calcType={0}, from client: {1}".format( self.orchestrator.calcType, reason ))
        # do some stuff, delete the group / mark as being errored
        # tell clients this group is not good


    def remote_collateVWData( self, groupId, ordinal, data ) :
        print("Collating VW Data")
        groupId = groupId.decode()
        if self.orchestrator.collateVWData( groupId, ordinal, data ) :
            collatedData = self.orchestrator.getCollatedVWData( groupId )

            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( groupId )
            for ref in userRefs :
                ref.callRemote( "sharedVWData", groupId, collatedData)

        
    def remote_ephemeralKeyCompleted( self, groupId, user ) :
        groupId = groupId.decode()
        print("EphemeralKey has been completed, groupId = {0}, user = {1}".format \
            (groupId, user))
        
        if self.orchestrator.allEphemeralKeysCompleted( user, groupId ) :
            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( groupId )
            for ref in userRefs :
                ref.callRemote( "completed", groupId)

    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def remote_sign( self, user, groupId, msg ) :
        print("remote_sign")
        groupId = groupId.decode()
        msg = msg.decode()
       
        print("sign: user={0}, groupId={1}, msg={2}".format(user, groupId, msg))

        if not self.orchestrator.validGroup(groupId) :
            errMsg = 'Group Id is not valid: {0}'.format(groupId)
            raise OrchestratorError( errMsg )

        participants = self.orchestrator.getParticipants(groupId)

        # check the user is in the group, then set the signer
        if user not in participants :
            errMsg = 'user is not in the group: {0}'.format(user)
            raise OrchestratorError( errMsg ) 
        self.orchestrator.setSigner(groupId, user)

        userRefs = self.orchestrator.getUserReferences(groupId)
        for ref in userRefs :
            ref.callRemote("requestSignatureData", groupId, msg).addCallback \
                (self.signingCallback)        
        return

    # Signing callback with data: 
    def signingCallback(self,  data ) :
        print("signingCallback")
        groupId     = data[0]
        ordinal     = data[1]
        sig         = data[2]
        msg         = data[3]

        print("\ngroupId={0}, ordinal={1}, sig={2}, msg={3}".format(groupId, ordinal, sig, msg))
        
        # if True then ready to distribute data
        if self.orchestrator.signature( groupId, ordinal, sig) :
            signatureData = self.orchestrator.getSignatureData(groupId) 
            
            # send the signature data out to the signer
            userRef = self.orchestrator.getSignerReference( groupId )
            userRef.callRemote( "readyToSign", groupId, msg, signatureData) \
                .addCallbacks(self.signingCompletedCallback, self.signingErrorCallback)

    def signingCompletedCallback ( self, gid ) :
        print("signingCompletedCallback: gid = {0}".format(gid))

    def signingErrorCallback ( self, gid ) :
        print("signingErrorCallback: gid = {0}".format(gid))        

#-----------------------------------------------------------------

if __name__ == '__main__':

    reactor.listenTCP(8790, pb.PBServerFactory(OrchestratorProtocol()))
    reactor.run()


