#include "Multicast.hpp"
#include "../config.hpp"

#include <thread>

void NetClientHandler::listen_loop()
{
    std::array<uint8_t, RECV_DATA_LEN> recv_buffer;
    asio::ip::udp::endpoint sender_endpoint;

    this->isRunning.store(true, std::memory_order_release);
    try
    {
        while (this->isRunning.load(std::memory_order_acquire))
        {
            size_t bytes_received = socket.receive_from(asio::buffer(recv_buffer), sender_endpoint);
            uint8_t * data = recv_buffer.data();
            this->dataBuffer.push( data, bytes_received);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Receive error: " << e.what() << std::endl;
    }
}

void NetClientHandler::stop(){
    this->isRunning.store(false, std::memory_order_relaxed);
}

NetClientHandler::~NetClientHandler()
{
    stop();
    socket.close();
}