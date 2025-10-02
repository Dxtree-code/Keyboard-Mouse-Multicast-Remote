#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stdint.h>
#include <vector>

#include "../../vendor/asio/asio/include/asio.hpp"
#include "../mouse/Mouse.hpp"
#include "../keyboard/keyboard.hpp"
#include "../tools/serializer.hpp"
#include "../mouse/MouseTracker.hpp"

// A structure to provide facade to send input / command to client
// This used on Server
struct NetInputSender
{
    asio::io_context &io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint multicast_endpoint;

    NetInputSender(asio::io_context &io, const std::string &address,
                   unsigned short port)
        : io_context(io),
          socket(io_context, asio::ip::udp::v4()),
          multicast_endpoint(asio::ip::make_address(address), port)
    {
    }
    void send_loop(int interval_seconds, MouseCapture *mouseCapture);
    void send_loop(int interval_seconds, KeyboardCapture *KeyboardCapture);
    void send_command(uint8_t *data, int len);
};

// This Provide buffer from data received to executor thread
struct NetReceiverBuffer{
    int head = 0;
    int tail = 0;
    std::array< std::array<uint8_t, RECV_DATA_LEN>, RECV_BUFFER_LEN>  buffer;
    
    void push(std::array<uint8_t, RECV_DATA_LEN> data);
    uint8_t * pop();
};

// A structure to provide facade to start listning and apply input data from NetInputSender
// This used on Client
struct NetClientReceiver
{
    asio::io_context &io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint listen_endpoint;
    asio::ip::address multicast_address;

    NetClientReceiver(asio::io_context &io, std::string &multicast_ip, int port): 
    io_context(io),
    socket(io_context, asio::ip::udp::v4()),
    listen_endpoint(asio::ip::udp::v4(), port),
    multicast_address(asio::ip::make_address(multicast_ip))
    {
        socket.set_option(asio::ip::udp::socket::reuse_address(true));
        socket.bind(listen_endpoint);

        socket.set_option(asio::ip::multicast::join_group(multicast_address));
    }
    
    void listen_loop();
};

std::string getHostname();

bool has_ip(int ip_arr[4]);