#pragma once
#include "./network/Multicast.hpp"
#include "./mouse/Mouse.hpp"


int startTrackServer();
void startClient(std::string listenAddr, int port);