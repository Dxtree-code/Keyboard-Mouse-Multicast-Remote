#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <memory>

#include "asio.hpp"

#include "mmki/keyboard/keyboard_capture.hpp"
#include "mmki/mouse/mouse_capture.hpp"
#include "mmki/network/multicast_server.hpp"
#include "mmki/mouse/mouse.hpp"
#include "mmki/keyboard/keyboard.hpp"

using std::atomic;
using std::string;
using std::thread;
using std::shared_ptr;

class TrackServer
{
    shared_ptr<MouseCapture> mCapture;
    shared_ptr<KeyboardCapture> kCapture;
    asio::io_context io_context;
    NetServerHandler server;
    
    MouseTracker &mouseTracker;
    KeyboardTracker &keyboardTracker;

    thread mouseTrackerThread;
    thread keyboardTrackerThread;
    thread mouseSenderThread;
    thread keyboardSenderThread;

    atomic<bool> isRunning;

    private:
    void setIsRunning(bool value);
    bool getIsRunning();
    public:
    int startTrackServer();
    TrackServer(string multicast_address, int multicast_port);
    void wait();
    void stop();

    void sendStopSignal(int ip[4]);
};