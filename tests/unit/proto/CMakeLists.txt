#################################################################
#  Date             12/02/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

if(NOT Protobuf_FOUND)
  message(STATUS "SDK WARNS : Protobuf was not found on your system, unit tests for protobuf will not be built")
  return()
endif()

set(TestMessageDefinition_PROTOFILE "${CMAKE_CURRENT_SOURCE_DIR}/TestMessageDefinition.proto")
protobuf_generate_cpp(TestMessageDefinition_SRCS TestMessageDefinition_HDRS ${TestMessageDefinition_PROTOFILE})
add_executable(test_Protobuf test_Protobuf.cpp ${TestMessageDefinition_SRCS} ${TestMessageDefinition_HDRS})
target_include_directories(test_Protobuf PUBLIC ${protobuf_generate_PROTOC_OUT_DIR})
target_link_libraries(test_Protobuf protobuf::libprotobuf)
set_target_properties(test_Protobuf PROPERTIES  FOLDER "tests/unit" DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
sdk_add_unit_test(test_Protobuf)
