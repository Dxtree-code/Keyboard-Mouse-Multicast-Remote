#include "server.h"
#include <thread>
#include "mouseTracker.h"
#include "MulticastServer.h"
int startTrackServer(){
    MouseCapture *capture = MouseCapture::GetInstance();

    std::thread poller(PollMouseWindows, std::ref(*capture));
    poller.detach(); // run polling in background

     try {
        asio::io_context io_context;

        // Configure server
        std::string multicast_address = "172.29.224.207";
        unsigned short multicast_port = 12345;

        UdpMulticastServer server(io_context, multicast_address, multicast_port);

        // Run the sending loop
        server.send_loop(10, capture);

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}