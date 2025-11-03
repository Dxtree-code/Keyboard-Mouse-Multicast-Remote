#include "mmki/network/multicast_client.hpp"

#include <thread>

NetClientHandler::NetClientHandler(asio::io_context &io, std::string &multicast_ip, int port, NetBuffer &dataBuffer) : 
    io_context(io),
    socket(io_context, asio::ip::udp::v4()),
    listen_endpoint(asio::ip::udp::v4(), port),
    multicast_address(asio::ip::make_address(multicast_ip)),
    dataBuffer(dataBuffer)
{
    socket.set_option(asio::ip::udp::socket::reuse_address(true));
    socket.bind(listen_endpoint);

    socket.set_option(asio::ip::multicast::join_group(multicast_address));
}


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
        // std::cerr << "Receive error: " << e.what() << std::endl;
    }
}

void NetClientHandler::stop(){
    this->isRunning.store(false, std::memory_order_release);
    this->socket.close();
}

NetClientHandler::~NetClientHandler()
{
    stop();
    socket.close();
}