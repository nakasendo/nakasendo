#################################################################
#  Date             11/09/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE Polynomial)
wasm_get_js_file(JS_OUTPUT_FILE Polynomial)

#set(JS_MODULE_HPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/JsPolynomial.hpp")
set(JS_MODULE_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/JsPolynomial.cpp")
set(JS_MODULE_SOURCE_FILES ${JS_MODULE_CPP_FILES})

set(JS_MODULE_TARGET_NAME JsPolynomial)
add_custom_target(${JS_MODULE_TARGET_NAME} ALL SOURCES ${JS_MODULE_SOURCE_FILES})
set_property(TARGET ${JS_MODULE_TARGET_NAME} PROPERTY FOLDER "modules")

add_dependencies(${JS_MODULE_TARGET_NAME} WASMPolynomial)

add_custom_command(TARGET ${JS_MODULE_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${JS_MODULE_CPP_FILES}
                   -v -Oz --js-opts 1
                   -std=c++11 -Wall --bind -s WASM=0 -s EXPORT_ALL=1 -s SINGLE_FILE=1
                   -L${WASM_OUTPUT_DIR} -lWASMBigNumbers${WASM_DEBUG_POSTFIX} -lWASMPolynomial${WASM_DEBUG_POSTFIX}
                   -lssl -lcrypto -L${SDK_OPENSSL_WASM_LIBRARY_DIR}
                   -I${CMAKE_CURRENT_SOURCE_DIR} -I${SDK_OPENSSL_WASM_INCLUDE_DIR} -I${CMAKE_SOURCE_DIR}/depends/json -I${CMAKE_SOURCE_DIR}/src/core -I${SDK_GENERATED_HPP_DIR}
                   -o ${JS_OUTPUT_FILE}
)

install(FILES "${JS_OUTPUT_FILE}" DESTINATION "lib" COMPONENT JSModules)
