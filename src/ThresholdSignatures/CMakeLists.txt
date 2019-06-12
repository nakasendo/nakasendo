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

set(Threshold_signatures_CPP_FILES "${THRESHOLD_SIGNATURES_CPP_FILES}"  CACHE INTERNAL "Cache list of Threshold signatures cpp files")

set(THRESHOLD_SIGNATURES_SOURCE_FILES ${THRESHOLD_SIGNATURES_HPP_FILES} ${THRESHOLD_SIGNATURES_CPP_FILES})

#find_package(nlohmann_json)

add_library(ThresholdSignatures ${LIBS_BUILD_TYPE} ${THRESHOLD_SIGNATURES_SOURCE_FILES} )

target_link_libraries(ThresholdSignatures BigNumbers MessageHash ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries(ThresholdSignatures nlohmann_json::nlohmann_json)
