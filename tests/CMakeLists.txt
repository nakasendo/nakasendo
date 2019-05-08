#################################################################
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
target_link_libraries(SymEncDecTest SymEncDec)
set_property(TARGET MessageHashClient PROPERTY FOLDER "tests")


## Add unit testing. TODO : remove all the above test cases into subdirectory and make it a real tests (with CHECKING results)
add_subdirectory(unit)
#add_subdirectory(functional)
#add_subdirectory(prog) # test program like running benchmark performance, demos programs
