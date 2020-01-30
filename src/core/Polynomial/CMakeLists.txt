#################################################################
#  Date             29/07/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(Polynomial_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/LGInterpolator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Polynomial.h
  ${CMAKE_CURRENT_SOURCE_DIR}/PolynomialConfig.h
)
set(Polynomial_HPP_FILES ${Polynomial_HPP_FILES_PUBLIC})

set(Polynomial_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/LGInterpolator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Polynomial.cpp
)

set(Polynomial_SOURCE_FILES ${Polynomial_HPP_FILES} ${Polynomial_CPP_FILES})
add_library(Polynomial ${LIBS_BUILD_TYPE} ${Polynomial_SOURCE_FILES})

target_link_libraries(Polynomial BigNumbers ECPoint)
target_compile_definitions(Polynomial PRIVATE EXPORT_Polynomial)
set_property(TARGET Polynomial PROPERTY FOLDER "core")


install(FILES ${Polynomial_HPP_FILES_PUBLIC} DESTINATION "include/Polynomial" COMPONENT core)
install(TARGETS Polynomial DESTINATION "lib" COMPONENT core)

###################################################################################
##                     WASM build for Polynomial library     v                   ##
###################################################################################

if(BUILD_WEBASSEMBLY)
set(_WASM_LIBRARY_TARGET_NAME WASMPolynomial)

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_bitcode_file(BITCODE_OUTPUT_FILE Polynomial)
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE Polynomial)

add_custom_target(${_WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${Polynomial_SOURCE_FILES})
set_property(TARGET ${_WASM_LIBRARY_TARGET_NAME} PROPERTY FOLDER "core")

add_custom_command(TARGET ${_WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${Polynomial_CPP_FILES}
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
