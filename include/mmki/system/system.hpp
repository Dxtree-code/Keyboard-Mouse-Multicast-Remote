#pragma once
#include "../config.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

enum SystemCommand{
    STOP = 0x8000,
    INVALID = 0x0000
};


struct DisplaySetting{
    int relativeDpi; // scale
    #ifdef _WIN32
    DEVMODE originalDm;
    #endif

    bool dpiChanged =  false;
    bool resChanged =  false;
    
    void changeDisplaySetting(int height, int width,  int relativeDpi);
    void revertDisplaySetting();
    void changeDisplayScale(int  relativeDpi);
};