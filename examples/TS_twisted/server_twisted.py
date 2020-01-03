
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
    # Send group is set message to all users in the group
    def groupIsSet(self, groupID):        

        participants = self.orchestrator.getParticipants(groupID)
        for user in participants :
            params = self.orchestrator.getGroupIsSetParameters( user, groupID )

            log.msg ('sending groupIsSet message to {0}'.format(user))
            
            self.orchestrator.getUserRef(user).callRemote \
                ( "groupIsSet", groupID.encode(), params[0], params[1], params[2] ) 
        return

    #--------------------------------------------------
    # operations available to remote in this section
    #-------------------------------------------------- 

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
                    (self.acceptInviteCallback)

        return groupId 


    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def remote_sharePublicKey( self, user, groupId, calcType ) :
        groupId = groupId.decode()
        self.orchestrator.setCalcType (groupId, calcType.decode())         

        if not self.orchestrator.validGroup(groupId) :
            errMsg = 'Group Id is not valid: {0}'.format(groupId)
            raise OrchestratorError( errMsg )

        if self.orchestrator.isLocked(groupId) :
            errMsg = 'Group Id is locked, try again later: {0}'.format(groupId)
            raise OrchestratorError( errMsg )
        
        self.orchestrator.lock(groupId)

        participants = self.orchestrator.getParticipants(groupId)

        if user not in participants :
            errMsg = 'user is not in the group: {0}'.format(user)
            raise OrchestratorError( errMsg ) 
        
        userRefs = self.orchestrator.getUserReferences(groupId)
        calcType = self.orchestrator.calcType( groupId )
        for ref in userRefs :
            ref.callRemote("requestData", groupId, calcType).addCallback \
                (self.collateDataCallback)        
        return 

    #-------------------------------------------------
    def remote_sharePublicKeyCompleted( self, groupId, user ) :
        groupId = groupId.decode()
        self.orchestrator.unLock( groupId )


    #-------------------------------------------------
    def remote_initiatePresigning( self, user, groupId, number ) :
        groupId = groupId.decode()
                
        if self.orchestrator.isLocked(groupId) :
            errMsg = 'Group Id is locked, try again later: {0}'.format(groupId)
            raise OrchestratorError( errMsg )  

        self.orchestrator.lock( groupId ) 
        return [user, groupId, number]

    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def remote_presigning( self, user, groupId, calcType ) :
        
        groupId = groupId.decode()
        calcType = calcType.decode()
        self.orchestrator.setCalcType( groupId, calcType )
       
        log.msg("presigning: user={0}, groupId={1}, calcType={2}".format(user, groupId, calcType) )

        if not self.orchestrator.validGroup(groupId) :
            errMsg = 'Group Id is not valid: {0}'.format(groupId)
            raise OrchestratorError( errMsg )


        participants = self.orchestrator.getParticipants(groupId)

        if user not in participants :
            errMsg = 'user is not in the group: {0}'.format(user)
            raise OrchestratorError( errMsg ) 

        userRefs = self.orchestrator.getUserReferences(groupId)
        for ref in userRefs :
            ref.callRemote("requestData", groupId, calcType).addCallback \
                (self.collateDataCallback)        
        return
    
    #-------------------------------------------------
    def remote_presigningCompleted( self, groupId ) :
        groupId = groupId.decode()

        if not self.orchestrator.validGroup(groupId) :
            errMsg = 'Group Id is not valid: {0}'.format(groupId)
            raise OrchestratorError( errMsg )

        self.orchestrator.unLock( groupId )


    #-------------------------------------------------
    def remote_collateVWData( self, groupId, ordinal, data ) :
        log.msg("Collating VW Data")
        groupId = groupId.decode()
        if self.orchestrator.collateVWData( groupId, ordinal, data ) :
            collatedData = self.orchestrator.getCollatedVWData( groupId )

            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( groupId )
            for ref in userRefs :
                ref.callRemote( "sharedVWData", groupId, collatedData)
    
    
    #-------------------------------------------------        
    def remote_ephemeralKeyCompleted( self, groupId, user ) :
        groupId = groupId.decode()
        log.msg("EphemeralKey has been completed, groupId = {0}, user = {1}".format \
            (groupId, user))
        
        if self.orchestrator.allEphemeralKeysCompleted( user, groupId ) :
            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( groupId )
            for ref in userRefs :
                ref.callRemote( "completed", groupId)

    
    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def remote_sign( self, user, groupId, msg ) :
        log.msg("remote_sign")
        groupId = groupId.decode()
        msg = msg.decode()
       
        log.msg("sign: user={0}, groupId={1}, msg={2}".format(user, groupId, msg))

        if not self.orchestrator.validGroup(groupId) :
            errMsg = 'Group Id is not valid: {0}'.format(groupId)
            raise OrchestratorError( errMsg )

        if self.orchestrator.isLocked(groupId) :
            errMsg = 'Group Id is locked, try again later: {0}'.format(groupId)
            raise OrchestratorError( errMsg )
        
        self.orchestrator.lock(groupId)        

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


    #-------------------------------------------------
    # routes eval (f_x) through from the fromOridnal, to toOrdinal
    def remote_routeEvals(self, gid, user, toOrdinal, fromOrdinal, f_x) :
        groupId = gid.decode()
        log.msg("routeEvals. toOrdinal={0}, fromOrdinal={1}, f_x={2}".format(toOrdinal, fromOrdinal, f_x))

        # go from ordinal to user
        refDict = self.orchestrator.getPtpReferences( user, groupId )
        refDict[toOrdinal].callRemote("distributeEvals", \
            groupId, toOrdinal, fromOrdinal, f_x)
        return 
        
    #-------------------------------------------------
    # called when a Player has received all their Evals
    def remote_receivedAllEvals(self, gid, ordinal ) :
        log.msg("receivedAllEvals")
        groupId = gid.decode()

        # if all Players have received their Eval data then continue
        if self.orchestrator.allEvalsReceived( groupId, ordinal ) :

            collatedData = self.orchestrator.getCollatedData( groupId) 
            
            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( groupId )
            for ref in userRefs :
                ref.callRemote( "createSecret", groupId, self.orchestrator.calcType(groupId), collatedData[0], collatedData[1])\
                    .addCallbacks(self.secretVerificationCallback, self.verificationErrorCallback)


    #--------------------------------------------------
    # Callback operations available in this section
    #-------------------------------------------------- 

    def acceptInviteCallback(self, data ) :
        
        user        = data[0]
        groupID     = data[1]
        acceptance  = data[2]    

        if self.orchestrator.acceptInvite(user, groupID, acceptance) :
            self.groupIsSet(groupID)


    #-------------------------------------------------
    # collate data
    def collateDataCallback(self,  data ) :
        #JAS
        # TODO : can remove user from this as not used here!
        log.msg('collateDataCallback')
        groupId     = data[0]
        ordinal     = data[1]
        user        = data[2] 
        hiddenPoly  = data[3]
        hiddenEvals = data[4]

        # if True then ready to distribute data

        if self.orchestrator.collateData( groupId, ordinal, hiddenPoly, hiddenEvals) :

            # Call the shareEvals, pass in a different set of ordinal:refs for each
            userRefs        = self.orchestrator.getUserReferences( groupId )

            participants = self.orchestrator.getParticipants( groupId )
            
            for p, ref in zip(participants, userRefs) :
                newUserRefs = self.orchestrator.getPtpReferences( p, groupId )
                toOrdinals = []
                for key, value in newUserRefs.items() :
                    
                    toOrdinals.append(key)

                log.msg(toOrdinals)
                ref.callRemote("shareEvals", groupId, toOrdinals )
                

            log.msg('finished in collateDataCallback')

            # // following part is the orig                    
            #collatedData = self.orchestrator.getCollatedData(groupId) 
            
            # send the public data out to all group participants
            #userRefs = self.orchestrator.getUserReferences( groupId )
            #for ref in userRefs :
            #    ref.callRemote( "createSecret", groupId, self.orchestrator.calcType(groupId), collatedData[0], collatedData[1], collatedData[2])\
            #        .addCallbacks(self.secretVerificationCallback, self.verificationErrorCallback)


    #-------------------------------------------------
    # Receives verification from all group members
    def secretVerificationCallback(self, data):

        user        = data[0]
        groupId     = data[1]


        if self.orchestrator.secretVerification(user, groupId) :
            calcType = self.orchestrator.calcType(groupId)
            log.msg("secretVerification complete for: {0}".format(calcType))

            # contact all the group participants with verification success
            userRefs = self.orchestrator.getUserReferences(groupId)
            for ref in userRefs :
                ref.callRemote("groupIsVerified", groupId, calcType)


    #-------------------------------------------------
    def verificationErrorCallback(self, data):

        l = data.value.split("'")[1::2]

        user    = l[0]
        groupId = l[1]
        reason  = l[2]

        log.msg ("Verification Error: groupId = {0}, user = {1}, reason = {2}".format\
                ( groupId, user, reason ))

        # contact all the group participants to delete group
        userRefs = self.orchestrator.getUserReferences(groupId)
        for ref in userRefs :
            ref.callRemote("deleteGroup", groupId )

    #-------------------------------------------------
    # Signing callback with data: 
    def signingCallback(self,  data ) :
        log.msg("signingCallback")
        groupId     = data[0]
        ordinal     = data[1]
        sig         = data[2]
        msg         = data[3]
        
        # if True then ready to distribute data
        if self.orchestrator.signature( groupId, ordinal, sig) :
            signatureData = self.orchestrator.getSignatureData(groupId) 
            
            # send the signature data out to the signer
            userRef = self.orchestrator.getSignerReference( groupId )
            userRef.callRemote( "readyToSign", groupId, msg, signatureData) \
                .addCallbacks(self.signingCompletedCallback, self.signingErrorCallback)

    #-------------------------------------------------- 
    def signingCompletedCallback ( self, groupId ) :
        log.msg("signingCompletedCallback: groupId = {0}".format(groupId))

        self.orchestrator.unLock(groupId)

    #-------------------------------------------------- 
    def signingErrorCallback ( self, groupId ) :
        log.msg("signingErrorCallback: groupId = {0}".format(groupId)) 

#-----------------------------------------------------------------

if __name__ == '__main__':

    reactor.listenTCP(8790, pb.PBServerFactory(OrchestratorProtocol()))
    reactor.run()


