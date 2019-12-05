#!/bin/bash
echo "Generating proto grpc files..."
python3 -m grpc_tools.protoc -I../protos --python_out=. --grpc_python_out=. ../protos/ts_messages.proto
echo "DONE"
