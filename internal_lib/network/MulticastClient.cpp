#include "Multicast.hpp"
#include "../config.hpp"

void NetClientReceiver::listen_loop()
{
    std::array<uint8_t, RECV_DATA_LEN> recv_buffer;
    asio::ip::udp::endpoint sender_endpoint;

    MouseState mState;
    MouseState prevMState;

    KeyboardState kState;
    while (true)
    {
        try
        {
            size_t bytes_received = socket.receive_from(
                asio::buffer(recv_buffer), sender_endpoint);

            if (bytes_received != 16)
            { // check if buffer is valid
                std::cout << "Catch data with unrecognize bytesize" << std::endl;
                continue;
            }

            auto data = recv_buffer.data();
            if (isMouseData(data, 16))
            {
                parseMouseData(mState, data, bytes_received);
                WinApplyMouseState(mState, prevMState);
            }
            else if (isKeyboardData(data, 16))
            {
                parseKeyboardData(kState, data, 16);
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
                        return;
                    }
                }
            }
        }
        catch (std::exception &e)
        {
            std::cerr << "Receive error: " << e.what() << std::endl;
        }
    }
}
