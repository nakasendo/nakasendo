#################################################################
#  Date             15/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################
set(SYM_ENC_DEC_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDec.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDecAPI.h
)
set(SYM_ENC_DEC__HPP_FILES_PRIVATE # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDecImpl.h
)

set(SYM_ENC_DEC_HPP_FILES ${SYM_ENC_DEC_HPP_FILES_PUBLIC} ${SYM_ENC_DEC__HPP_FILES_PRIVATE})

set(SYM_ENC_DEC_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDec.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDecAPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/SymEncDecImpl.cpp
)
set(SymEncDec_CPP_FILES "${SYM_ENC_DEC_CPP_FILES}" CACHE INTERNAL "Cache list of Sym Encrypt Decrypt cpp files")

set(SymEncDec_SOURCE_FILES ${SYM_ENC_DEC_HPP_FILES} ${SymEncDec_CPP_FILES})
add_library(SymEncDec ${LIBS_BUILD_TYPE} ${SymEncDec_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
	target_link_libraries(SymEncDec crypt32 ws2_32)	
endif()
target_link_libraries(SymEncDec OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(SymEncDec PRIVATE EXPORT_SymEncDec)
set_property(TARGET SymEncDec PROPERTY FOLDER "core")
sdkAppendToGlobalSet(SDK_CORE_COMPONENT_LIST SymEncDec)

install(FILES ${SYM_ENC_DEC_HPP_FILES_PUBLIC} DESTINATION "include/SymEncDec" COMPONENT CPP)
install(TARGETS SymEncDec DESTINATION "lib" COMPONENT CPP)
