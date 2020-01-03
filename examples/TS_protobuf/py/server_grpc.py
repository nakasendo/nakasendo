

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

        participants = self.orchestrator.getParticipants(gid)
        print('gid = {0}, participants = {1}'.format(gid, participants))
        for user in participants :
            params = self.orchestrator.getGroupIsSetParameters( user, gid )

            print ('sending groupIsSet message to {0}'.format(user))
            ref = self.orchestrator.getUserRef(user)
            response = ref[0].CallGroupIsSet( stub.GroupIsSetRequest \
                ( groupId=gid, ordinal=params[0], ordinalList=params[1], degree=params[2] ) )

        return


    #--------------------------------------------------
    # operations available to remote in this section
    #-------------------------------------------------- 

    # Register user with Orchestrator        
    def CallRegister(self, request, context):

        client_port = str(request.reference)
        print('client_port = {0}'.format(client_port))

        # create a gRPC channel + stub
        client_channel = grpc.insecure_channel('localhost' + ':' + str(client_port))
        client_conn = rpc.TSServiceStub( client_channel )

        self.orchestrator.register( request.user, (client_conn, client_port) )
    
        return stub.GenericReply( success = True ) 


    #-------------------------------------------------
    def CallCreateGroup(self, request, context) :
        
        print('user = {0}, m = {1}, n = {2} '.format \
            ( request.user, request.m, request.n ) )

        results = None
        try :
            results =  self.orchestrator.createGroup(request.user, request.m, request.n)
        except Exception as inst:
            #raise OrchestratorError( inst.args )
            print('ERROR in OrchestratorProtocol:CallCreateGroup')

        gid = results[0]
        inviteList = results[1]
        print('inviteList = {0}'.format(inviteList))

        for invitee in inviteList :
            call_future = invitee[0].CallInvite.future( stub.IdentityMessage( groupId=gid))
            call_future.add_done_callback( self.acceptInviteCallback )

        return stub.IdentityMessage( groupId=gid )


    #-------------------------------------------------
    # This sends a request for data to all participants of group
    def CallSharePublicKey( self, request, context ) :

        print('in CallSharePublicKey')
        gid = request.groupId
        self.orchestrator.setCalcType (gid, request.calculation )       

        if not self.orchestrator.validGroup(gid) :
            errMsg = 'Group Id is not valid: {0}'.format(gid)
            #raise OrchestratorError( errMsg )
            print('ERROR in OrchestratorProtocol:CallSharePublicKey')
            print(errMsg)

        if self.orchestrator.isLocked(gid) :
            errMsg = 'Group Id is locked, try again later: {0}'.format(gid)
            #raise OrchestratorError( errMsg )
            print('ERROR in OrchestratorProtocol:CallSharePublicKey')
            print(errMsg)
        
        self.orchestrator.lock(gid)

        participants = self.orchestrator.getParticipants(gid)

        if request.user not in participants :
            errMsg = 'user is not in the group: {0}'.format(request.user)
            #raise OrchestratorError( errMsg ) 
            print('ERROR in OrchestratorProtocol:CallSharePublicKey')
            print(errMsg)
        
        userRefs = self.orchestrator.getUserReferences(gid)
        calcType = self.orchestrator.calcType( gid )
        for ref in userRefs :
            print('calling request data...')
            call_future = ref[0].CallRequestData.future( stub.DataRequest( groupId=gid))
            call_future.add_done_callback( self.collateDataCallback )      

        
        return stub.GenericReply( success=True )

    #-------------------------------------------------
    def CallInitiatePresign(self, request, context) :
        gid = request.groupId 

        if self.orchestrator.isLocked(gid) :
            errMsg = 'Group Id is locked, try again later: {0}'.format(gid)
            #raise OrchestratorError( errMsg )  
            print('ERROR: ' + errMsg )

        self.orchestrator.lock( gid ) 
        return stub.InitPresignReqRep \
            ( user=request.user, groupId=gid, number=request.number )


    #-------------------------------------------------
    def CallPresigning (self, request, context) :
        user        = request.user 
        gid         = request.groupId 
        calcType    = request.calculation 

        self.orchestrator.setCalcType( gid, calcType )
       
        print("presigning: user={0}, groupId={1}, calcType={2}".format(user, gid, calcType) )

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
            #ref = tup[0]           
            print('calling request data...')
            call_future = ref[0].CallRequestData.future( stub.DataRequest( groupId=gid))
            call_future.add_done_callback( self.collateDataCallback )      
            
        return stub.GenericReply( success=True )      

    #-------------------------------------------------
    def CallCollateVWData(self, request, context) :
        gid = request.groupId 

        print("Collating VW Data")

        # orchestrator expects a tuple ()
        data = ( request.data.v, request.data.w )
      
        if self.orchestrator.collateVWData( gid, request.data.ordinal, data ) :
            collatedData = self.orchestrator.getCollatedVWData( gid )


            vwList = []
            for ordinal, vw in collatedData.items() :
                vwdata = stub.VWData( ordinal=ordinal, v=vw[0], w=vw[1] )
                vwList.append( vwdata )

            # send the public data out to all group participants
            userRefs = self.orchestrator.getUserReferences( gid )
            for ref in userRefs :
                
                ref[0].CallSharedVWData( stub.SharedVWDataRequest \
                    ( groupId=gid, data=vwList )   )  

        return stub.GenericReply( success=True )   

    #-------------------------------------------------
    def CallPubKeyComplete(self, request, context) :
        print('Sharing public key completed: {0}'.format(request.groupId))
        self.orchestrator.unLock( request.groupId )   

        return stub.GenericReply( success=True )      

    #-------------------------------------------------
    def CallEphemKeyComplete(self, request, context) :
        print("EphemeralKey has been completed, groupId = {0}, user = {1}".format \
            (request.groupId, request.user))

        if self.orchestrator.allEphemeralKeysCompleted( request.user, request.groupId ) :
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
    def CallSign(self, request, context) :

        gid         = request.groupId
        msg         = request.message
        user        = request.user
       
        print("sign: user={0}, groupId={1}, msg={2}".format(user, gid, msg))

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
        if user not in participants :
            errMsg = 'user is not in the group: {0}'.format(user)
            #raise OrchestratorError( errMsg ) 
            print( 'ERROR: ' + msg )
        self.orchestrator.setSigner(gid, user)

        userRefs = self.orchestrator.getUserReferences(gid)
        for ref in userRefs :
            call_future = ref[0].CallRequestSignature.future( stub.SignatureRequest \
                ( groupId=gid, message=msg ) )
            call_future.add_done_callback( self.signingCallback )      

            #ref.callRemote("requestSignatureData", groupId, msg).addCallback \
            #    (self.signingCallback)       
            #  
        return stub.GenericReply( success=True ) 


    #--------------------------------------------------
    # Callback operations available in this section
    #-------------------------------------------------- 
    
    def acceptInviteCallback (self, call_future ) :
        res = call_future.result() 
        
        if self.orchestrator.acceptInvite(res.user, res.groupId, res.acceptance) :
            self.groupIsSet(res.groupId)  
            print('acceptInviteCallback: group is set!')  


    #--------------------------------------------------                 
    def collateDataCallback (self, call_future ) :
        res = call_future.result()

        print('collateDataCallback')

        # if True then ready to distribute data
        if self.orchestrator.collateData( res.groupId, res.ordinal, res.hiddenPoly, res.hiddenEvals) :

            # Call the shareEvals, pass in a different set of ordinal:refs for each
            userRefs = self.orchestrator.getUserReferences( res.groupId )
            participants = self.orchestrator.getParticipants( res.groupId )
            
            for p, ref in zip(participants, userRefs) :

                newUserRefs = self.orchestrator.getPtpReferences( p, res.groupId )
                ptpRefs = []
                for key, value in newUserRefs.items() :
                    tmp = stub.ptpRef( ordinal=key, url=value[1] )
                    ptpRefs.append(tmp)


                ref[0].CallShareEvals( stub.ShareEvalsRequest \
                    ( groupId=res.groupId, reference=ptpRefs ) )

            

    #---------------------------------------------------
    def CallReceivedAllEvals ( self, request, context ) :

        gid     = request.groupId
        ord     = request.ordinal
        #user     = request.ordinal

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
    
            for ref in userRefs :
                print('calling createsecret...')

                call_future = ref[0].CallCreateSecret.future( stub.CreateSecretRequest( \
                    groupId=gid, calculation=self.orchestrator.calcType(gid), \
                        hiddenPolys=hpList, hiddenEvals=hidEvalList ))
                call_future.add_done_callback( self.secretVerificationCallback )      
        
        print('returning True from CallReceivedAllEvals')
        return stub.GenericReply( success=True )
    
    #--------------------------------------------------
    def secretVerificationCallback (self, call_future ) :
        res = call_future.result() 
        
        print('secretVerificationCallback: ')  

        if self.orchestrator.secretVerification(res.user, res.groupId) :
            calcType = self.orchestrator.calcType(res.groupId)
            print("secretVerification complete for: {0}".format(calcType))

            # contact all the group participants with verification success
            userRefs = self.orchestrator.getUserReferences(res.groupId)
            for ref in userRefs :
                ref[0].CallGroupIsVerified( stub.GroupIsVerifiedRequest \
                    ( groupId=res.groupId, calculation=calcType ) )

    #--------------------------------------------------                 
    def signingCallback (self, call_future ) :
        res = call_future.result()

        gid         = res.groupId
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

            call_future = userRef[0].CallReadyToSign.future( stub.ReadyToSignRequest \
                ( groupId=gid, message=msg, signatureData=sigList ) )
            call_future.add_done_callback( self.signingCompletedCallback )      


    #--------------------------------------------------                 
    def signingCompletedCallback (self, call_future ) :
        res = call_future.result()    
        self.orchestrator.unLock(res.groupId)


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
