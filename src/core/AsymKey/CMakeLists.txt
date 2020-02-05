#################################################################
#  Date             07/05/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(AsymKey_HPP_FILES_PUBLIC # Keep alphabet order
  "${CMAKE_CURRENT_SOURCE_DIR}/AsymKey.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/AsymKeyConfig.h"
)
set(AsymKey_HPP_FILES_PRIVATE # Keep alphabet order
  "${CMAKE_CURRENT_SOURCE_DIR}/AsymKeyImpl.h"
)
set(AsymKey_HPP_FILES ${AsymKey_HPP_FILES_PUBLIC} ${AsymKey_HPP_FILES_PRIVATE})

set(AsymKey_CPP_FILES # Keep alphabet order
  "${CMAKE_CURRENT_SOURCE_DIR}/AsymKey.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/AsymKeyImpl.cpp"
)

set(AsymKey_SOURCE_FILES ${AsymKey_HPP_FILES} ${AsymKey_CPP_FILES})
add_library(AsymKey ${LIBS_BUILD_TYPE} ${AsymKey_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
  target_link_libraries(AsymKey crypt32 ws2_32)
endif()
target_link_libraries(AsymKey OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} BigNumbers MessageHash Polynomial SecretSplit)
target_compile_definitions(AsymKey PRIVATE EXPORT_AsymKey)
set_property(TARGET AsymKey PROPERTY FOLDER "core")


install(FILES ${AsymKey_HPP_FILES_PUBLIC} DESTINATION "include/AsymKey" COMPONENT core)
install(TARGETS AsymKey DESTINATION "lib" COMPONENT core)

###################################################################################
##                     WASM build for AsymKey library                            ##
###################################################################################

if(BUILD_WEBASSEMBLY)
set(_WASM_LIBRARY_TARGET_NAME WASMAsymKey)

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_bitcode_file(BITCODE_OUTPUT_FILE AsymKey)
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE AsymKey)

add_custom_target(${_WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${AsymKey_SOURCE_FILES})
set_property(TARGET ${_WASM_LIBRARY_TARGET_NAME} PROPERTY FOLDER "core")

add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${AsymKey_CPP_FILES}
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
