
"""The Python implementation of the GRPC Threshold Signature client."""

from __future__ import print_function
import logging

from time import sleep

import grpc

import ts_messages_pb2 as stub
import ts_messages_pb2_grpc as rpc


def run():

    with grpc.insecure_channel('localhost:50051') as channel :

        self.conn = rpc..TSServiceStub( channel )

        response = self.conn.CallRegister( stub.RegisterRequest \
                ( user = 'josie' ) )
        print("Greeter client received: {0}".format( response.success ) )

        sleep(5)
        for msg in self.conn.ChatStream(chat.Empty() ):
                print ('in msg loop ')
  


if __name__ == '__main__' :

    logging.basicConfig()
    run()
