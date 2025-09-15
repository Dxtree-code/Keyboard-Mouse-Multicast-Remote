#pragma once
#include "./network/Multicast.hpp"
#include "./mouse/Mouse.hpp"



struct TrackServer{
    MouseCapture *capture;
    KeyboardCapture *kCapture;
    asio::io_context io_context;
    UdpMulticastServer * server;
    std::string multicast_address;
    int multicast_port;
    
    static TrackServer *instance; 

    int startTrackServer();
    static TrackServer* getInstance();
    TrackServer(std::string multicast_address, int multicast_port);
    
    void sendStopSignal(int ip[4]);
};

void startClient(std::string listenAddr, int port);