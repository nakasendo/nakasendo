# To run:
# Start orchestrator.py first (i.e. the server)
# Then start one or more clients:
#   python3 client.py <username>
#
# See also startclient.sh for starting multiple clients
#   and stopcleints.sh for stopping multiple clients
#

import sys
from random import randint
from time import sleep
from twisted.spread import pb
from twisted.internet import stdio, reactor, protocol
from twisted.protocols import basic
from twisted.web import client
from twisted.python import log


from Player import Player

# Error class 
class ClientError(pb.Error):
    """This is an Expected Exception. Something bad happened."""
    pass

#----------------------------------------------------------------------
#----------------------------------------------------------------------
# Client Protocol 
# handles communications to the orchestrator server
class ClientProtocol( pb.Referenceable ):
    
    def __init__(self ) :
        log.startLogging(sys.stdout)
        self.orchestratorRef = None
        self.user = sys.argv[1]
        self.Player = Player( )

    #--------------------------------------------------
    def connect(self):
        factory = pb.PBClientFactory()
        reactor.connectTCP("localhost", 8790, factory)
        def1 = factory.getRootObject()
        def1.addCallbacks(self.got_obj1, self.err_obj1)

    def got_obj1(self, obj1):
        self.orchestratorRef = obj1
        self.orchestratorRef.callRemote("register", self.user, self)
        
    def err_obj1(self, reason):
        log.msg("error getting object: {0}".format(reason))

    #--------------------------------------------------
    def createGroup(self, n, m):
        log.msg('calling remote createGroup with {0}, {1}'.format(n,m))

        d = self.orchestratorRef.callRemote \
            ( "createGroup", self.user, n, m )
        d.addCallback   ( self.success_remote )
        d.addErrback    ( self.err_remote ) 
    
    #--------------------------------------------------
    def sharePublicKey( self, gid ) :
        if not self.Player.checkGroup( gid ) :
            msg = "GroupID not found: {0}".format(gid)
            log.msg(msg)
            raise ClientError( msg )

        self.Player.setShareInitiator( gid )

        d = self.orchestratorRef.callRemote \
            ( "sharePublicKey", self.user, gid.encode(), b'PRIVATEKEYSHARE' )
    
    #--------------------------------------------------
    def presigning( self, gid, number ) :
        if not self.Player.checkGroup( gid ) :
            msg = "GroupID not found: {0}".format(gid)
            log.msg(msg)
            raise ClientError( msg )

        
        ret = self.orchestratorRef.callRemote \
            ( "initiatePresigning", self.user, gid.encode(), number ).addCallbacks\
                (self.initiateSuccessCallback, self.initiateErrorCallback)


    #--------------------------------------------------
    def sign ( self, gid, msg ) :
        log.msg("in sign, gid = {0}, msg={1}".format(gid, msg))
        
        if not self.Player.checkGroup( gid ) :
            msg = "GroupID not found: {0}".format(gid)
            log.msg(msg)
            raise ClientError( msg )     

        self.Player.setSigningInitiator( gid )
        msg = self.Player.hashMessage( msg )
        d = self.orchestratorRef.callRemote \
            ( "sign", self.user, gid.encode(), msg.encode() )  

    #--------------------------------------------------
    def printPublicKey( self, gid ) :
        if not self.Player.checkGroup( gid ) :
            msg = "GroupID not found: {0}".format(gid)
            log.msg(msg)
            raise ClientError( msg )  

        publicKeyShare = self.Player.getPublicKeyShare( gid ) 
        log.msg( "Public Key for gid = {0}: {1} ".format( gid, publicKeyShare) )

    def err_remote(self, reason):
        log.msg ("Error from server:", reason)

    def success_remote(self, result):
        log.msg("Operation succeeded, groupID = {}".format(result))

    
    #--------------------------------------------------
    # operations available to remote in this section
    #--------------------------------------------------
         
    def remote_invite(self, gid):
        # random sleep to mimic network communications
        sleep(randint(4,12))
        log.msg("Received invitation for group {0}, replying with Acceptance".format(gid))
        return [ self.user, gid, True ]

    def remote_groupIsSet(self, gid, ordinal, ordinalList, degree) :
        gid = gid.decode()
        
        if not self.Player.addGroup ( gid, ordinal, ordinalList, degree ) :
            raise  ClientError( "Group already exists")
        
        return

    def remote_requestData(self, gid, calcType) :
        return self.Player.requestData( gid, calcType ) 


    def remote_createSecret(self, gid, calcType, evals,  hiddenPolys, hiddenEvals) :
        try:
            self.Player.createSecret( gid, calcType, evals,  hiddenPolys, hiddenEvals )
        except Exception as inst:
            raise ClientError( [self.user, gid, inst.args ] )
        return [self.user, gid]

    def remote_groupIsVerified( self, gid, calcType ) :
        log.msg("groupisVerified: gid = {0}, calcType = {1}".format(gid, calcType ))
        #log.msg("groupisVerified: gid = {0}, calcType = {1}".format(gid, calcType ))
        
        
        if calcType == 'LITTLEK' :
            log.msg("presign initiator = {0}".format(self.Player.isPresignInitiator(gid)))
            if  self.Player.isPresignInitiator(gid) :
                log.msg("calling remote_presigning for ALPHA")
                d  = self.orchestratorRef.callRemote \
                    ( "presigning", self.user, gid.encode(), b'ALPHA' ) 
            
        elif calcType == 'ALPHA' : 
            res = self.Player.getVWshares( gid )
            d = self.orchestratorRef.callRemote \
                ( "collateVWData", gid.encode(), res[0], res[1])
                
        elif calcType == 'PRIVATEKEYSHARE' :
            if self.Player.isShareInitiator( gid ) :
                d = self.orchestratorRef.callRemote \
                    ( "sharePublicKeyCompleted", gid.encode(), self.user )


    def remote_sharedVWData( self, gid, data ) :
        self.Player.setSharedVWData(gid, data)
        d = self.orchestratorRef.callRemote \
                ( "ephemeralKeyCompleted", gid.encode(), self.user )        


    def remote_completed( self, gid ) :
        if self.Player.isPresignInitiator(gid) :
            
            numberPresignsLeft = self.Player.numberPresignsLeftToDo( gid )
            if numberPresignsLeft > 0 :
                log.msg("number presign left to do = {0}".format(numberPresignsLeft))
                log.msg("calling remote_presigning for LITTLEK")
                d = self.orchestratorRef.callRemote \
                    ( "presigning", self.user, gid.encode(), b'LITTLEK' )  

            else: 
                log.msg("I was the presign initiator.  Looks like we're FINISHED!!!")
                d = self.orchestratorRef.callRemote \
                    ( "presigningCompleted", gid.encode() )


    def remote_requestSignatureData( self, gid, message ) :
        return self.Player.requestSignatureData( gid, message )


    def remote_readyToSign( self, gid, message, signatureData ) :
        self.Player.sign( gid, message, signatureData )
        return gid

    def remote_deleteGroup( self, gid ) :        
        self.Player.deleteGroup( gid )

    #--------------------------------------------------
    # Callback operations available in this section
    #-------------------------------------------------- 
    def initiateSuccessCallback( self, data ) :
        
        user        = data[0]
        groupId     = data[1]
        number      = data[2]

        log.msg("starting presigning ")  

        self.Player.setPresignInitiator(groupId, number)

        d = self.orchestratorRef.callRemote \
            ( "presigning", self.user, groupId.encode(), b'LITTLEK' )        
    
    def initiateErrorCallback( self, reason ) :
        log.msg ("Error:  from server: {0}".format( reason ))        

