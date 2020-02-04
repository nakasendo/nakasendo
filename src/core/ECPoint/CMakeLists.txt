#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(ECPoint_HPP_FILES_PUBLIC # Keep alphabet order
    ${CMAKE_CURRENT_SOURCE_DIR}/ECPoint.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ECPointConfig.h
)
set(ECPoint_HPP_FILES_PRIVATE # Keep alphabet order
    ${CMAKE_CURRENT_SOURCE_DIR}/ECPointImpl.h
)
set(ECPoint_HPP_FILES ${ECPoint_HPP_FILES_PUBLIC} ${ECPoint_HPP_FILES_PRIVATE})

set(ECPoint_CPP_FILES # Keep alphabet order
    ${CMAKE_CURRENT_SOURCE_DIR}/ECPoint.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECPointImpl.cpp
)

set(ECPoint_SOURCE_FILES ${ECPoint_HPP_FILES} ${ECPoint_CPP_FILES})
add_library(ECPoint ${LIBS_BUILD_TYPE} ${ECPoint_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
    target_link_libraries(ECPoint crypt32 ws2_32)
endif()
target_link_libraries(ECPoint BigNumbers OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(ECPoint PRIVATE EXPORT_ECPoint)
set_property(TARGET ECPoint PROPERTY FOLDER "core")

install(FILES ${ECPoint_HPP_FILES_PUBLIC} DESTINATION "include/ECPoint" COMPONENT core)
install(TARGETS ECPoint DESTINATION "lib" COMPONENT core)

###################################################################################
##                     WASM build for ECPoint library                            ##
###################################################################################

if(BUILD_WEBASSEMBLY)
set(_WASM_LIBRARY_TARGET_NAME WASMECPoint)

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_bitcode_file(BITCODE_OUTPUT_FILE ECPoint)
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE ECPoint)

add_custom_target(${_WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${ECPoint_SOURCE_FILES})
set_property(TARGET ${_WASM_LIBRARY_TARGET_NAME} PROPERTY FOLDER "core")

add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${ECPoint_CPP_FILES}
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
