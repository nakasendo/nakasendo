#################################################################
#  Date             15/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
set(MESSAGE_HASH_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64EncDec.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashAPI.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHash.h
)
set(MESSAGE_HASH_HPP_FILES_PRIVATE # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64EncDecImpl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashImpl.h
)

set(MESSAGE_HASH_HPP_FILES ${MESSAGE_HASH_HPP_FILES_PUBLIC} ${MESSAGE_HASH_HPP_FILES_PRIVATE})

set(MESSAGE_HASH_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64EncDec.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/Base64EncDecImpl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashAPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHash.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/MessageHashImpl.cpp
)
set(MessageHash_CPP_FILES "${MESSAGE_HASH_CPP_FILES}" CACHE INTERNAL "Cache list of MessageHash cpp files")

set(MESSAGE_HASH_SOURCE_FILES ${MESSAGE_HASH_HPP_FILES} ${MESSAGE_HASH_CPP_FILES})
add_library(MessageHash ${LIBS_BUILD_TYPE} ${MESSAGE_HASH_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
  target_link_libraries(MessageHash crypt32 ws2_32)
endif()
target_link_libraries(MessageHash OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(MessageHash PRIVATE EXPORT_MessageHash)
set_property(TARGET MessageHash PROPERTY FOLDER "core")

install(FILES ${MESSAGE_HASH_HPP_FILES_PUBLIC} DESTINATION "include/MessageHash" COMPONENT CPP)
install(TARGETS MessageHash DESTINATION "lib" COMPONENT CPP)
