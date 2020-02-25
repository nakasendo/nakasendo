
"""The Python implementation of the GRPC Threshold Signature client."""

from __future__ import print_function
from concurrent import futures
import logging

import threading
from tkinter import *
from tkinter import simpledialog
import time
import sys
from random import randint

import grpc

import ts_messages_pb2 as stub
import ts_enums_pb2 as enums
import ts_messages_pb2_grpc as rpc

from player import Player

_ONE_DAY_IN_SECONDS = 60 * 60 * 24



#----------------------------------------------------------------------
#----------------------------------------------------------------------
# Client Protocol 
# handles communications to the orchestrator server
class ClientProtocol:

    class ServerProtocol( rpc.TSServiceServicer ) :

        def __init__( self, user, cp ) : 
            print('in ServerProtocol:__init__')
            self.user   = user 
            self.cp = cp

        #----------------------------------------------------------------------
        def CallInvite(self, request, context) :
            self.cp.myPrint('in CallInvite')
            # random sleep to mimic network communications
            time.sleep(randint(1,4))   
            self.cp.myPrint("Received invitation for group {0}, replying with Acceptance".format(request.id.groupId))
            idMsg = stub.IdentityMessage( userId=self.user, groupId=request.id.groupId )
            return stub.InviteReply( id=idMsg, acceptance=True )

        #----------------------------------------------------------------------    
        def CallGroupIsSet(self, request, context) :
            groupId = request.groupId

            idMsg = stub.IdentityMessage( userId=self.user, groupId=request.groupId )
            
            # need to find out my own player ordinal, and an ordinalLIst for the rest of the group
            ordinal = 0
            ordinalList = []
            self.cp.myPrint('in CallGroupIsSet')
            for participant in request.participants :

                if ( participant.playerId.name == self.user ) :
                    self.cp.myPrint(' this is me, setting ordinal to {0}'.format(participant.ordinal))
                    ordinal = participant.ordinal
                else :
                    ordinalList.append( ( participant.ordinal, participant.playerId.port ) )

            if ordinal == 0 :
                self.cp.myPrint(' Ordinal is zero, something has gone wrong')
                return stub.GroupIsSetReply( id=idMsg, success=False )

            else :
                self.cp.myPrint(str(ordinalList))
                if not self.cp.Player.addGroup  \
                    ( groupId, ordinal, ordinalList, request.degree ) :
                    print('Error: Group {0} already exists'.format(groupId) )
                    return stub.GroupIsSetReply( id=idMsg, success=False )
                
                return stub.GroupIsSetReply( id=idMsg, success=True )

        #----------------------------------------------------------------------
        def CallShareSecretData(self, request, context) :
            self.cp.myPrint('CallShareSecretData')

            calculation = 'PRIVATEKEYSHARE'
            calcType = request.calculation 
            if calcType == enums.LITTLEK :
                calculation = 'LITTLEK'
            elif calcType == enums.ALPHA :
                calculation = 'ALPHA'

            self.cp.myPrint('calculation type = {0}, {1}'.format(calcType, calculation))

            ret = self.cp.Player.requestData( request.id.groupId, calculation ) 
            gid  = ret[0]
            ordi = ret[1]
            hpol = ret[2]

                
            # create list of hidden evaluated polynomials
            heval = ret[3]
            hepList = []
            for key, value in heval.items() :
                hep = stub.evaluatedPoly(ordinal=key, f_x=value)
                hepList.append(hep)

            idMsg = stub.IdentityMessage( userId=self.user, groupId=gid )                       
            return stub.ShareSecretDataReply( id=idMsg, \
                ordinal=ordi, hiddenPoly=hpol, hiddenEvals=hepList )
        
        #----------------------------------------------------------------------
        def CallCollatedSecretShare(self, req, context) :

            self.cp.myPrint('CallCollatedSecretShare   ')
            
            # convert received data back into Player expected formats
            calculation = 'PRIVATEKEYSHARE'

            calcType = req.calculation 
            if calcType == enums.LITTLEK :
                calculation = 'LITTLEK'
            elif calcType == enums.ALPHA :
                calculation = 'ALPHA'


            hiddenPolys = {}
            for hp in req.hiddenPolys :
                hiddenPolys[hp.ordinal] = hp.coefficients 


            hiddenEvals = {}                
            for ev in req.hiddenEvals :
                innerEP = {}
                for e in ev.ep :
                    innerEP[e.ordinal] = e.f_x
                hiddenEvals[ev.ordinal] = innerEP

            try:
                self.cp.Player.createSecret( \
                    req.id.groupId, calculation, \
                        hiddenPolys, hiddenEvals )
            except Exception as inst:
                self.cp.myPrint('exception raised')
                self.cp.myPrint( str(inst) )
                #raise ClientError( [self.user, gid, inst.args ] )

            idMsg = stub.IdentityMessage( userId=self.user, groupId=req.id.groupId ) 
            return stub.CollatedSecretReply( id=idMsg )

        #----------------------------------------------------------------------
        def CallGroupIsVerified( self, request, context) :

            
            gid         = request.id.groupId
            calcType    = request.calculation

            self.cp.myPrint('GroupIsVerified: {0}'.format(gid))

            if calcType == enums.LITTLEK :
                self.cp.myPrint("presign initiator = {0}".format(self.cp.Player.isPresignInitiator(gid)))
                if  self.cp.Player.isPresignInitiator(gid) :
                    self.cp.myPrint("calling remote_presigning for ALPHA")
                    idMsg = stub.IdentityMessage( userId=self.user, groupId=gid )
                    response = self.cp.conn.CallPresigning( stub.PresigningRequest \
                        ( id=idMsg, calculation=enums.ALPHA) )
                
            elif calcType == enums.ALPHA : 
                res = self.cp.Player.getVWshares( gid )
                #self.cp.myPrint('res = ')
                #self.cp.myPrint( str(res ))

                vwdata = stub.VWData( ordinal=res[0], v=res[1][0], w=res[1][1])
                
                idMsg = stub.IdentityMessage( userId=self.user, groupId=gid )
                response = self.cp.conn.CallShareVW( stub.ShareVWDataMessage \
                        ( id=idMsg, data=vwdata ) )
                    
            elif calcType == enums.PRIVATEKEYSHARE :
                if self.cp.Player.isShareInitiator( gid ) :

                    response = self.cp.conn.CallPubKeyComplete( stub.IdentityMessage \
                        (groupId=gid, userId=self.user))

            return stub.GroupIsVerifiedReply( success=True)       

        #----------------------------------------------------------------------
        def CallCollatedVWShare ( self, request, context): 
            self.cp.myPrint('CallSharedVWData')
            gid = request.id.groupId

            # player needs dict (tuple)
            vwDict = {}
            for data in request.data :
                vw = (data.v, data.w)
                vwDict[data.ordinal] = vw
            self.cp.Player.setSharedVWData(gid, vwDict)
            
            response = self.cp.conn.CallEphemKeyComplete( stub.IdentityMessage \
                ( groupId=gid, userId=self.user ) )

            idMsg = stub.IdentityMessage( userId=self.user, groupId=gid )
            return stub.CollatedVWShareReply( \
                id=idMsg, ordinal=self.cp.Player.getOrdinal(gid), success=True )  

        #----------------------------------------------------------------------
        def CallCompleted ( self, request, context): 
            self.cp.myPrint('CallCompleted')

            gid=request.groupId

            if self.cp.Player.isPresignInitiator(gid) :
                
                numberPresignsLeft = self.cp.Player.numberPresignsLeftToDo( gid )
                if numberPresignsLeft > 0 :
                    self.cp.myPrint("number presign left to do = {0}".format(numberPresignsLeft))
                    self.cp.myPrint("calling remote_presigning for LITTLEK")  
                    
                    idMsg = stub.IdentityMessage( userId=self.user, groupId=gid )
                    response = self.cp.conn.CallPresigning( stub.ShareSecretRequest \
                        ( id=idMsg, calculation=enums.LITTLEK) )                 

                else: 
                    self.cp.myPrint("I was the presign initiator.  Looks like we're FINISHED!!!")
                    
                    response = self.cp.conn.CallPresignComplete( stub.IdentityMessage \
                        ( groupId=gid ) )

            return stub.GenericReply( success=True )  

        #----------------------------------------------------------------------
        def CallShareOfSignature( self, request, context ) :
            ret = self.cp.Player.requestSignatureData( request.groupId, request.message )

            idMsg = stub.IdentityMessage( userId=self.user, groupId=ret[0] )
            return stub.ShareOfSigReply \
                ( id=idMsg, ordinal=ret[1], signature=ret[2] , message=ret[3] ) 


        #----------------------------------------------------------------------
        def CallSignMessage( self, request, context ) :
            # create dictionary for Player
            sigDict = {}
            for sig in request.signatures :
                sigDict[sig.ordinal] = sig.signature

            
            sig = self.cp.Player.sign( request.id.groupId, request.message, sigDict )
            idMsg = stub.IdentityMessage( userId=self.user, groupId=request.id.groupId )

            return stub.SignMessageReply(  id=idMsg )  

        #----------------------------------------------------------------------   
        def CallDistributeEvals ( self, request, context ) :
            gid         = request.id.groupId
            toOrdinal   = request.toOrdinal
            fromOrdinal = request.fromOrdinal
            f_x         = request.f_x

            self.cp.myPrint('Received evals from Player{0}'.format(fromOrdinal))

            idMsg = stub.IdentityMessage( groupId=gid, userId=self.user  )

            if self.cp.Player.allEvalsReceived( gid, toOrdinal, fromOrdinal, f_x) :
                
                response = self.cp.conn.CallReceivedAllEvals( \
                    stub.RxAllEvalsRequest ( id=idMsg, ordinal=toOrdinal ) )

            return stub.DistributeEvalsReply(  id=idMsg, success=True )  

        #----------------------------------------------------------------------    
        def CallInitShareEvals( self, request, context  ) :
            gid = request.id.groupId
            
            self.cp.myPrint('ServerProtocol... CallInitShareEvals')

            ordinalList = self.cp.Player.getOrdinalList(gid)
            idMsg = stub.IdentityMessage( groupId=gid )

            sentToAll = True 

            for p in ordinalList :
                self.cp.myPrint('Sending evals to Player {0} and port {1}'.format(p[0], p[1]))

                # create a gRPC channel + stub
                channel = grpc.insecure_channel('localhost' + ':' + str(p[1]))
                connection = rpc.TSServiceStub( channel )
                
                ret = self.cp.Player.getEvals(gid, p[0])

                response = connection.CallDistributeEvals( stub.DistributeEvalsRequest \
                    ( id=idMsg, toOrdinal=p[0], \
                        fromOrdinal=self.cp.Player.getOrdinal(gid), f_x=ret ) )
                if not response :
                    sentToAll = False 


            return stub.InitShareEvalsReply( success=sentToAll )


    #----------------------------------------------------------------------
    # Client Protocol
    def __init__(self, user, port, myPrint ):

        self.user       = user 
        self.myPrint    = myPrint
        self.port       = port
        self.Player     = Player( myPrint )

        # create a gRPC channel + stub
        channel = grpc.insecure_channel('localhost' + ':' + str(50050))
        self.conn = rpc.TSServiceStub( channel )

        # register with server
        player = stub.Player( name=self.user, port=self.port )
        response = self.conn.CallRegister( stub.RegisterRequest( playerId=player ) )
        self.myPrint( "ClientProtocol:__init__ received: {0}".format( response.success ) )

        # create new listening thread for when new message streams come in
        threading.Thread(target=self.__serve, daemon=True).start()


    #--------------------------------------------------
    def createGroup(self, param_m, param_n):
        #log.msg('calling remote createGroup with {0}, {1}'.format(n,m))
        self.myPrint('calling remote createGroup with {0}, {1}'.format(param_m, param_n))

        response = self.conn.CallCreateGroup( \
            stub.CreateGroupRequest( userId=self.user, m=int( param_m ), n=int( param_n ) ) )

        self.myPrint('response = ')
        self.myPrint(str(response))
        return response.id.groupId

    #--------------------------------------------------
    def shareSecret( self, gid ) :
        if not self.Player.checkGroup( gid ) :
            msg = "GroupID not found: {0}".format(gid)
            #raise ClientError( msg )
            self.myPrint('ERROR: shareSecret')
            self.myPrint( msg)

        self.Player.setShareInitiator( gid )

        idMsg = stub.IdentityMessage( userId=self.user, groupId=gid )
        response = self.conn.CallShareSecret( stub.ShareSecretRequest \
            ( id=idMsg, calculation=enums.PRIVATEKEYSHARE ) )

        self.myPrint(str(response))

    #--------------------------------------------------
    def presigning( self, gid, num ) :
        if not self.Player.checkGroup( gid ) :
            msg = "GroupID not found: {0}".format(gid)
            self.myPrint('ERROR: presigning')
            self.myPrint( msg )
            return

        idMsg = stub.IdentityMessage( userId=self.user, groupId=gid )
        call_future = self.conn.CallInitiatePresign.future( stub.InitPresignRequest \
            ( id=idMsg, number=num ) )
        call_future.add_done_callback( self.initiatePresignCallback )
        
        self.myPrint( 'initiated presigning ')


    #--------------------------------------------------
    def sign( self, gid, msg ) :     
        self.myPrint("in sign, gid = {0}, msg={1}".format(gid, msg))
        
        if not self.Player.checkGroup( gid ) :
            msg = "GroupID not found: {0}".format(gid)
            self.myPrint(msg)
            #raise ClientError( msg )     

        self.Player.setSigningInitiator( gid )
        msg     = self.Player.hashMessage( msg )
        idMsg   = stub.IdentityMessage( userId=self.user, groupId=gid )
        response = self.conn.CallInitSignature( stub.InitSignatureRequest \
            ( id=idMsg, message=msg ) )

        self.myPrint( 'initiated signing ') 

    #--------------------------------------------------
    def printPublicKey( self, gid ) :
        if not self.Player.checkGroup( gid ) :
            msg = "GroupID not found: {0}".format(gid)
            self.myPrint(msg)
            #raise ClientError( msg )  

        publicKeyShare = self.Player.getPublicKeyShare( gid ) 
        self.myPrint( "Public Key for gid = {0}: {1} ".format( gid, publicKeyShare) )

    #--------------------------------------------------
    # Callback operations available in this section
    #--------------------------------------------------    
    
    def initiatePresignCallback (self, call_future ) :
        res = call_future.result() 
        self.myPrint('\nstarting presigning for LITTLEK')
        
        self.Player.setPresignInitiator(res.id.groupId, res.number)
      
        idMsg = stub.IdentityMessage( userId=self.user, groupId=res.id.groupId )      
        response = self.conn.CallPresigning( stub.PresigningRequest \
            ( id=idMsg, calculation=enums.LITTLEK) )


    #--------------------------------------------------
    # Serve
    #--------------------------------------------------
    def __serve( self ):

        print('in serve()')
        server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
        rpc.add_TSServiceServicer_to_server( self.ServerProtocol(self.user, self), server )

        portStr = '[::]:' + self.port
        print (portStr)
        server.add_insecure_port(portStr)
        server.start()
        try:
            while True:
                time.sleep(_ONE_DAY_IN_SECONDS)
        except KeyboardInterrupt:
            server.stop(0)


