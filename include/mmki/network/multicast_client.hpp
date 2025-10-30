#pragma once

#include "asio.hpp"
#include "mmki/network/net.hpp"

// A structure to provide facade to start listning and apply input data from NetInputSender
// This used on Client
class NetClientHandler
{
    protected:
    asio::io_context &io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint listen_endpoint;
    asio::ip::address multicast_address;
    NetBuffer &dataBuffer;

    std::thread listenThread; // listen thread;
    std::thread exectThread;  // execution thread;

    std::atomic<bool> isRunning = true;
    // Constructor
    public:
    NetClientHandler(asio::io_context &io, std::string &multicast_ip, int port, NetBuffer &dataBuffer);

    ~NetClientHandler();
    
    void listen_loop();
    void stop();
};
