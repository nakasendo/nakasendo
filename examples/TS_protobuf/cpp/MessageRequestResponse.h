#ifndef __MESSAGE_REQUEST_RESPONSE_H__
#define __MESSAGE_REQUEST_RESPONSE_H__

#include <string>
#include <boost/asio.hpp>

void genericRequest(std::ostream& os, boost::asio::streambuf& b, const std::string& addr, const std::string port);
void genericRequestResponse(std::ostream& os, boost::asio::streambuf& b, const std::string& addr, const std::string port);



#endif //#ifndef __MESSAGE_REQUEST_RESPONSE_H__

