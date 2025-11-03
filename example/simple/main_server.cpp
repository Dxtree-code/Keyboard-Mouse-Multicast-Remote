#include "mmki/track_server.hpp"
int main()
{
    // printf("aaaaaaaaaaaaaa");
    // startSiomayServerC();
    TrackServer server("239.255.0.1", 8080);
    server.startTrackServer();
    server.wait();
    return 0;
}