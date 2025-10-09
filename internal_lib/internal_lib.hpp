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
    asio::io_context io_context;
    NetReceiverBuffer dataBuffer;
    NetClientHandler netClient;

    thread listenThread;
    thread executorThread;

    atomic<bool> isRunning = false;
    
    ListenerClient(string listenAddress, int port);
    void startListener();
    void startExecutor();
    void startClient();
    void stopClient();
};
void startClient();