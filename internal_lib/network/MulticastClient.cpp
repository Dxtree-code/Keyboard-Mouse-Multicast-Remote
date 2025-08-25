#include "Multicast.hpp"
void  UdpMultiCastClient::listen_loop() {
    std::array<uint8_t, 1024> recv_buffer;
    asio::ip::udp::endpoint sender_endpoint;

    MouseState mState;
    MouseState prevState;
    while (true) {
        try {
            size_t bytes_received = socket.receive_from(
                asio::buffer(recv_buffer), sender_endpoint);

            if (bytes_received >= 16) { // check if buffer is valid
                parseMouseData(mState, recv_buffer.data(), bytes_received);

                // Handle the received MouseState
                std::cout << "Received packet from " 
                            << sender_endpoint.address().to_string() << ": "
                            << "dx=" << mState.dx 
                            << ", dy=" << mState.dy 
                            << ", left=" << mState.leftClick
                            << ", right=" << mState.rightClick
                            << ", mid=" << mState.midClick
                            << std::endl;
                
                WinApplyMouseState(mState, prevState);
            }
        } catch (std::exception& e) {
            std::cerr << "Receive error: " << e.what() << std::endl;
        }
    }
}
