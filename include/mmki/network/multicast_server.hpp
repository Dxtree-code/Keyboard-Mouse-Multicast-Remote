#pragma once

#include <memory>

#include "asio.hpp"
#include "mmki/keyboard/keyboard_capture.hpp"
#include "mmki/mouse/mouse_capture.hpp"
#include <atomic>

using std::atomic;
using std::shared_ptr;
// A structure to provide facade to send input / command to client
// This used on Server
class NetServerHandler
{
    protected:
    asio::io_context &io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint multicast_endpoint;
    
    atomic<bool> isRunning;

    void setIsRunning(bool value);

    public:
    NetServerHandler(asio::io_context &io, const std::string &address,unsigned short port);

    void sendLoop(int interval_seconds, shared_ptr<MouseCapture> mouseCapture);
    void sendLoop(int interval_seconds, shared_ptr<KeyboardCapture> keyboardCapture);
    void sendCommand(uint8_t *data, int len);
    
    bool getIsRunning();
    void stop();
};