#include "extern.hpp"
#include "internal_lib.hpp"

TrackServer *instance = nullptr;
void startSiomayServerC(){
    instance = new TrackServer("224.0.0.1", 8080);
    instance->startTrackServer();
}

void sendStopCommandC(int ip[4]){
    if (instance != nullptr){
        instance->sendStopSignal(ip);
    }
}

void  startClientC(){
    startClient("224.0.0.1", 8080);
}
