#pragma once

extern "C" {
    void startSiomayServerC();
    void sendStopCommandC(int ip[4]);
    void startClientC();
}