#################################################################
#  Date             29/07/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

set(POLYNOMIAL_HPP_FILES_PUBLIC # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/Polynomial.h
)
set(POLYNOMIAL_HPP_FILES ${POLYNOMIAL_HPP_FILES_PUBLIC})

set(POLYNOMIAL_CPP_FILES # Keep alphabet order
  ${CMAKE_CURRENT_SOURCE_DIR}/Polynomial.cpp
)
set(Polynomial_CPP_FILES "${POLYNOMIAL_CPP_FILES}" CACHE INTERNAL "Cache list of Polynomial cpp files")

set(POLYNOMIAL_SOURCE_FILES ${POLYNOMIAL_HPP_FILES} ${POLYNOMIAL_CPP_FILES})
add_library(Polynomial ${LIBS_BUILD_TYPE} ${POLYNOMIAL_SOURCE_FILES})

target_link_libraries(Polynomial BigNumbers)
target_compile_definitions(Polynomial PRIVATE EXPORT_Polynomial)
set_property(TARGET Polynomial PROPERTY FOLDER "core")
sdkAppendToGlobalSet(SDK_CORE_COMPONENT_LIST Polynomial)

install(FILES ${POLYNOMIAL_HPP_FILES_PUBLIC} DESTINATION "include/Polynomial" COMPONENT core)
install(TARGETS Polynomial DESTINATION "lib" COMPONENT core)