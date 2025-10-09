#include "internal_lib.hpp"


ListenerClient::ListenerClient(string listenAddress, int port):
    io_context(),
    dataBuffer(),
    netClient(io_context, listenAddress, port, dataBuffer)
{

}

void ListenerClient::startListener(){
    this->netClient.listen_loop();
}

void ListenerClient::startClient(){
    if (this->isRunning.load(std::memory_order_acquire) == true){
        return;
    }
    this->isRunning.store(true, std::memory_order_release);
    this->listenThread = thread([&](){
        this->startListener();
    });
    this->executorThread =  thread([&](){
        this->startExecutor();
    });

    if (this->executorThread.joinable()){
        this->executorThread.join();
    }
    if (this->listenThread.joinable()){
        this->listenThread.join();
    }
}


void ListenerClient::startExecutor(){
     // TO DO: Create function recv ( uint8_t, bytes_received) {Do Something}
    MouseState mState;
    MouseState prevMState;

    KeyboardState kState;
    while(this->isRunning.load(std::memory_order_acquire)){
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
                    this->isRunning.store(false, std::memory_order_release);
                }
            }
        }
    }
}