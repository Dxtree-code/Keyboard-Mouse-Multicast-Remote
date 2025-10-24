#include "src/TrackServer.hpp"
#include "src/extern.hpp"
int main()
{
    // printf("aaaaaaaaaaaaaa");
    // startSiomayServerC();
    TrackServer server("239.255.0.1", 8080);
    server.startTrackServer();
    return 0;
}