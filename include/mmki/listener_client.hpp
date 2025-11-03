#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <chrono>

#include "asio.hpp"

#include "mmki/keyboard/keyboard.hpp"
#include "mmki/keyboard/keyboard_capture.hpp"
#include "mmki/mouse/mouse.hpp"
#include "mmki/mouse/mouse_capture.hpp"
#include "mmki/network/multicast_client.hpp"
#include "mmki/network/net.hpp"

using std::atomic;
using std::string;
using std::thread;

struct ListenerClient
{
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<Clock> TimePoint;

    asio::io_context io_context;
    NetBuffer dataBuffer;
    NetClientHandler netClient;

    thread listenThread;
    thread executorThread;
    
    MouseExecutor &mousex;
    KeyboardExecutor &keyboardx;
    

    atomic<bool> isRunning = false;
    atomic<bool> hasNewData = false;

    int lifeDuration = 0; // in second;
    TimePoint lifeLimit;
    ListenerClient(string listenAddress, int port);
    ListenerClient(string listenAddress, int port, int lifeDuration);
    void startListener();
    void startExecutor();

    void startClient();
    void wait(); // wait all thread to stop and help handle time limit to live
    void stopClient();
};