#pragma once
#include "./network/Multicast.hpp"
#include "./mouse/Mouse.hpp"

struct TrackServer
{
    MouseCapture *capture;
    KeyboardCapture *kCapture;
    asio::io_context io_context;
    NetInputSender *server;
    std::string multicast_address;
    int multicast_port;

    int startTrackServer();
    TrackServer(std::string multicast_address, int multicast_port);

    void sendStopSignal(int ip[4]);
};

void startClient(std::string listenAddr, int port);