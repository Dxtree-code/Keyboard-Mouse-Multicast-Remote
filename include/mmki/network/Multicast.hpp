#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <stdint.h>
#include <vector>
#include <atomic>

#include "asio.hpp"
#include "mmki/config.hpp"
#include "mmki/mouse/Mouse.hpp"
#include "mmki/keyboard/keyboard.hpp"
#include "mmki/tools/serializer.hpp"
#include "mmki/mouse/mouse_capture.hpp

// A structure to provide facade to send input / command to client
// This used on Server
struct NetSenderHandler
{
    asio::io_context &io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint multicast_endpoint;

    NetSenderHandler(asio::io_context &io, const std::string &address,
                     unsigned short port)
        : io_context(io),
          socket(io_context, asio::ip::udp::v4()),
          multicast_endpoint(asio::ip::make_address(address), port)
    {
    }
    void sendLoop(int interval_seconds, MouseCapture *mouseCapture);
    void sendLoop(int interval_seconds, KeyboardCapture *KeyboardCapture);
    void sendCommand(uint8_t *data, int len);
};

// This struct used for transfer data between receiver thread and executor thread
struct NetRecvData
{
    std::array<uint8_t, RECV_DATA_LEN> data;
    int len;

    bool setData(uint8_t *data, int len);
    int getData(uint8_t **store);
};
// This Provide buffer from data received to executor thread
struct NetReceiverBuffer
{
    int head = 0;
    int tail = 0;
    std::atomic<int> count = 0;
    std::array<NetRecvData, RECV_BUFFER_LEN> buffer;

    bool push(uint8_t *data, int byteSize);
    NetRecvData *pop();
};

// A structure to provide facade to start listning and apply input data from NetInputSender
// This used on Client
class NetClientHandler
{
protected:
    asio::io_context &io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint listen_endpoint;
    asio::ip::address multicast_address;
    NetReceiverBuffer &dataBuffer;

    std::thread listenThread; // listen thread;
    std::thread exectThread;  // execution thread;

    std::atomic<bool> isRunning = true;
    // Constructor
public:
    NetClientHandler(asio::io_context &io, std::string &multicast_ip, int port, NetReceiverBuffer &dataBuffer) :
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

    ~NetClientHandler();

    void listen_loop();
    void stop();
};

std::string getHostname();

bool hasIp(int ip_arr[4]);
