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

set(ThresholdSignature_PROTOFILE "${CMAKE_CURRENT_SOURCE_DIR}/../protos/ts_messages.proto" "${CMAKE_CURRENT_SOURCE_DIR}/../protos/ts_enums.proto")
protobuf_generate_cpp(ThresholdSignature_SRCS ThresholdSignature_HDRS ${ThresholdSignature_PROTOFILE})
protobuf_generate_python(PY_ThresholdSignature_SRCS ${ThresholdSignature_PROTOFILE})

set(TSCore_HPP_FILES
    ${ThresholdSignature_HDRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/exportTSCore.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupMetaData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/orchestrator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/player.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TSMessageFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TSProtoBufHelper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TSState.h
)

set(TSCore_CPP_FILES
    ${ThresholdSignature_SRCS}
    ${PY_ThresholdSignature_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupMetaData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/orchestrator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TSMessageFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TSProtoBufHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TSState.cpp
)

set(TSCore_SOURCE_FILES ${TSCore_HPP_FILES} ${TSCore_CPP_FILES} ${ThresholdSignature_HDRS} ${ThresholdSignature_SRCS})
add_library(TSCore ${LIBS_BUILD_TYPE} ${TSCore_SOURCE_FILES})

target_link_libraries(TSCore BigNumbers MessageHash ECPoint Polynomial OpenSSL::Crypto OpenSSL::SSL protobuf::libprotobuf ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(TSCore PUBLIC ${protobuf_generate_PROTOC_OUT_DIR})
target_compile_definitions(TSCore PRIVATE EXPORT_TSCore)
set_property(TARGET TSCore PROPERTY FOLDER "core")

install(FILES ${TSCore_HPP_FILES} DESTINATION "include/TSCore" COMPONENT core)
install(TARGETS TSCore DESTINATION "lib" COMPONENT core)

add_custom_command(
    TARGET TSCore POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PY_ThresholdSignature_SRCS} ${CMAKE_SOURCE_DIR}/src/applications/TS_protobuf/py
)