#----------------------------------------------------------------------
#----------------------------------------------------------------------
# Stdio Client Protocol
# This accepts input from the TKInter and routes to required command
class StdioClientProtocol(  ) :
    
    def __init__(self, user, port) :
        
        self.user = user 
    
        # Simple Tk window for the UI
        self.root = Tk()  
        frame = Frame(self.root, width=300, height=300)
        frame.pack()
        self.window = frame
        self.root.withdraw()
        self.root.deiconify()  

        self.__setup_ui()

        # this starts a client thread which keeps connection to server open
        self.client = ClientProtocol(user, port, self.printToWindow )          


        self.window.mainloop()

    # Draw the window
    def __setup_ui(self):
        self.chat_list = Text()
        self.chat_list.pack(side=TOP)
        self.lbl_username = Label(self.window, text=self.user)
        self.lbl_username.pack(side=LEFT)
        self.entry_message = Entry(self.window, bd=5, width=50)
        self.entry_message.bind('<Return>', self.lineReceived)
        self.entry_message.focus()
        self.entry_message.pack(side=BOTTOM)


    def printToWindow( self, text ) :
        self.chat_list.insert(END, text + '\n' )

    # do something with text entered in the TK textbox
    def lineReceived(self, event):

        # retrieve message from the UI
        line = self.entry_message.get() 
        self.chat_list.insert(END, '>>> ' + line + '\n')
        self.entry_message.delete(0, END) 

        # Parse the command
        commandParts = line.split()
        command = commandParts[0].lower()
        args = []

        

        # Dispatch the command to the appropriate method.  
        # Note to implement a new command add another do_* method.
        try:
            method = getattr(self, 'do_' + command)
        except AttributeError as e:
            self.chat_list.insert(END, "Error: no such command\n")  
        else:

            if command != 'groupids' and command != 'help' and command != 'quit':
                if len(commandParts) <= 1 :
                    self.chat_list.insert(END, "Error: missing group id\n")  
                    command = 'help'
                    return
                else :
                    args.append( commandParts[1] )
                    if command == 'sign' :
                        strval = ' '.join(commandParts[2:])
                        args.append(strval.strip('\''))
                    else :
                        for item in commandParts[ 2: ] :
                            args.append(item ) 
            try:
                method(*args)
            except Exception as e:
                self.chat_list.insert(END, "Error: {0} \n".format( str(e)) )  


    # List commands
    def do_help(self, command=None) :
        if command:
            doc = getattr(self, 'do_' + command).__doc__            
            print('doc = {0}'.format(doc))
            self.chat_list.insert(END, doc )  
        else:
            commands = [cmd[3:]
                    for cmd in dir(self)
                    if cmd.startswith('do_')]

            self.chat_list.insert(END, 'Valid commands: ' + " ".join(commands) + '\n' )

    # Quit cleanly
    def do_quit(self):
        self.chat_list.insert(END, "\nGoodbye ")
        self.root.destroy()


    # Ask client to send createGroup message to remote
    def do_create(self, m, n):
        groupId = self.client.createGroup( m, n )
        self.chat_list.insert(END, '>>> create ' + m + ' ' + m )
        self.chat_list.insert(END, 'groupId = ' + groupId )


    def do_share(self, gid) :        
        self.client.shareSecret( gid )
        self.chat_list.insert(END, '>>> share ' + gid )  
    
    def do_presign(self, gid, number=1) :        
        self.client.presigning( gid, int(number) )        
        self.chat_list.insert(END, '>>> presign ' + gid + ' ' + number) 

    def do_print(self, gid) :
        self.chat_list.insert(END, str(self.client.Player) ) 
        
    def do_groupids(self):
        self.chat_list.insert(END, '>>> groupids ') 
        self.chat_list.insert(END, self.client.Player.GroupIDs() ) 
    
    def do_sign( self, gid, msg ) :
        self.client.sign( gid, msg )
        self.chat_list.insert(END, '>>> sign ' + gid + ' ' + msg + '\n') 

    def do_printpubkey( self, gid ) :  
        self.client.printPublicKey( gid )
        self.chat_list.insert(END, '>>> printpubkey ') 

  

if __name__ == '__main__':
    username = sys.argv[1]
    port = sys.argv[2]

    if username == None :
        print ('Please enter username')
        exit()
    
    if port == None :
        print( 'Please enter a port')
    c = StdioClientProtocol( username, port )