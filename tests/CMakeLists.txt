################################################################
#  Date             24/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

add_executable(BigNumbersClient BigNumbersClient.cpp)
target_link_libraries(BigNumbersClient BigNumbers)
set_property(TARGET BigNumbersClient PROPERTY FOLDER "tests")

add_executable(BigNumbersRandTest BigNumbersRandTest.cpp)
target_link_libraries(BigNumbersRandTest BigNumbers)
set_property(TARGET BigNumbersRandTest PROPERTY FOLDER "tests")

add_executable(MessageHashClient MessageHashClient.cpp)
target_link_libraries(MessageHashClient MessageHash)
set_property(TARGET MessageHashClient PROPERTY FOLDER "tests")

add_executable(SymEncDecTest SymEncDecTest.cpp)
target_link_libraries(SymEncDecTest SymEncDec MessageHash)
set_property(TARGET SymEncDecTest PROPERTY FOLDER "tests")

add_executable(ECPointTest ECPointTest.cpp)
target_link_libraries(ECPointTest ECPoint BigNumbers)
set_property(TARGET ECPointTest PROPERTY FOLDER "tests")


## To run a single unit test, just run the compiled binary
add_subdirectory(unit)
## To run a single functional test in python :
## python test_script.py --data_dir=/absolute/path/to/data/dir --module_dir=/absolute/path/to/PyModules/dir
add_subdirectory(functional)
#add_subdirectory(prog) # test program like running benchmark performance, demos programs
