#ifndef server_hpp
#define server_hpp
#include "session.hpp"

class server {
public:
    server(boost::asio::io_service& io_service, int port);
    void handle_accept(session* new_session, const boost::system::error_code& error);
    
private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

#endif /* server_hpp */
