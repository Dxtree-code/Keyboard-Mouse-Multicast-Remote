#include "client.h"
void startClient(std::string listenAddr, int port){
    asio::io_context io_context;
    
    try{
        UdpMultiCastClient client(io_context, listenAddr, port);
        client.listen_loop();

    }catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}