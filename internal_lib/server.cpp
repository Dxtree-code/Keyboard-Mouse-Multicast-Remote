#include "internal_lib.hpp"
#include <thread>


int startTrackServer(){

    // start mouse capture
    MouseCapture *capture = MouseCapture::GetInstance();
    std::thread poller(PollMouseWindows, std::ref(*capture));
    poller.detach(); // run polling in background

    startHook();
    std::thread t(MessagePump);


    // get and start keyboard Tracker
    KeyboardCapture *kCapture = KeyboardCapture::GetInstance();
    std::thread trackKey(startKeyboardTrack);
    trackKey.detach();

     try {
        asio::io_context io_context;

        // Configure server
        std::string multicast_address = "10.22.65.108";
        unsigned short multicast_port = 12345;

        UdpMulticastServer server(io_context, multicast_address, multicast_port);

        // Run the sending loop
        std::thread mouseSend([&]() {
            server.send_loop(10, capture);
        });
        mouseSend.detach();
        server.send_loop(10, kCapture);

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}