#include "Multicast.hpp"
#include "../config.hpp"

#include <thread>

void NetClientHandler::listen_loop()
{
    std::array<uint8_t, RECV_DATA_LEN> recv_buffer;
    asio::ip::udp::endpoint sender_endpoint;

    try
    {
        while (this->isRunning.load(std::memory_order_relaxed))
        {
            size_t bytes_received = socket.receive_from(asio::buffer(recv_buffer), sender_endpoint);
            uint8_t * data = recv_buffer.data();
            dataBuffer.push( data, bytes_received);

        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Receive error: " << e.what() << std::endl;
    }
}

void NetClientHandler::executor_loop(){
     // TO DO: Create function recv ( uint8_t, bytes_received) {Do Something}
    MouseState mState;
    MouseState prevMState;

    KeyboardState kState;
    while(this->isRunning.load(std::memory_order_relaxed)){
        NetRecvData  * netData =  this->dataBuffer.pop();
        if(netData  ==  nullptr) continue;

        uint8_t * data; 
        int dataLen  = netData->getData(&data);

        if (isMouseData(data, 16))
        {
            parseMouseData(mState, data, dataLen);
            WinApplyMouseState(mState, prevMState);
        }
        else if (isKeyboardData(data, dataLen))
        {
            parseKeyboardData(kState, data, dataLen);
            WinApplyKeyInput(kState.press, kState.code);
        }
        else if (isCommandData(data, 16))
        {
            int ipdata[4] = {0};
            SystemCommand cmd = parseCommandData(ipdata, data, 16);

            if (cmd == SystemCommand::STOP)
            {
                if (has_ip(ipdata))
                {
                    this->isRunning.store(false, std::memory_order_relaxed);
                }
            }
        }
    }
}

void NetClientHandler::start_process(){
    //initializing listen_loop and executor loop
    this->listenThread = std::thread([this](){
        this->listen_loop();
    });

    this->exectThread = std::thread([this](){
        this->executor_loop();
    });

    if (this->listenThread.joinable()){
        this->listenThread.join();
    }
    if (this->exectThread.joinable()){
        this->exectThread.join();
    }

}   

void NetClientHandler::stop_process(){
    this->isRunning.store(false, std::memory_order_relaxed);
    if (this->listenThread.joinable()){
        this->listenThread.join();
    }
    if (this->exectThread.joinable()){
        this->exectThread.join();
    }
}

NetClientHandler::~NetClientHandler()
{
    stop_process(); // ensure all threads are joined
    socket.close();
}