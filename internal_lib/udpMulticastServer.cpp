#include "MulticastServer.h"


void UdpMulticastServer::send_loop(int interval_seconds, MouseCapture * mouseCapture) {
    MouseState state;
    uint8_t buf[16]={};
    while (true) {
        if (mouseCapture->poll(state)){
            formatMouseData(state, buf, 16);
            // std::cout
            //         << "dx=" << state.dx 
            //         << ", dy=" << state.dy 
            //         << ", dz=" << state.dScroll 
            //         << ", left=" << state.leftClick
            //         << ", right=" << state.rightClick
            //         << ", mid=" << state.midClick
            //         << std::endl;
            socket.send_to(asio::buffer(buf, 16), multicast_endpoint);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_seconds));
    }
}
