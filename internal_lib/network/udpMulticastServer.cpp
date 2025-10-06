#include "Multicast.hpp"

// INI SANGAT GK BAGUS, BUAT KEJAR DEADLINE, DI OVERLOAD AJA DULU,
// NANTI VU BENERIN BIAR BAGUS, MAU PAKE CALLBACK ATAU APA TERSERAH
void NetSenderHandler::send_loop(int interval_ms, MouseCapture *mouseCapture)
{
    MouseState state;
    uint8_t buf[16] = {};

    while (true)
    {
        if (mouseCapture->poll(state))
        {
            formatMouseData(state, buf, 16);
            asio::error_code ec;
            if (socket.is_open())
                socket.send_to(asio::buffer(buf, 16), multicast_endpoint, 0, ec);
            if (ec)
            {
                std::cerr << "mouse send error: " << ec.message() << std::endl;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
}

void NetSenderHandler::send_loop(int interval_ms, KeyboardCapture *keyboardCapture)
{
    KeyboardState state;
    uint8_t buf[16] = {};
    asio::error_code ec;

    while (true)
    {
        if (keyboardCapture->poll(state))
        {
            std::cout << "isKeyDown=" << state.press
                      << ", code=" << state.code << std::endl;

            formatKeyboardData(state, buf, 16);

            socket.send_to(asio::buffer(buf, 16), multicast_endpoint, 0, ec);
            if (ec)
            {
                std::cerr << "Keyboard send_to failed: " << ec.message()
                          << " (" << ec.value() << ")" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
}

void NetSenderHandler::send_command(uint8_t *data, int len)
{
    asio::error_code ec;
    socket.send_to(asio::buffer(data, len), multicast_endpoint, 0, ec);
    if (ec)
    {
        std::cerr << "send_command failed: " << ec.message()
                  << " (" << ec.value() << ")" << std::endl;
    }
}
