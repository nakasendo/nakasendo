################################################################
#  Date             01/08/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

add_subdirectory(cpp)
add_subdirectory(python)
add_subdirectory(javascript)
add_subdirectory(TS_protobuf)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/README" DESTINATION "example" COMPONENT "Examples")
