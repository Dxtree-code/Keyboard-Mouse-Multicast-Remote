#pragma once
#include "./network/Multicast.hpp"
#include "./mouse/Mouse.hpp"
#include "./config.hpp"
#include <atomic>

using std::string;
using std::thread;
using std::atomic;
using std::cout;
using std::endl;



struct TrackServer
{
    MouseCapture *capture;
    KeyboardCapture *kCapture;
    asio::io_context io_context;
    NetSenderHandler *server;

    int startTrackServer();
    TrackServer(std::string multicast_address, int multicast_port);

    void sendStopSignal(int ip[4]);
};

struct ListenerClient{
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::time_point<Clock> TimePoint;

    asio::io_context io_context;
    NetReceiverBuffer dataBuffer;
    NetClientHandler netClient;

    thread listenThread;
    thread executorThread;

    atomic<bool> isRunning = false;
    atomic<bool>  hasNewData = false;
    
    int lifeDuration=0; // in second;
    TimePoint lifeLimit;
    ListenerClient(string listenAddress, int port);
    ListenerClient(string listenAddress, int port, int lifeDuration);
    void startListener();
    void startExecutor();

    void startClient();
    void wait(); // wait all thread to stop and help handle time limit to live
    void stopClient();
};