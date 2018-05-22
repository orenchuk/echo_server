#include "Client.hpp"

map<string, int> server_response;
//std::mutex m;
int counter = 0, counter2 = 0, do_read_counter = 0;


Client::Client(const string& message, boost::asio::io_service& service)
    : socket_(service), started_(true), message_(message) {}

void Client::start(boost::asio::ip::tcp::endpoint ep) {
    socket_.async_connect(ep, boost::bind(&Client::on_connect, shared_from_this(), _1));
}

boost::shared_ptr<Client> Client::start(boost::asio::ip::tcp::endpoint ep, const string& message, boost::asio::io_service& service_) {
    boost::shared_ptr<Client> new_(new Client(message, service_));
    new_->start(ep);
    return new_;
}

void Client::stop() {
    if (!started_) return;
    started_ = false;
    socket_.close();
}

void Client::on_write(const boost::system::error_code& err, size_t bytes) {
    do_read(); // WTF???
}

void Client::do_write(const string& msg) {
    if (!started_) return;
    std::copy(msg.begin(), msg.end(), write_buffer_);
    socket_.async_write_some(boost::asio::buffer(write_buffer_, msg.size()), boost::bind(&Client::on_write, shared_from_this(), _1, _2));
}

void Client::on_connect(const boost::system::error_code& err) {
    counter++;
    if (!err) {
        do_write(message_ + "\n");
    } else {
        stop();
    }
}

void Client::on_read(const boost::system::error_code& err, size_t bytes) {
    counter2++;
    if (!err) {
        string copy(read_buffer_, bytes - 1);
//        cout << "server echoed our " << message_ << ": " << (copy == message_ ? "OK" : "FAIL") << endl;
        copy == message_ ? ++server_response["OK"] : ++server_response["FAIL"];
    } else { cout<<"on_read error occuired"<<endl; }
    stop();
}

void Client::do_read() {
    do_read_counter++;
    boost::asio::async_read(socket_, boost::asio::buffer(read_buffer_), boost::bind(&Client::read_complete, shared_from_this(), _1, _2), boost::bind(&Client::on_read, shared_from_this(), _1, _2));
}

size_t Client::read_complete(const boost::system::error_code& err, size_t bytes) {
    if (err) {
        cout << "read_complete error occuired" << endl;
        return 0;
    }
    bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

void Client::print_out() {
    cout << "OK: " << server_response["OK"] << endl;
    cout << "FAIL: " << server_response["FAIL"] << endl;
    cout << "on_connection counter: " << counter << endl << "on_read counter: " << counter2 << endl;
    cout << "do_read counter: " << do_read_counter << endl;
}

