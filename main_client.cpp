#include "src/TrackServer.hpp"
#include <iostream>
#include <string>
#include "src/extern.hpp"
int main()
{
    int port = 8080;
    std::string address = "239.255.0.1";
    ListenerClient client(address, port, 10);
    client.startClient();
    std::cout << "end";
    client.wait();
}