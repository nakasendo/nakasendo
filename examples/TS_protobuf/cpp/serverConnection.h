#ifndef __SERVER_CONNECTION_H__
#define __SERVER_CONNECTION_H__

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

class MessageDescriptor;

class session
  : public std::enable_shared_from_this<session>
{
    public:
        session(tcp::socket socket) : socket_(std::move(socket))
        { return ;}

        void start(){
            do_read();
        }

    private:
        void do_read();
        void do_write(MessageDescriptor&); 

        tcp::socket socket_;
        enum { max_length = 1024 };
        char data_[max_length];
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
  {
    do_accept();
  }

private:
    void do_accept(); 
    tcp::acceptor m_acceptor;
};


#endif // #ifndef __SERVER_CONNECTION_H__

