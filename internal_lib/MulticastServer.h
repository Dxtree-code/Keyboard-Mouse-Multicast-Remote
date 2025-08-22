#pragma once

#include "../vendor/asio/asio/include/asio.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
struct UdpMulticastServer {
    asio::io_context& io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint multicast_endpoint;
    std::string message;

    UdpMulticastServer(asio::io_context& io, const std::string& address,
                       unsigned short port, const std::string& msg)
        : io_context(io),
          socket(io_context, asio::ip::udp::v4()),
          multicast_endpoint(asio::ip::make_address(address), port),
          message(msg)
    {

    }

    void send_loop(int interval_seconds);
};