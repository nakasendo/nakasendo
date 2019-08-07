#################################################################
#  Date             02/08/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(SECRETSPLIT_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/KeyShare.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SecretSplit.h
)
set(SECRETSPLIT_HPP_FILES ${SECRETSPLIT_HPP_FILES_PUBLIC})

set(SECRETSPLIT_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/KeyShare.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SecretSplit.cpp
)
set(SecretSplit_CPP_FILES "${SECRETSPLIT_CPP_FILES}" CACHE INTERNAL "Cache list of SecretSplit cpp files")

set(SECRETSPLIT_SOURCE_FILES ${SECRETSPLIT_HPP_FILES} ${SECRETSPLIT_CPP_FILES})
add_library(SecretSplit ${LIBS_BUILD_TYPE} ${SECRETSPLIT_SOURCE_FILES})

target_link_libraries(SecretSplit Polynomial BigNumbers MessageHash)
target_compile_definitions(SecretSplit PRIVATE EXPORT_SecretSplit)
set_property(TARGET SecretSplit PROPERTY FOLDER "core")
sdkAppendToGlobalSet(SDK_CORE_COMPONENT_LIST SecretSplit)

install(FILES ${SECRETSPLIT_HPP_FILES_PUBLIC} DESTINATION "include/SecretSplit" COMPONENT core)
install(TARGETS SecretSplit DESTINATION "lib" COMPONENT core)
