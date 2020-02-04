ps -ef | grep client_grpc.py | grep -v grep | awk '{print $2}' | xargs kill
