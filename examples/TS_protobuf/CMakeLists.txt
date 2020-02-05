################################################################
#  Date             01/08/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

if (WIN32)
	message("Windows protocol buffer issues, see DLLs vs. static liniking https://github.com/protocolbuffers/protobuf/blob/master/cmake/README.md")
  return()
endif()
add_subdirectory(cpp)
