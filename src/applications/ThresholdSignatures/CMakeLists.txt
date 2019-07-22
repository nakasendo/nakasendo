#################################################################
#  Date             11/06/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(THRESHOLD_SIGNATURES_HPP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/ThresholdSignatures.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Group.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupState.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Player.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UUID.h
    ${CMAKE_CURRENT_SOURCE_DIR}/polynomial/KnownPolynomial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/polynomial/Polynomial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/EnumCast.h
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/Message.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupBuilder.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupResponseMessage.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupSetupMessage.h

)

set(THRESHOLD_SIGNATURES_CPP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Group.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UUID.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/Message.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupBuilder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupResponseMessage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GroupSetupMessage.cpp
)


set(THRESHOLD_SIGNATURES_SOURCE_FILES ${THRESHOLD_SIGNATURES_HPP_FILES} ${THRESHOLD_SIGNATURES_CPP_FILES})

add_library(ThresholdSignatures ${LIBS_BUILD_TYPE} ${THRESHOLD_SIGNATURES_SOURCE_FILES} )

target_link_libraries(ThresholdSignatures BigNumbers MessageHash ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(ThresholdSignatures PUBLIC ${SDK_APPLICATION_SOURCE_DIR})
#target_include_directories(ThresholdSignatures/polynomial PUBLIC ${SDK_APPLICATION_SOURCE_DIR})
target_compile_definitions(ThresholdSignatures PRIVATE EXPORT_ThresholdSignatures)
set_property(TARGET ThresholdSignatures PROPERTY FOLDER "applications")


## Do not install anything because it is at applications level
