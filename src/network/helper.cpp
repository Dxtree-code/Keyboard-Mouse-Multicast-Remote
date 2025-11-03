#include "asio.hpp"

#include "mmki/network/helper.hpp"

std::string getHostname(){
    return asio::ip::host_name();
}

bool hasIp (int ip_arr[4]) {
    try {
        asio::io_context io;
        asio::ip::tcp::resolver resolver(io);

        auto results = resolver.resolve(asio::ip::host_name(), "");

        // Convert int array into a bytes array (uint8_t)
        std::array<unsigned char, 4> bytes = {
            static_cast<unsigned char>(ip_arr[0]),
            static_cast<unsigned char>(ip_arr[1]),
            static_cast<unsigned char>(ip_arr[2]),
            static_cast<unsigned char>(ip_arr[3])
        };
        
        asio::ip::address_v4 target(bytes);
        
        for (auto& entry : results) {
            auto addr = entry.endpoint().address();
            if (addr.is_v4() && addr == target) {
                return true;
            }
        }
    } catch (std::exception& e) {
        // std::cerr << "Error: " << e.what() << std::endl;
    }
    return false;
}