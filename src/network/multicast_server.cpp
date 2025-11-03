#include <thread>

#include "mmki/network/multicast_server.hpp"
#include "mmki/tools/serializer.hpp"

void NetServerHandler::setIsRunning(bool value){
    this->isRunning.store(value, std::memory_order_release);
}
bool NetServerHandler::getIsRunning(){
    return this->isRunning.load(std::memory_order_acquire);
}
NetServerHandler::NetServerHandler(asio::io_context &io, const std::string &address,unsigned short port):
        io_context(io),
        socket(io_context, asio::ip::udp::v4()),
        multicast_endpoint(asio::ip::make_address(address), port),
        isRunning(true){}

void NetServerHandler::sendLoop(int interval_ms, shared_ptr<MouseCapture> mouseCapture)
{
    MouseState state;
    uint8_t buf[16] = {};
    this->setIsRunning(true);

    while (this->getIsRunning())
    {
        if (mouseCapture->poll(state))
        {
            formatMouseData(state, buf, 16);
            asio::error_code ec;
            if (socket.is_open())
                socket.send_to(asio::buffer(buf, 16), multicast_endpoint, 0, ec);
            if (ec)
            {
                // std::cerr << "mouse send error: " << ec.message() << std::endl;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
}

void NetServerHandler::sendLoop(int interval_ms, shared_ptr<KeyboardCapture> keyboardCapture)
{
    KeyboardState state;
    uint8_t buf[16] = {};
    asio::error_code ec;

    this->setIsRunning(true);
    while (this->getIsRunning())
    {
        if (keyboardCapture->poll(state))
        {
            formatKeyboardData(state, buf, 16);

            socket.send_to(asio::buffer(buf, 16), multicast_endpoint, 0, ec);
            if (ec)
            {
                // std::cerr << "Keyboard send_to failed: " << ec.message()
                //           << " (" << ec.value() << ")" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
}

void NetServerHandler::sendCommand(uint8_t *data, int len)
{
    asio::error_code ec;
    socket.send_to(asio::buffer(data, len), multicast_endpoint, 0, ec);
    if (ec)
    {
        // std::cerr << "send_command failed: " << ec.message()
        //           << " (" << ec.value() << ")" << std::endl;
    }
}

void NetServerHandler::stop(){
    this->setIsRunning(false);
}