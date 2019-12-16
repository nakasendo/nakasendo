#!/bin/bash
echo "Generating proto grpc files..."
python3 -m grpc_tools.protoc -I../protos --python_out=../py/ --grpc_python_out=../py/ ts_messages.proto
python3 -m grpc_tools.protoc -I../protos --python_out=../py/ --grpc_python_out=../py/ ts_enums.proto
echo "DONE"
