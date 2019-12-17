#include <serverConnection.h>
//#include <TSMessageFactory.h>
#include <TSMessageHandler.h>

void session::do_read(){
   auto self(shared_from_this());
    boost::asio::streambuf b;
    boost::asio::streambuf::mutable_buffers_type bufs = b.prepare(8192);
    size_t n = socket_.receive(bufs);
    b.commit(n);
    std::istream is(&b);
    MessageDescriptor msgid;
    is >> reinterpret_cast<std::underlying_type<TSMessageDefs>::type &>( msgid.m_EnumMSGID);
    //is >> msgid.m_EnumMSGID ;
    if(!HandleMessage(msgid, is)){
        std::cout << "Unable to handle request message" << std::endl;
        return;
    }
    do_write(msgid);
}

void session::do_write(MessageDescriptor& requestMsgID){
    auto self(shared_from_this());
    boost::asio::streambuf b;
    std::ostream os(&b);
    if( !CreateResponseMessage(requestMsgID,os)){
        std::cout << "Unable to create response message for request ID : " << requestMsgID.m_EnumMSGID << std::endl;
        return;
    }
    size_t n = socket_.send(b.data());
    b.consume(n);

}


void server::do_accept(){
m_acceptor.async_accept(
    [this](boost::system::error_code ec, tcp::socket socket)
    {
      if (!ec)
      {
        std::make_shared<session>(std::move(socket))->start();
      }

      do_accept();
    });

}
