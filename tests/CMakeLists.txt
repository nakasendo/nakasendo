################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

## To run a single unit test, just run the compiled binary
add_subdirectory(unit)
## To run a single functional test in python :
## python test_script.py --data_dir=/absolute/path/to/data/dir --module_dir=/absolute/path/to/PyModules/dir
add_subdirectory(functional)
#add_subdirectory(prog) # test program like running benchmark performance, demos programs