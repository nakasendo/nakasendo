#include <MessageRequestResponse.h>
#include <TSMessageHandler.h>

// send request only ... no need to wait for a response
void genericRequest(std::ostream& os, boost::asio::streambuf& b, const std::string& addr, const std::string port){
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    //tcp::resolver::results_type endpoints = resolver.resolve(argv[1], argv[2]);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(addr,port);

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
    size_t n = socket.send(b.data());

    b.consume(n); // sent data is removed from input sequence
    return;
}


void genericRequestResponse(std::ostream& os, boost::asio::streambuf& b, const std::string& addr, const std::string port){
     
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(addr,port);

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
    size_t n = socket.send(b.data());

    b.consume(n); // sent data is removed from input sequence

    // wait for response
    
    boost::asio::streambuf c;
    boost::asio::streambuf::mutable_buffers_type bufs = c.prepare(4096);
    size_t mn = socket.receive(bufs);
    c.commit(mn);
    
    std::istream is(&c);
    MessageDescriptor msgid;
    is >> reinterpret_cast<std::underlying_type<TSMessageDefs>::type &>( msgid.m_EnumMSGID);
    if(!HandleMessage(msgid, is)){
        std::cout << "Failure to handle the response message with message id: " << msgid.m_EnumMSGID << std::endl;
    }
    
    return ;
}


