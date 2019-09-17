#################################################################
#  Date             17/09/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(BCHAddress_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BCHAddress.h
  ${CMAKE_CURRENT_SOURCE_DIR}/BCHAddressConfig.h
  ${CMAKE_CURRENT_SOURCE_DIR}/BCHAddressInfo.h
)
set(BCHAddress_HPP_FILES_PRIVATE # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BCHAddressImpl.h
)
set(BCHAddress_HPP_FILES ${BCHAddress_HPP_FILES_PUBLIC} ${BCHAddress_HPP_FILES_PRIVATE})

set(BCHAddress_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BCHAddress.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BCHAddressImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BCHAddressInfo.cpp
)

set(BCHAddress_SOURCE_FILES ${BCHAddress_HPP_FILES} ${BCHAddress_CPP_FILES} )
add_library(BCHAddress ${LIBS_BUILD_TYPE} ${BCHAddress_SOURCE_FILES}  )

target_link_libraries(BCHAddress MessageHash SymEncDec)
target_compile_definitions(BCHAddress PRIVATE EXPORT_BCHAddress)
set_property(TARGET BCHAddress PROPERTY FOLDER "core")



install(FILES ${BCHAddress_HPP_FILES_PUBLIC} DESTINATION "include/BCHAddress" COMPONENT core)
install(TARGETS BCHAddress DESTINATION "lib" COMPONENT core)

###################################################################################
##                     WASM build for BCHAddress library                            ##
###################################################################################

set(_WASM_LIBRARY_TARGET_NAME WASMBCHAddress)

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_bitcode_file(BITCODE_OUTPUT_FILE BCHAddress)
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE BCHAddress)

add_custom_target(${_WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${BCHAddress_SOURCE_FILES})
set_property(TARGET ${_WASM_LIBRARY_TARGET_NAME} PROPERTY FOLDER "core")

add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${BCHAddress_CPP_FILES}
                   -std=c++11 -Wall
                   -lssl -lcrypto -L${SDK_OPENSSL_WASM_LIBRARY_DIR}
                   -I${CMAKE_CURRENT_SOURCE_DIR} -I${SDK_OPENSSL_WASM_INCLUDE_DIR} -I${CMAKE_SOURCE_DIR}/depends/json  -I${CMAKE_SOURCE_DIR}/src/core -I${SDK_GENERATED_HPP_DIR}
                   -o ${BITCODE_OUTPUT_FILE}
)
add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} POST_BUILD
                   COMMAND ${emar_EXECUTABLE}
                   ARGS
                   r ${LIBRARY_OUTPUT_FILE} ${BITCODE_OUTPUT_FILE}
)

install(FILES "${LIBRARY_OUTPUT_FILE}" DESTINATION "lib" COMPONENT WASMModules)
