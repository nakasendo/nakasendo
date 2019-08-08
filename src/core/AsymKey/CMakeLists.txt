#################################################################
#  Date             07/05/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(ASYM_KEY_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/AsymKey.h
  ${CMAKE_CURRENT_SOURCE_DIR}/AsymKeyAPI.h
)
set(ASYM_KEY_HPP_FILES_PRIVATE # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/AsymKeyImpl.h
)
set(ASYM_KEY_HPP_FILES ${ASYM_KEY_HPP_FILES_PUBLIC} ${ASYM_KEY_HPP_FILES_PRIVATE})

set(ASYM_KEY_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/AsymKey.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/AsymKeyAPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/AsymKeyImpl.cpp
)
set(AsymKey_CPP_FILES "${ASYM_KEY_CPP_FILES}" CACHE INTERNAL "Cache list of AsymKey cpp files")

set(ASYM_KEY_SOURCE_FILES ${ASYM_KEY_HPP_FILES} ${ASYM_KEY_CPP_FILES})
add_library(AsymKey ${LIBS_BUILD_TYPE} ${ASYM_KEY_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
  target_link_libraries(AsymKey crypt32 ws2_32)
endif()
target_link_libraries(AsymKey OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} BigNumbers MessageHash Polynomial SecretSplit)
target_compile_definitions(AsymKey PRIVATE EXPORT_AsymKey)
set_property(TARGET AsymKey PROPERTY FOLDER "core")
sdkAppendToGlobalSet(SDK_CORE_COMPONENT_LIST AsymKey)

install(FILES ${ASYM_KEY_HPP_FILES_PUBLIC} DESTINATION "include/AsymKey" COMPONENT core)
install(TARGETS AsymKey DESTINATION "lib" COMPONENT core)
