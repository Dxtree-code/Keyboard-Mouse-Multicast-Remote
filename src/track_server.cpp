#include <thread>
#include <stdio.h>

#include "mmki/track_server.hpp"
#include "mmki/tools/serializer.hpp"
#include "mmki/keyboard/keyboard_factory.hpp"
#include "mmki/mouse/mouse_factory.hpp"

using std::make_shared;
// Change to use member initilaizer;
TrackServer::TrackServer(std::string multicast_address, int multicast_port):
    mCapture(make_shared<MouseCapture>()),
    kCapture(make_shared<KeyboardCapture>()),
    io_context(),
    server(this->io_context, multicast_address, multicast_port),
    mouseTracker(MouseFactory::getMouseTrackerByOs(mCapture)),
    keyboardTracker(KeyboardFactory::getKeyboardTrackerByOs(kCapture)),
    isRunning(true)
{
  
}

void TrackServer::setIsRunning(bool value){
    this->isRunning.store(value, std::memory_order_release);
}

bool TrackServer::getIsRunning(){
    return this->isRunning.load(std::memory_order_acquire);
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

void TrackServer::wait(){
    while(this->getIsRunning()){
        std::this_thread::sleep_for(std::chrono::seconds(WAIT_CHECKING_RATE));
    }
    this->stop();
}

void TrackServer::stop(){
    this->setIsRunning(false);
    this->mouseTracker.stop();
    this->keyboardTracker.stop();
    this->server.stop();
    
    if (this->mouseSenderThread.joinable()) this->mouseSenderThread.join();
    if (this->mouseTrackerThread.joinable()) this->mouseTrackerThread.join();
    if (this->keyboardSenderThread.joinable()) this->keyboardSenderThread.join();
    if (this->keyboardTrackerThread.joinable()) this->keyboardTrackerThread.join();
}

void TrackServer::sendStopSignal(int ip[4])
{
    uint8_t buf[16] = {};
    formatStopCommandData(buf, 16, ip);
    this->server.sendCommand(buf, 16);
}