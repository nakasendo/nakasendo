#################################################################
#  Date             11/06/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(THRESHOLD_SIGNATURES_HPP_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Group.h
        ${CMAKE_CURRENT_SOURCE_DIR}/GroupState.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Player.h
        ${CMAKE_CURRENT_SOURCE_DIR}/UUID.h
)

set(THRESHOLD_SIGNATURES_CPP_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Group.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Player.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/UUID.cpp
)


set(THRESHOLD_SIGNATURES_SOURCE_FILES ${THRESHOLD_SIGNATURES_HPP_FILES} ${THRESHOLD_SIGNATURES_CPP_FILES})

add_library(ThresholdSignatures ${LIBS_BUILD_TYPE} ${THRESHOLD_SIGNATURES_SOURCE_FILES} )

target_link_libraries(ThresholdSignatures BigNumbers MessageHash ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

set_target_properties(ThresholdSignatures PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

