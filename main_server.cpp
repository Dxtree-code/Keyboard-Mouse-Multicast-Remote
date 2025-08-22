#include "internal_lib/MulticastServer.h"


int main() {
    try {
        asio::io_context io_context;

        // Configure server
        std::string multicast_address = "172.29.224.207";
        unsigned short multicast_port = 12345;

        UdpMulticastServer server(io_context, multicast_address, multicast_port);

        // Run the sending loop
        server.send_loop(1);

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}