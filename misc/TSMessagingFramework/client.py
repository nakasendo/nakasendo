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



#----------------------------------------------------------------------
#----------------------------------------------------------------------
# Client Protocol 
# handles commuications to the orchestrator server
class ClientProtocol( pb.Referenceable ):
    
    def __init__(self ) :
        self.orchestratorRef = None
        self.user = sys.argv[1]

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
        print ("error getting object: ",reason)

    #--------------------------------------------------
    def createGroup(self, n, m):
        print ('calling remote createGroup with {0}, {1}'.format(n,m))

        d = self.orchestratorRef.callRemote \
            ( "createGroup", self.user, n, m )
        d.addCallback   ( self.success_remote )
        d.addErrback    ( self.err_remote ) 

    def err_remote(self, reason):
        print ("Error from server:", reason)

    def success_remote(self, result):
        print ("Operation succeeded, groupID = ", result)


    
    #--------------------------------------------------
    # operations available to remote in this section
    #--------------------------------------------------
         
    def remote_invite(self, gid):
        sleep(randint(4,12))
        print("Received invitation for group {0}, replying with Acceptance".format(gid))
        return [ self.user, gid, True ]

    def remote_groupIsSet(self, gid, ordinal, ordinalList) :
        print("GroupIsSet:\n\tgroupId =  {0}\n\tmy ordinal = {1}\n\trest of ordinals = {2}".format \
            ( gid.decode(), ordinal, ordinalList))
        print("Create & Verify shared secret....")
        sleep(randint(4,12))
        print("...returning")

        return [self.user, gid, "some shared secret data"]

    def remote_listCoefficients(self, gid, data) :
        print("ListCoefficients:\n\tgroupId =  {0}\n\tdata = {1}".format(gid.decode(), data))
        print("Do something with the data....")
        return



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
        args = commandParts[1:]

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

