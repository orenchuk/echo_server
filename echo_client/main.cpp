#include <iostream>
#include <boost/thread.hpp>
#include "Client.hpp"
#include <thread>
#include <chrono>

inline std::chrono::steady_clock::time_point get_current_time_fenced() {
    assert(std::chrono::steady_clock::is_steady &&
           "Timer should be steady (monotonic).");
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::steady_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::seconds>(d).count();
}

int main(int argc, const char * argv[]) {
    try {
        boost::asio::io_service service;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 8001);
        string msg = "hello";
        auto start = get_current_time_fenced();
        for ( int i = 0; i < 50000; ++i) {
            Client::start(ep, msg, service);
            //std::this_thread::sleep_for(std::chrono::nanoseconds(100000000));
    //        boost::this_thread::sleep(boost::posix_time::millisec(100));
        }
        service.run();
        auto stop = get_current_time_fenced();
        auto total = to_us(stop - start);
        cout << "Time: " << total << "s" << endl;
        Client::print_out();
    } catch (std::exception& e) {
        cout << "Exception: " << e.what() << endl;
    }
    return 0;
}
