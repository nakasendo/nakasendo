#################################################################
#  Date             15/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
set(SymEncDec_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDec.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDecConfig.h
)
set(SymEncDec__HPP_FILES_PRIVATE # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDecImpl.h
)

set(SymEncDec_HPP_FILES ${SymEncDec_HPP_FILES_PUBLIC} ${SymEncDec__HPP_FILES_PRIVATE})

set(SymEncDec_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDec.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDecImpl.cpp
)

set(SymEncDec_SOURCE_FILES ${SymEncDec_HPP_FILES} ${SymEncDec_CPP_FILES})
add_library(SymEncDec ${LIBS_BUILD_TYPE} ${SymEncDec_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
	target_link_libraries(SymEncDec crypt32 ws2_32)	
endif()
target_link_libraries(SymEncDec MessageHash OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(SymEncDec PRIVATE EXPORT_SymEncDec)
set_property(TARGET SymEncDec PROPERTY FOLDER "core")

install(FILES ${SymEncDec_HPP_FILES_PUBLIC} DESTINATION "include/SymEncDec" COMPONENT core)
install(TARGETS SymEncDec DESTINATION "lib" COMPONENT core)

###################################################################################
##                     WASM build for SymEncDec library                          ##
###################################################################################

if(BUILD_WEBASSEMBLY)
set(_WASM_LIBRARY_TARGET_NAME WASMSymEncDec)

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_bitcode_file(BITCODE_OUTPUT_FILE SymEncDec)
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE SymEncDec)

add_custom_target(${_WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${SymEncDec_SOURCE_FILES})
set_property(TARGET ${_WASM_LIBRARY_TARGET_NAME} PROPERTY FOLDER "core")

add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${SymEncDec_CPP_FILES}
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
