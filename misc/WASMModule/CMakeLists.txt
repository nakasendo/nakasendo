#################################################################
#  Date             03/09/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################


## To build wasm modules, it is required to install emscripten and build openssl to wasm
##   - To setup emscripten, follow the documentation in https://emscripten.org/docs/getting_started/downloads.html
##     After completing installation, add path to emcc to the PATH environment variable
##   - To install openssl build in wasm, download from https://bitbucket.org/nch-atlassian/sdklibraries/downloads/openssl-1.1.1b_wasm.tar.gz
##     Extract it somewhere, and set the environment variable OPENSSL_WASM_ROOT_DIR to this directory
##
## Example of linking with WASMBigNumbers
##  emcc BigNumbersClient.cpp -lWASMBigNumbers -L /path/to/INSTALLATION/lib -I /path/to/INSTALLATION/include -std=c++11 -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s WASM=1 -o index.html
##



##############################################################################################################################
##############################################################################################################################

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(MODULE_BASE_NAME CoreModule) ## Bignumber, MessageHash ...

wasm_get_debug_postfix(WASM_DEBUG_POSTFIX)
wasm_get_output_dir(WASM_OUTPUT_DIR)
wasm_get_bitcode_file(BITCODE_OUTPUT_FILE ${MODULE_BASE_NAME})
wasm_get_emslib_file(LIBRARY_OUTPUT_FILE ${MODULE_BASE_NAME})
wasm_get_js_file(JS_OUTPUT_FILE ${MODULE_BASE_NAME})

message("-----------------------------  WASM_DEBUG_POSTFIX[${WASM_DEBUG_POSTFIX}]")
message("-----------------------------  WASM_OUTPUT_DIR[${WASM_OUTPUT_DIR}]")
message("-----------------------------  BITCODE_OUTPUT_FILE[${BITCODE_OUTPUT_FILE}]")
message("-----------------------------  LIBRARY_OUTPUT_FILE[${LIBRARY_OUTPUT_FILE}]")
message("-----------------------------  JS_OUTPUT_FILE[${JS_OUTPUT_FILE}]")
message("-----------------------------  emcc_EXECUTABLE[${emcc_EXECUTABLE}]")
message("-----------------------------  emar_EXECUTABLE[${emar_EXECUTABLE}]")

##################### BUILD MODULE LIBRARY
set(CORE_MODULE_HPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CoreModuleFile1.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/CoreModuleFile2.hpp")
set(CORE_MODULE_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/CoreModuleFile1.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/CoreModuleFile2.cpp")
set(CORE_MODULE_SOURCE_FILES ${CORE_MODULE_HPP_FILES} ${CORE_MODULE_CPP_FILES})

set(WASM_LIBRARY_TARGET_NAME WASM${MODULE_BASE_NAME})
add_custom_target(${WASM_LIBRARY_TARGET_NAME} ALL SOURCES ${CORE_MODULE_SOURCE_FILES})

add_custom_command(TARGET ${WASM_LIBRARY_TARGET_NAME} PRE_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${CORE_MODULE_CPP_FILES}
                   -std=c++11 -Wall
                   -lssl -lcrypto -L${SDK_OPENSSL_WASM_LIBRARY_DIR}
                   -I${CMAKE_CURRENT_SOURCE_DIR} -I${SDK_OPENSSL_WASM_INCLUDE_DIR} -I${CMAKE_SOURCE_DIR}/depends/json -I${CMAKE_SOURCE_DIR}/src/core -I${SDK_GENERATED_HPP_DIR}
                   -o ${BITCODE_OUTPUT_FILE}
)
add_custom_command(TARGET ${WASM_LIBRARY_TARGET_NAME} POST_BUILD
                   COMMAND ${emar_EXECUTABLE}
                   ARGS
                   r ${LIBRARY_OUTPUT_FILE} ${BITCODE_OUTPUT_FILE}
)

##################### BUILD MODULE JAVASCRIPT
set(JS_MODULE_HPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/JsModule.hpp")
set(JS_MODULE_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/JsModule.cpp")
set(JS_MODULE_SOURCE_FILES ${JS_MODULE_CPP_FILES})

set(WASM_JS_TARGET_NAME Js${MODULE_BASE_NAME})
add_custom_target(${WASM_JS_TARGET_NAME} ALL SOURCES ${JS_MODULE_SOURCE_FILES})
add_dependencies(${WASM_JS_TARGET_NAME} ${WASM_LIBRARY_TARGET_NAME})

add_custom_command(TARGET ${WASM_JS_TARGET_NAME} POST_BUILD
                   COMMAND ${emcc_EXECUTABLE}
                   ARGS
                   ${JS_MODULE_CPP_FILES}
                   -std=c++11 -Wall --bind -s WASM=0 -s EXPORT_ALL=1 -s SINGLE_FILE=1
                   -l${WASM_LIBRARY_TARGET_NAME}${WASM_DEBUG_POSTFIX} -L${WASM_OUTPUT_DIR}
                   -lssl -lcrypto -L${SDK_OPENSSL_WASM_LIBRARY_DIR}
                   -I${CMAKE_CURRENT_SOURCE_DIR} -I${SDK_OPENSSL_WASM_INCLUDE_DIR} -I${CMAKE_SOURCE_DIR}/depends/json -I${CMAKE_SOURCE_DIR}/src/core -I${SDK_GENERATED_HPP_DIR}
                   -o ${JS_OUTPUT_FILE}
)

install(FILES "${LIBRARY_OUTPUT_FILE}" DESTINATION "lib" COMPONENT WASMModules)
install(FILES "${JS_OUTPUT_FILE}" DESTINATION "lib" COMPONENT WASMModules)
install(FILES "${LIBRARY_OUTPUT_FILE}" DESTINATION "example/javascript" COMPONENT "Examples")
install(FILES "${JS_OUTPUT_FILE}" DESTINATION "example/javascript" COMPONENT "Examples")
