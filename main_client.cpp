#include "internal_lib/client.h"
#include <iostream>
#include <string>
int main(){
    std::string addr; 
    int port;
    std::cout<<"addr: ";
    std::cin>>addr;
    std::cout<<"port: ";
    std::cin>>port;
    
    startClient(addr, port);
}