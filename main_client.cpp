#include "internal_lib/internal_lib.hpp"
#include <iostream>
#include <string>
#include "internal_lib/extern.hpp"
int main(){
    int port = 8080;
    std::string address  = "239.255.0.1";
    ListenerClient client(address, port);
    client.startClient();
    std::cout<<"end";
}