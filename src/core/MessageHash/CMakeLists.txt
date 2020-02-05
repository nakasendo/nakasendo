#################################################################
#  Date             15/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
set(MessageHash_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64EncDec.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Base58EncDec.h
  ${CMAKE_CURRENT_SOURCE_DIR}/conversions.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHash.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashConfig.h
)
set(MessageHash_HPP_FILES_PRIVATE # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64EncDecImpl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashImpl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Base58EncDecImpl.h
)

set(MessageHash_HPP_FILES ${MessageHash_HPP_FILES_PUBLIC} ${MessageHash_HPP_FILES_PRIVATE})

set(MessageHash_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64EncDec.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64EncDecImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/conversions.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHash.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Base58EncDec.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Base58EncDecImpl.cpp
)

set(MessageHash_SOURCE_FILES ${MessageHash_HPP_FILES} ${MessageHash_CPP_FILES})
add_library(MessageHash ${LIBS_BUILD_TYPE} ${MessageHash_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
  target_link_libraries(MessageHash crypt32 ws2_32)
endif()
target_link_libraries(MessageHash OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(MessageHash PRIVATE EXPORT_MessageHash)
set_property(TARGET MessageHash PROPERTY FOLDER "core")

install(FILES ${MessageHash_HPP_FILES_PUBLIC} DESTINATION "include/MessageHash" COMPONENT core)
install(TARGETS MessageHash DESTINATION "lib" COMPONENT core)

###################################################################################
##                     WASM build for MessageHash library                        ##
###################################################################################

if(BUILD_WEBASSEMBLY)
set(_WASM_LIBRARY_TARGET_NAME WASMMessageHash)

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_bitcode_file(BITCODE_OUTPUT_FILE MessageHash)
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE MessageHash)

add_custom_target(${_WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${MessageHash_SOURCE_FILES})
set_property(TARGET ${_WASM_LIBRARY_TARGET_NAME} PROPERTY FOLDER "core")

add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${MessageHash_CPP_FILES}
                   -std=c++11 -Wall
                   -lssl -lcrypto -L${SDK_OPENSSL_WASM_LIBRARY_DIR}
                   -I${CMAKE_CURRENT_SOURCE_DIR} -I${SDK_OPENSSL_WASM_INCLUDE_DIR} -I${CMAKE_SOURCE_DIR}/depends/json -I${CMAKE_SOURCE_DIR}/src/core -I${SDK_GENERATED_HPP_DIR}
                   -o ${BITCODE_OUTPUT_FILE}
)
add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} POST_BUILD
                   COMMAND ${emar_EXECUTABLE}
                   ARGS
                   r ${LIBRARY_OUTPUT_FILE} ${BITCODE_OUTPUT_FILE}
)

install(FILES "${LIBRARY_OUTPUT_FILE}" DESTINATION "lib" COMPONENT WASMModules)
endif(BUILD_WEBASSEMBLY)
