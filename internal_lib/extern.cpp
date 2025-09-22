#include "extern.hpp"
#include "internal_lib.hpp"

TrackServer *instance = nullptr;
void startSiomayServerC(){
    instance = new TrackServer("239.255.0.1", 8080);
    std::cout<<"Try Starting Track Server"<<std::endl;
    instance->startTrackServer();
    std::cout<<"Track Server ended"<<std::endl;
    return;
}

void sendStopCommandC(int ip[4]){
    if (instance != nullptr){
        instance->sendStopSignal(ip);
    }
}

void  startClientC(){
    startClient("239.255.0.1", 8080);
}
