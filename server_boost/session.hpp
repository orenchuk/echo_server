#ifndef session_hpp
#define session_hpp

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class session {
public:
    session(boost::asio::io_service& io_service);
    tcp::socket& socket() { return socket_; }
    void start();
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);    
    void handle_write(const boost::system::error_code& error);
    
private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

#endif /* session_hpp */
