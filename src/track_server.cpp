#include <thread>
#include <stdio.h>

#include "mmki/track_server.hpp"
#include "mmki/tools/serializer.hpp"

using std::make_shared;
// Change to use member initilaizer;
TrackServer::TrackServer(std::string multicast_address, int multicast_port):
    mCapture(make_shared<MouseCapture>()),
    kCapture(make_shared<KeyboardCapture>()),
    io_context(),
    server(this->io_context, multicast_address, multicast_port),
    mouseTracker(MouseTracker::getMouseTracker(mCapture)),
    keyboardTracker(KeyboardTracker::getKeyboardTracker(kCapture))
{
  
}

int TrackServer::startTrackServer()
{
    //Tracker Mouse
    mouseTrackerThread =  thread( [this](){mouseTracker.pollMouse();});
    keyboardTrackerThread =  thread([this](){keyboardTracker.pollKeyboard();});

    try
    {
        mouseSenderThread = thread([this](){ this->server.sendLoop(SERVER_SEND_RATE, mCapture); });
        keyboardSenderThread = thread([this](){this->server.sendLoop(SERVER_SEND_RATE,kCapture); });
    }
    catch (std::exception &e)
    {
        // std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

void TrackServer::sendStopSignal(int ip[4])
{
    uint8_t buf[16] = {};
    formatStopCommandData(buf, 16, ip);
    this->server.sendCommand(buf, 16);
}