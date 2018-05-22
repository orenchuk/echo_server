#include "server.hpp"

int main(int argc, char* argv[]) {
    try {
        int port = 8001;
        boost::asio::io_service service;
        server server_(service, port);
        service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
