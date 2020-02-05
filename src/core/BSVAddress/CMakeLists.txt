#################################################################
#  Date             17/09/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(BSVAddress_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BSVAddress.h
  ${CMAKE_CURRENT_SOURCE_DIR}/BSVAddressConfig.h
  ${CMAKE_CURRENT_SOURCE_DIR}/BSVAddressInfo.h
)
set(BSVAddress_HPP_FILES_PRIVATE # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BSVAddressImpl.h
)
set(BSVAddress_HPP_FILES ${BSVAddress_HPP_FILES_PUBLIC} ${BSVAddress_HPP_FILES_PRIVATE})

set(BSVAddress_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BSVAddress.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BSVAddressImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BSVAddressInfo.cpp
)

set(BSVAddress_SOURCE_FILES ${BSVAddress_HPP_FILES} ${BSVAddress_CPP_FILES} )
add_library(BSVAddress ${LIBS_BUILD_TYPE} ${BSVAddress_SOURCE_FILES}  )

target_link_libraries(BSVAddress MessageHash SymEncDec)
target_compile_definitions(BSVAddress PRIVATE EXPORT_BSVAddress)
set_property(TARGET BSVAddress PROPERTY FOLDER "core")



install(FILES ${BSVAddress_HPP_FILES_PUBLIC} DESTINATION "include/BSVAddress" COMPONENT core)
install(TARGETS BSVAddress DESTINATION "lib" COMPONENT core)

###################################################################################
##                     WASM build for BSVAddress library                            ##
###################################################################################

if(BUILD_WEBASSEMBLY)
set(_WASM_LIBRARY_TARGET_NAME WASMBSVAddress)

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_bitcode_file(BITCODE_OUTPUT_FILE BSVAddress)
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE BSVAddress)

add_custom_target(${_WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${BSVAddress_SOURCE_FILES})
set_property(TARGET ${_WASM_LIBRARY_TARGET_NAME} PROPERTY FOLDER "core")

add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${BSVAddress_CPP_FILES}
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
endif(BUILD_WEBASSEMBLY)
