

from concurrent import futures
import time
import logging

import grpc

import ts_messages_pb2
import ts_messages_pb2_grpc

_ONE_DAY_IN_SECONDS = 60 * 60 * 24


class OrchestratorProtocol( ts_messages_pb2_grpc.TSServiceServicer ) :

    def __init__( self ) : 
        print('in OrchestratorProtocol:__init__')

        clients []

    def CallRegister(self, request, context):
        print( 'registering {0}'.format(request.user) )
        clients.append request.user
        # call into orchestrator to resister with username and reference
        # what will the reference be ?

        print('context - peer: {0}'.format(context.peer()))
        return ts_messages_pb2.RegisterReply( success = True ) 

    def CallCreateGroup(self, request, context) :
        
        print('user = {0}, m = {1}, n = {2} '.format \
            ( request.user, request.m, request.n )
        #try :
        #    results =  self.orchestrator.createGroup(proposer, m, n)
        #except Exception as inst:
        #    raise OrchestratorError( inst.args )

        #groupId = results[0]
        #inviteList = results[1]

        #for invitee in inviteList :
        #    invitee.callRemote("invite", groupId).addCallback \
        #            (self.acceptInviteCallback)
        print('users: {0}'.format(self.clients) )
        
        #return groupId 
        return '123-abc'


def serve():

    print('in serve()')
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    ts_messages_pb2_grpc.add_TSServiceServicer_to_server( OrchestratorProtocol(), server )

    server.add_insecure_port('[::]:50051')
    server.start()
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        server.stop(0)


if __name__ == '__main__':
    logging.basicConfig()
    serve()
