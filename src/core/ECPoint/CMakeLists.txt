#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(EC_POINT_HPP_FILES_PUBLIC # Keep alphabet order
	${CMAKE_CURRENT_SOURCE_DIR}/ECPoint.h
)
set(EC_POINT_HPP_FILES_PRIVATE # Keep alphabet order
	${CMAKE_CURRENT_SOURCE_DIR}/ECPointImpl.h
)
set(EC_POINT_HPP_FILES ${EC_POINT_HPP_FILES_PUBLIC} ${EC_POINT_HPP_FILES_PRIVATE})

set(EC_POINT_CPP_FILES # Keep alphabet order
	${CMAKE_CURRENT_SOURCE_DIR}/ECPoint.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ECPointImpl.cpp
)
set(ECPoint_CPP_FILES "${EC_POINT_CPP_FILES}" CACHE INTERNAL "Cache list of ECPoint cpp files")

set(EC_POINT_SOURCE_FILES ${EC_POINT_HPP_FILES} ${EC_POINT_CPP_FILES})
add_library(ECPoint ${LIBS_BUILD_TYPE} ${EC_POINT_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
	target_link_libraries(ECPoint crypt32 ws2_32)
endif()
target_link_libraries(ECPoint BigNumbers OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(ECPoint PRIVATE EXPORT_ECPoint)
set_property(TARGET ECPoint PROPERTY FOLDER "core")
sdkAppendToGlobalSet(SDK_CORE_COMPONENT_LIST ECPoint)

install(FILES ${EC_POINT_HPP_FILES_PUBLIC} DESTINATION "include/ECPoint" COMPONENT core)
install(TARGETS ECPoint DESTINATION "lib" COMPONENT core)
