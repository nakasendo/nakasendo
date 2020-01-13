
/* g++ -o serverListener -I. -I /Users/j.murphy/nChain/SDK-CLEAN/sdklibraries-john/src/core -I /Users/j.murphy/nChain/SDK-CLEAN/build/generated/hpp serverConnection.cpp MessageRequestResponse.cpp GroupMetaData.cpp TSMessageHandler.cpp TSMessageFactory.cpp serverListener.cpp ts.pb.cc orchestrator.cpp player.cpp -L /Users/j.murphy/nChain/SDK-CLEAN/build/x64/release -lprotobuf -lboost_system-mt -ldl -lBigNumbers -lMessageHash -lECPoint -lpolynomial -pthread -std=c++17*/



#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <ts_messages.pb.h>
#include <serverConnection.h>


using boost::asio::ip::tcp;




int main(int argc, char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    try{
        if (argc != 2){
          std::cerr << "Usage: async_tcp_echo_server <port>\n";
          return 1;
        }

        boost::asio::io_context io_context;
        server s(io_context, std::atoi(argv[1]));

        io_context.run();
    }
    catch (std::exception& e){
        std::cerr << "Exception: " << e.what() << "\n";
    }
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}

