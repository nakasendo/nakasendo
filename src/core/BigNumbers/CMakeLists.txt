#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(BIG_NUMBERS_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbersAPI.h
)
set(BIG_NUMBERS_HPP_FILES_PRIVATE # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbersImpl.h
)
set(BIG_NUMBERS_HPP_FILES ${BIG_NUMBERS_HPP_FILES_PUBLIC} ${BIG_NUMBERS_HPP_FILES_PRIVATE})

set(BIG_NUMBERS_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbersAPI.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/BigNumbersImpl.cpp
)
set(BigNumbers_CPP_FILES "${BIG_NUMBERS_CPP_FILES}" CACHE INTERNAL "Cache list of BigNumbers cpp files")

set(BIG_NUMBERS_SOURCE_FILES ${BIG_NUMBERS_HPP_FILES} ${BIG_NUMBERS_CPP_FILES})
add_library(BigNumbers ${LIBS_BUILD_TYPE} ${BIG_NUMBERS_SOURCE_FILES})
if(WIN32) #https://github.com/openssl/openssl/pull/1062
  target_link_libraries(BigNumbers crypt32 ws2_32)
endif()
target_link_libraries(BigNumbers OpenSSL::Crypto OpenSSL::SSL ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_compile_definitions(BigNumbers PRIVATE EXPORT_BigNumbers)
set_property(TARGET BigNumbers PROPERTY FOLDER "core")

install(FILES ${BIG_NUMBERS_HPP_FILES_PUBLIC} DESTINATION "include/BigNumbers" COMPONENT CPP)
install(TARGETS BigNumbers DESTINATION "lib" COMPONENT CPP)