#################################################################
#  Date             12/02/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

if(NOT Boost_FOUND)
  message(STATUS "SDK WARNS : Boost was not found on your system, thess examples will not be built")
  return()
endif()

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.69.0 COMPONENTS system thread chrono) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    add_executable(tsClient serverConnection.cpp MessageRequestResponse.cpp TSMessageHandler.cpp tsClient.cpp)
    target_link_libraries(tsClient BigNumbers MessageHash ECPoint Polynomial TSCore protobuf::libprotobuf ${Boost_LIBRARIES})
    target_include_directories(tsClient PUBLIC ${protobuf_generate_PROTOC_OUT_DIR})
    set_property(TARGET tsClient PROPERTY FOLDER "examples")
    list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/tsClient.cpp")
    
    add_executable(serverListener serverConnection.cpp MessageRequestResponse.cpp TSMessageHandler.cpp serverListener.cpp)
    target_link_libraries(serverListener BigNumbers MessageHash ECPoint Polynomial TSCore protobuf::libprotobuf ${Boost_LIBRARIES})
    target_include_directories(serverListener PUBLIC ${protobuf_generate_PROTOC_OUT_DIR})
    set_property(TARGET serverListener PROPERTY FOLDER "examples")
    list(APPEND cpp_example_files "${CMAKE_CURRENT_SOURCE_DIR}/serverListener.cpp")
endif()

