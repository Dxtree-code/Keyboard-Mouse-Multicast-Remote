#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    void startSiomayServerC();
    void sendStopCommandC(int ip[4]);
    void startClientC();
#ifdef __cplusplus
}
#endif