#----------------------------------------------------------------------
#----------------------------------------------------------------------
# Stdio Client Protocol
# This accepts input from the commandline and routes to required command
class StdioClientProtocol(basic.LineReceiver):
    delimiter = b'\n'   # unix terminal style newlines. 
                        # may need to change this for windows

    def __init__(self) :
        client = None


    def connectionMade(self):
        # connect client protocol - this is the pb stuff (for remote calls)
        self.client = ClientProtocol()
        self.client.connect()

        message = "Welcome Player {0}. Type 'help' for help.".format(self.client.user)
        self.sendLine(message.encode())
        self.sendLine(b'>>>')

    
    # Process Input from the console
    def lineReceived(self, line):
        # Ignore blank lines
        if not line: return
        line = line.decode("ascii")


        # Parse the command
        commandParts = line.split()
        command = commandParts[0].lower()
        args = []
        if command != 'groupids' and command != 'help' and command != 'quit':
            if len(commandParts) <= 1 :
                self.sendLine(b'Error: missing group id') 
                command = 'help'
            else :
                args.append( commandParts[1] )
                if command == 'sign' :
                    strval = ' '.join(commandParts[2:])
                    args.append( strval )
                else :
                    for item in commandParts[ 2: ] :
                        args.append(item ) 

        # Dispatch the command to the appropriate method.  
        # Note to implement a new command add another do_* method.
        try:
            method = getattr(self, 'do_' + command)
        except AttributeError as e:
            self.sendLine(b'Error: no such command.')
        else:
            try:
                method(*args)
            except Exception as e:
                self.sendLine(b'Error: ' + str(e).encode("ascii"))
                self.sendLine(b'>>>')

    
    # List commands
    def do_help(self, command=None):
        if command:
            doc = getattr(self, 'do_' + command).__doc__
            self.sendLine(doc.encode("ascii"))
        else:
            commands = [cmd[3:].encode("ascii")
                        for cmd in dir(self)
                        if cmd.startswith('do_')]
            self.sendLine(b"Valid commands: " + b" ".join(commands))
            self.sendLine(b'>>>')

    # Quit cleanly
    def do_quit(self):
        self.sendLine(b'Goodbye.')
        self.transport.loseConnection()

    # Ask client to send createGroup message to remote
    def do_create(self, n, m):
        self.client.createGroup( n, m )
        self.sendLine(b'>>>')

    def do_share(self, gid) :        
        self.client.sharePublicKey( gid )
        self.sendLine(b'>>>')
    
    def do_presign(self, gid, number=1) :        
        self.client.presigning( gid, int(number) )
        self.sendLine(b'>>>')

    def do_print(self, gid) :
        self.sendLine(str(self.client.Player).encode())
        
    def do_groupids(self):
        self.sendLine(self.client.Player.GroupIDs().encode())
    
    def do_sign( self, gid, msg ) :
        self.client.sign( gid, msg )

    def do_printpubkey( self, gid ) :
        self.client.printPublicKey( gid )

    def __checkSuccess(self, pageData):
        msg = "Success: got {} bytes.".format(len(pageData))
        self.sendLine(msg.encode("ascii"))

    def __checkFailure(self, failure):
        msg = "Failure: " + failure.getErrorMessage()
        self.sendLine(msg.encode("ascii"))
        
    

    def connectionLost(self, reason):
        reactor.stop()




#-----------------------------------------------------------------

if __name__ == "__main__":
    stdio.StandardIO(StdioClientProtocol())
    reactor.run()

