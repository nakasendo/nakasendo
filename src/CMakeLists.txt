#################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

#TODO create a variable holding all source files from core to make the wasm build later in modules wasm
add_subdirectory(core)
add_subdirectory(modules)
add_subdirectory(ThresholdSignatures)
