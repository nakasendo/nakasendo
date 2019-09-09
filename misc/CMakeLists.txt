#################################################################
#  Date             03/09/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

if(NOT BUILD_MISC_ONLY)## Define -DBUILD_MISC_ONLY=ON to build o only prototypes in misc 
    message("Missing BUILD_MISC_ONLY")
    message("Define -DBUILD_MISC_ONLY=ON to build o only prototypes in misc")
else()
    add_subdirectory(PyThresholdSignature)
    add_subdirectory(WASMModule)
endif()

