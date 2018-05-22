#ifndef Client_hpp
#define Client_hpp

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <map>
#include <mutex>

using std::string;
using std::cout;
using std::endl;
using std::map;

//static boost::asio::io_service service;

class Client : public boost::enable_shared_from_this<Client>, boost::noncopyable {
private:
    boost::asio::ip::tcp::socket socket_;
    static const size_t max_msg_size = 1024;
    char read_buffer_[max_msg_size];
    char write_buffer_[max_msg_size];
    bool started_;
    string message_;
    
public:
    static boost::shared_ptr<Client> start(boost::asio::ip::tcp::endpoint ep, const string& message, boost::asio::io_service& service);
    void stop();
    bool started() { return started_; }
    static void print_out();
    
private:
    Client(const string& message, boost::asio::io_service& service);
    void start(boost::asio::ip::tcp::endpoint ep);
    void on_connect(const boost::system::error_code& err);
    void on_read(const boost::system::error_code& err, size_t bytes);
    void on_write(const boost::system::error_code& err, size_t bytes);
    void do_read();
    void do_write(const string& msg);
    size_t read_complete(const boost::system::error_code& err, size_t bytes);
};

#endif /* Client_hpp */
