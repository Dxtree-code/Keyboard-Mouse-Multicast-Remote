#include "mmki/TrackServer.hpp"

ListenerClient::ListenerClient(string listenAddress, int port) : io_context(),
                                                                 dataBuffer(),
                                                                 netClient(io_context, listenAddress, port, dataBuffer),
                                                                 lifeLimit(Clock::now() + std::chrono::seconds(10))
{
}

ListenerClient::ListenerClient(string listenAddress, int port, int lifeDur) : io_context(),
                                                                              dataBuffer(),
                                                                              netClient(io_context, listenAddress, port, dataBuffer),
                                                                              lifeDuration(lifeDur),
                                                                              lifeLimit(Clock::now() + std::chrono::seconds(lifeDur))
{
}

void ListenerClient::startListener()
{
    this->netClient.listen_loop();
}

void ListenerClient::startClient()
{
    if (this->isRunning.load(std::memory_order_acquire) == true)
    {
        return;
    }
    this->isRunning.store(true, std::memory_order_release);
    this->listenThread = thread([&]()
                                { this->startListener(); });
    this->executorThread = thread([&]()
                                  { this->startExecutor(); });
}

void ListenerClient::stopClient()
{
    this->netClient.stop();
    this->isRunning.store(false, std::memory_order_release);
    if (this->executorThread.joinable())
    {
        this->executorThread.join();
    }
    if (this->listenThread.joinable())
    {
        this->listenThread.join();
    }
}

void ListenerClient::wait()
{
    while (this->isRunning.load(std::memory_order_acquire))
    {
        TimePoint now = Clock::now();
        if (this->hasNewData.load(std::memory_order_acquire))
        {
            this->hasNewData.store(false, std::memory_order_release);
            this->lifeLimit = Clock::now() + std::chrono::seconds(this->lifeDuration);
        }
        if (this->lifeLimit < now)
        {
            this->isRunning = false;
            this->stopClient();
        }
        std::this_thread::sleep_for(std::chrono::seconds(WAIT_CHECKING_RATE));
    }
    this->stopClient();
}

void ListenerClient::startExecutor()
{
    MouseState mState;
    MouseState prevMState;

    KeyboardState kState;
    while (this->isRunning.load(std::memory_order_acquire))
    {
        NetRecvData *netData = this->dataBuffer.pop();
        if (netData == nullptr)
            continue;
        this->hasNewData.store(true, std::memory_order_release);

        uint8_t *data;
        int dataLen = netData->getData(&data);

        if (isMouseData(data, 16))
        {
            parseMouseData(mState, data, dataLen);
            winApplyMouseState(mState, prevMState);
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
                if (hasIp(ipdata))
                {
                    this->isRunning.store(false, std::memory_order_release);
                }
            }
        }
    }
}