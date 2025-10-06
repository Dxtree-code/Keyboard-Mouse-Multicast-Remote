#include "internal_lib.hpp"
#include <thread>
#include <stdio.h>
TrackServer::TrackServer(std::string multicast_address, int multicast_port)
{
    std::cout << "Initializing Track Server" << std::endl;
    this->multicast_address = multicast_address; // Assign the parameter to the class member
    this->multicast_port = multicast_port;       // Assign the parameter to the class member

    this->kCapture = KeyboardCapture::GetInstance();
    this->capture = MouseCapture::GetInstance();
    this->server = new NetSenderHandler(this->io_context, this->multicast_address, this->multicast_port);
}

int TrackServer::startTrackServer()
{
    std::thread poller(PollMouseWindows, std::ref(*this->capture));
    poller.detach(); // run polling in background

    startHook();
    std::thread t(MessagePump);
    t.detach();

    std::thread trackKey(startKeyboardTrack);
    trackKey.detach();

    try
    {
        std::thread mouseSend([&]()
                              { this->server->send_loop(5, capture); });
        mouseSend.detach();
        this->server->send_loop(10, kCapture);
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

void TrackServer::sendStopSignal(int ip[4])
{
    uint8_t buf[16] = {};
    formatStopCommandData(buf, 16, ip);
    this->server->send_command(buf, 16);
}