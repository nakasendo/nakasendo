################################################################
#  Date             01/08/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

add_executable(BigNumbersClient BigNumbersClient.cpp)
target_link_libraries(BigNumbersClient BigNumbers)
set_property(TARGET BigNumbersClient PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/BigNumbersClient.cpp")

add_executable(BigNumbersRandTest BigNumbersRandTest.cpp)
target_link_libraries(BigNumbersRandTest BigNumbers)
set_property(TARGET BigNumbersRandTest PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/BigNumbersRandTest.cpp")

add_executable(MessageHashClient MessageHashClient.cpp)
target_link_libraries(MessageHashClient MessageHash)
set_property(TARGET MessageHashClient PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/MessageHashClient.cpp")

add_executable(SymEncDecTest SymEncDecTest.cpp)
target_link_libraries(SymEncDecTest SymEncDec MessageHash)
set_property(TARGET SymEncDecTest PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/SymEncDecTest.cpp")

add_executable(ECPointTest ECPointTest.cpp)
target_link_libraries(ECPointTest ECPoint BigNumbers)
set_property(TARGET ECPointTest PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/ECPointTest.cpp")

add_executable(SplitMessage shamirsSecret.cpp)
target_link_libraries(SplitMessage BigNumbers MessageHash Polynomial SecretSplit)
set_property(TARGET SplitMessage PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/shamirsSecret.cpp")

add_executable(RecoverMessage RecoverSecret.cpp)
target_link_libraries(RecoverMessage BigNumbers MessageHash Polynomial SecretSplit)
set_property(TARGET RecoverMessage PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/RecoverSecret.cpp")

add_executable(TSExample TSExample.cpp)
target_link_libraries(TSExample BigNumbers MessageHash ECPoint Polynomial AsymKey TSCore protobuf::libprotobuf)
target_include_directories(TSExample PUBLIC ${protobuf_generate_PROTOC_OUT_DIR})
set_property(TARGET TSExample PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/TSExample.cpp")

#sdkPrintList("cpp_example_files" "${cpp_example_files}")
foreach(_cpp_file ${cpp_example_files})
  install(FILES ${_cpp_file} DESTINATION "example/cpp" COMPONENT "Examples")
endforeach()
install(FILES exCMakeLists.txt DESTINATION "example/cpp" COMPONENT "Examples" RENAME CMakeLists.txt)
install(FILES exFindSDKLibraries.cmake DESTINATION "example/cpp/cmake" COMPONENT "Examples" RENAME FindSDKLibraries.cmake)

add_executable(KeyTest KeyTest.cpp)
target_link_libraries(KeyTest ECPoint SymEncDec BigNumbers MessageHash AsymKey BSVAddress)
set_property(TARGET KeyTest PROPERTY FOLDER "examples")
list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/KeyTest.cpp")
