#################################################################
#  Date             02/08/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(SecretSplit_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/KeyShare.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SecretSplit.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SecretSplitConfig.h
)
set(SecretSplit_HPP_FILES ${SecretSplit_HPP_FILES_PUBLIC})

set(SecretSplit_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/KeyShare.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SecretSplit.cpp
)

set(SecretSplit_SOURCE_FILES ${SecretSplit_HPP_FILES} ${SecretSplit_CPP_FILES})
add_library(SecretSplit ${LIBS_BUILD_TYPE} ${SecretSplit_SOURCE_FILES})

target_link_libraries(SecretSplit Polynomial BigNumbers MessageHash)
target_compile_definitions(SecretSplit PRIVATE EXPORT_SecretSplit)
set_property(TARGET SecretSplit PROPERTY FOLDER "core")

install(FILES ${SecretSplit_HPP_FILES_PUBLIC} DESTINATION "include/SecretSplit" COMPONENT core)
install(TARGETS SecretSplit DESTINATION "lib" COMPONENT core)
