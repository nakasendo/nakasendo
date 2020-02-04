

from concurrent import futures
import time
import logging

import grpc

import ts_messages_pb2 as stub
import ts_messages_pb2_grpc as rpc

from orchestrator import Orchestrator

_ONE_DAY_IN_SECONDS = 60 * 60 * 24


class OrchestratorProtocol( rpc.TSServiceServicer ) :

    def __init__( self ) : 
        print('in OrchestratorProtocol:__init__')

        self.orchestrator = Orchestrator( )

    #-------------------------------------------------
    # Send group is set message to all users in the group
    def groupIsSet(self, gid):        

        userRefs = self.orchestrator.getUserReferences( gid )
        participants = self.orchestrator.getParticipants(gid)
        print('gid = {0}, participants = {1}'.format(gid, participants))
        
        t = self.orchestrator.getDegree( gid )   
        counter = 1
        
        participantList = []


        for user in participants :
                
            ref = self.orchestrator.getUserRef(user)
            print('user {0} : ref {1} '.format(user, ref) ) 
            
            player = stub.Player( name=user, port=ref[1] )
            
            aParticipant = stub.GroupIsSetRequest.Participant( ordinal=counter, playerId=player)            
            participantList.append( aParticipant )
            
            counter = counter + 1

        gReq = stub.GroupIsSetRequest( groupId=gid, degree=t, participants=participantList )

        for user in participants :

            print ('sending groupIsSet message to {0}'.format(user))
            ref = self.orchestrator.getUserRef(user)

            
            response = ref[0].CallGroupIsSet( gReq )

        return


    #--------------------------------------------------
    # operations available to remote in this section
    #-------------------------------------------------- 

    # Register user with Orchestrator        
    def CallRegister(self, request, context):

        player = request.playerId
        client_port = str(player.port)
        print('client_port = {0}'.format(client_port))

        # create a gRPC channel + stub
        client_channel = grpc.insecure_channel('localhost' + ':' + str(client_port))
        client_conn = rpc.TSServiceStub( client_channel )

        self.orchestrator.register( player.name, (client_conn, client_port) )
    
        return stub.RegisterReply( success = True ) 


    #-------------------------------------------------
    def CallCreateGroup(self, request, context) :
        
        print('user = {0}, m = {1}, n = {2} '.format \
            ( request.userId, request.m, request.n ) )

        results = None
        try :
            results =  self.orchestrator.createGroup(request.userId, request.m, request.n)
        except Exception as inst:
            #raise OrchestratorError( inst.args )
            print('ERROR in OrchestratorProtocol:CallCreateGroup')

        gid = results[0]
        inviteList = results[1]

        for invitee in inviteList :
            idMsg = stub.IdentityMessage( groupId=gid )
            call_future = invitee[0].CallInvite.future( stub.InviteRequest ( id=idMsg ) )
            call_future.add_done_callback( self.acceptInviteCallback )

        idMsg = stub.IdentityMessage( userId=request.userId, groupId=gid )
        print('returning a creategroupreply')
        return stub.CreateGroupReply( id=idMsg )


    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def CallShareSecret( self, request, context ) :

        print('in CallShareSecret')
        gid = request.id.groupId
        self.orchestrator.setCalcType (gid, request.calculation )       

        if not self.orchestrator.validGroup(gid) :
            errMsg = 'Group Id is not valid: {0}'.format(gid)
            #raise OrchestratorError( errMsg )
            print('ERROR in OrchestratorProtocol:CallShareSecret')
            print(errMsg)

        if self.orchestrator.isLocked(gid) :
            errMsg = 'Group Id is locked, try again later: {0}'.format(gid)
            #raise OrchestratorError( errMsg )
            print('ERROR in OrchestratorProtocol:CallShareSecret')
            print(errMsg)
        
        self.orchestrator.lock(gid)

        participants = self.orchestrator.getParticipants(gid)

        if request.id.userId not in participants :
            errMsg = 'user is not in the group: {0}'.format(request.id.userId)
            #raise OrchestratorError( errMsg ) 
            print('ERROR in OrchestratorProtocol:CallShareSecret')
            print(errMsg)
        
        userRefs = self.orchestrator.getUserReferences(gid)
        calcType = self.orchestrator.calcType( gid )
        idMsg = stub.IdentityMessage( userId=request.id.userId, groupId=gid )
        
        for ref in userRefs :
            print('calling request data...')

            call_future = ref[0].CallShareSecretData.future( stub.ShareSecretDataRequest( id=idMsg ))
            call_future.add_done_callback( self.collateDataCallback )      

        
        return stub.ShareSecretReply( id=idMsg, success=True )

    #-------------------------------------------------
    def CallInitiatePresign(self, request, context) :
        gid = request.id.groupId 

        if self.orchestrator.isLocked(gid) :
            errMsg = 'Group Id is locked, try again later: {0}'.format(gid)
            #raise OrchestratorError( errMsg )  
            print('ERROR: ' + errMsg )

        self.orchestrator.lock( gid ) 
        idMsg = stub.IdentityMessage( userId=request.id.userId, groupId=gid )
        return stub.InitPresignReply \
            ( id=idMsg, number=request.number )


    #-------------------------------------------------
    def CallPresigning (self, request, context) :
        user        = request.id.userId 
        gid         = request.id.groupId 
        calcType    = request.calculation 

        self.orchestrator.setCalcType( gid, calcType )
       
        print("presigning: user={0}, groupId={1}, calcType={2}".format \
            (user, gid, calcType) )

        if not self.orchestrator.validGroup(gid) :
            errMsg = 'Group Id is not valid: {0}'.format(gid)
            #raise OrchestratorError( errMsg )
            print('ERROR: ' + errMsg )


        participants = self.orchestrator.getParticipants(gid)

        if user not in participants :
            errMsg = 'user is not in the group: {0}'.format(user)
            #raise OrchestratorError( errMsg ) 
            print('ERROR: ' + errMsg )

        userRefs = self.orchestrator.getUserReferences(gid)
        for ref in userRefs : 
    
            print('calling request data...')
            idMsg = stub.IdentityMessage( userId=user, groupId=gid )            
            call_future = ref[0].CallShareSecretData.future \
                ( stub.ShareSecretDataRequest( id=idMsg ))
            call_future.add_done_callback( self.collateDataCallback )      
            
        return stub.PresigningReply( success=True )      

    #-------------------------------------------------
    def CallShareVW(self, request, context) :
        gid     = request.id.groupId 
        user    = request.id.userId 

        print("Collating VW Data")

        # orchestrator expects a tuple ()
        data = ( request.data.v, request.data.w )
        idMsg = stub.IdentityMessage( userId=user, groupId=gid )       

        if self.orchestrator.collateVWData( gid, request.data.ordinal, data ) :
            collatedData = self.orchestrator.getCollatedVWData( gid )


            vwList = []
            for ordinal, vw in collatedData.items() :
                vwdata = stub.VWData( ordinal=ordinal, v=vw[0], w=vw[1] )
                vwList.append( vwdata )

            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( gid )
                       
            for ref in userRefs :
                
                ref[0].CallCollatedVWShare( stub.CollatedVWShareRequest \
                    ( id=idMsg, data=vwList )   )  

        return stub.ShareVWDataMessageReply( id=idMsg, success=True )   

    #-------------------------------------------------
    def CallPubKeyComplete(self, request, context) :
        print('Sharing public key completed: {0}'.format(request.groupId))
        self.orchestrator.unLock( request.groupId )   

        return stub.GenericReply( success=True )      

    #-------------------------------------------------
    def CallEphemKeyComplete(self, request, context) :
        print("EphemeralKey has been completed, groupId = {0}, user = {1}".format \
            (request.groupId, request.userId))

        if self.orchestrator.allEphemeralKeysCompleted( request.userId, request.groupId ) :
            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( request.groupId )
            for ref in userRefs :
                ref[0].CallCompleted( stub.IdentityMessage \
                    ( groupId=request.groupId ) )

        return stub.GenericReply( success=True )  

    #-------------------------------------------------
    def CallPresignComplete(self, request, context) :        
        groupId = request.groupId

        if not self.orchestrator.validGroup(groupId) :
            errMsg = 'Group Id is not valid: {0}'.format(groupId)
            #raise OrchestratorError( errMsg )
            print('ERROR: CallPresignComplete ' + errMsg )

        self.orchestrator.unLock( groupId )

        return stub.GenericReply( success=True ) 


    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def CallInitSignature(self, request, context) :

        gid         = request.id.groupId
        msg         = request.message
        userId      = request.id.userId
       
        print("sign: user={0}, groupId={1}, msg={2}".format(userId, gid, msg))

        if not self.orchestrator.validGroup(gid) :
            errMsg = 'Group Id is not valid: {0}'.format(gid)
            #raise OrchestratorError( errMsg )
            print('ERROR: ' + msg)

        if self.orchestrator.isLocked(gid) :
            errMsg = 'Group Id is locked, try again later: {0}'.format(gid)
            #raise OrchestratorError( errMsg )
            print('ERROR: ' + msg)
        
        self.orchestrator.lock(gid)        

        participants = self.orchestrator.getParticipants(gid)

        # check the user is in the group, then set the signer
        if userId not in participants :
            errMsg = 'user is not in the group: {0}'.format(useId)
            #raise OrchestratorError( errMsg ) 
            print( 'ERROR: ' + msg )
        self.orchestrator.setSigner(gid, userId)

        userRefs = self.orchestrator.getUserReferences(gid)
        for ref in userRefs :
            call_future = ref[0].CallShareOfSignature.future( stub.ShareOfSigRequest \
                ( groupId=gid, message=msg ) )
           
            call_future.add_done_callback( self.signingCallback )      

            #ref.callRemote("requestSignatureData", groupId, msg).addCallback \
            #    (self.signingCallback)       
            #  
        return stub.InitSignatureReply( success=True ) 


    #--------------------------------------------------
    # Callback operations available in this section
    #-------------------------------------------------- 
    
    def acceptInviteCallback (self, call_future ) :
        
        res = call_future.result() 
        if self.orchestrator.acceptInvite(res.id.userId, res.id.groupId, res.acceptance) :
            self.groupIsSet(res.id.groupId)  
            print('acceptInviteCallback: group is set!')  


    #--------------------------------------------------                 
    def collateDataCallback (self, call_future ) :
        res = call_future.result()
        
        print('collateDataCallback')

        # if True then ready to distribute data
        if self.orchestrator.collateData( res.id.groupId, res.ordinal, res.hiddenPoly, res.hiddenEvals) :

            # Call the shareEvals, pass in a different set of ordinal:refs for each
            userRefs = self.orchestrator.getUserReferences( res.id.groupId )
            participants = self.orchestrator.getParticipants( res.id.groupId )

            idMsg = stub.IdentityMessage( groupId=res.id.groupId )
            
            for ref in userRefs :
                ref[0].CallInitShareEvals( stub.InitShareEvalsRequest ( id=idMsg ) )
            

    #---------------------------------------------------
    def CallReceivedAllEvals ( self, request, context ) :

        gid     = request.id.groupId
        ord     = request.ordinal

        print('receivedAllEvals')

        # if all Players have received their Eval data then continue
        if self.orchestrator.allEvalsReceived( gid, ord ) :


            collatedData = self.orchestrator.getCollatedData( gid) 
            
            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( gid )

            # get hiddenPoly into suitable format
            hpList  = []
            for ordinalB, coeffs in collatedData[0].items() :
                hp = stub.hiddenPolynomial(ordinal=ordinalB,coefficients=coeffs)
                hpList.append(hp)


            # get hidden evals into a suitable format
            hidEvalList = []
            for ordinalC, evals in collatedData[1].items() :
                l_of_hep            = stub.listOfPolynomials()
                l_of_hep.ordinal    = ordinalC

                for e in evals :
                    l_of_hep.ep.add(ordinal=e.ordinal, f_x=e.f_x)

                hidEvalList.append(l_of_hep)
    
            idMsg = stub.IdentityMessage( groupId=gid )
            for ref in userRefs :
                print('calling createsecret...')

                call_future = ref[0].CallCollatedSecretShare.future( stub.CollatedSecretRequest( \
                    id=idMsg, calculation=self.orchestrator.calcType(gid), \
                        hiddenPolys=hpList, hiddenEvals=hidEvalList ))
                call_future.add_done_callback( self.secretVerificationCallback )      
        
        print('returning True from CallReceivedAllEvals')
        return stub.RxAllEvalsReply( success=True )
    
    #--------------------------------------------------
    def secretVerificationCallback (self, call_future ) :
        res     = call_future.result() 
        gid     = res.id.groupId
        user    = res.id.userId
        
        print('secretVerificationCallback: ')  

        idMsg = stub.IdentityMessage( userId=user, groupId=gid )
        if self.orchestrator.secretVerification(user, gid) :
            calcType = self.orchestrator.calcType(gid)
            print("secretVerification complete for: {0}".format(calcType))

            # contact all the group participants with verification success
            userRefs = self.orchestrator.getUserReferences(gid)
            for ref in userRefs :
                ref[0].CallGroupIsVerified( stub.GroupIsVerifiedRequest \
                    ( id=idMsg, calculation=calcType ) )

    #--------------------------------------------------                 
    def signingCallback (self, call_future ) :
        res = call_future.result()

        gid         = res.id.groupId
        ordinal     = res.ordinal
        sig         = res.signature
        msg         = res.message
        
        # if True then ready to distribute data
        if self.orchestrator.signature( gid, ordinal, sig) :
            signatureDict = self.orchestrator.getSignatureData(gid) 

            sigList = []
            for ord, sig in signatureDict.items() :
                sigData = stub.SigData( ordinal=ord, signature=sig )
                sigList.append(sigData)
            
            # send the signature data out to the signer
            userRef = self.orchestrator.getSignerReference( gid )
            idMsg = stub.IdentityMessage( userId=res.id.userId, groupId=gid )

            call_future = userRef[0].CallSignMessage.future( stub.SignDataMessage \
                ( id=idMsg, message=msg, signatures=sigList ) )
            call_future.add_done_callback( self.signingCompletedCallback )    


    #--------------------------------------------------                 
    def signingCompletedCallback (self, call_future ) :
        res = call_future.result()    
        print(res)
        self.orchestrator.unLock(res.id.groupId)


#--------------------------------------------------
# Serve
#--------------------------------------------------
def serve():

    print('in serve()')
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    rpc.add_TSServiceServicer_to_server( OrchestratorProtocol(), server )

    server.add_insecure_port('[::]:50050')
    server.start()
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        server.stop(0)


#--------------------------------------------------
# Main
#--------------------------------------------------
if __name__ == '__main__':
    logging.basicConfig()
    serve()
