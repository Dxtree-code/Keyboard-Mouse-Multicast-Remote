#include "MulticastServer.h"


void UdpMulticastServer::send_loop(int interval_seconds = 1) {
    while (true) {
        socket.send_to(asio::buffer(message), multicast_endpoint);
        std::cout << "Sent: " << message << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(interval_seconds));
    }
}

// int main() {
//     try {
//         asio::io_context io_context;

//         // Configure server
//         std::string multicast_address = "239.0.0.1";
//         unsigned short multicast_port = 12345;
//         std::string message = "Hello Multicast Clients!";

//         UdpMulticastServer server(io_context, multicast_address, multicast_port, message);

//         // Run the sending loop
//         server.send_loop();

//     } catch (std::exception& e) {
//         std::cerr << "Exception: " << e.what() << std::endl;
//     }

//     return 0;
// }
