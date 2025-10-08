#pragma once
#include "../config.hpp"
#include <windows.h>
enum SystemCommand{
    STOP = 0x8000
};


struct DisplaySetting{
    int relativeDpi; // scale
    DEVMODE originalDm;

    bool dpiChanged =  false;
    bool resChanged =  false;
    
    void changeDisplaySetting(int height, int width,  int relativeDpi);
    void revertDisplaySetting();
    void changeDisplayScale(int  relativeDpi);
};