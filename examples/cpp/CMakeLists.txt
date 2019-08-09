################################################################
#  Date             01/08/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

add_executable(BigNumbersClient BigNumbersClient.cpp)
target_link_libraries(BigNumbersClient BigNumbers)
set_property(TARGET BigNumbersClient PROPERTY FOLDER "examples")

add_executable(BigNumbersRandTest BigNumbersRandTest.cpp)
target_link_libraries(BigNumbersRandTest BigNumbers)
set_property(TARGET BigNumbersRandTest PROPERTY FOLDER "examples")

add_executable(MessageHashClient MessageHashClient.cpp)
target_link_libraries(MessageHashClient MessageHash)
set_property(TARGET MessageHashClient PROPERTY FOLDER "examples")

add_executable(SymEncDecTest SymEncDecTest.cpp)
target_link_libraries(SymEncDecTest SymEncDec MessageHash)
set_property(TARGET SymEncDecTest PROPERTY FOLDER "examples")

add_executable(ECPointTest ECPointTest.cpp)
target_link_libraries(ECPointTest ECPoint BigNumbers)
set_property(TARGET ECPointTest PROPERTY FOLDER "examples")

add_executable(SplitMessage shamirsSecret.cpp)
target_link_libraries(SplitMessage BigNumbers MessageHash Polynomial SecretSplit)
set_property(TARGET SplitMessage PROPERTY FOLDER "examples")

add_executable(RecoverMessage RecoverSecret.cpp)
target_link_libraries(RecoverMessage BigNumbers MessageHash Polynomial SecretSplit)
set_property(TARGET RecoverMessage PROPERTY FOLDER "examples")

## TODO : 
## Create a CMakeLists.txt.in or some cmake module to pack with the examples/cpp for users
