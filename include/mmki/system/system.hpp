#pragma once
#include "../config.hpp"

enum SystemCommand{
    STOP = 0x8000,
    INVALID = 0x0000
};

#ifdef _WIN32
#include <windows.h>

struct DisplaySetting{
    int relativeDpi; // scale
    DEVMODE originalDm;

    bool dpiChanged =  false;
    bool resChanged =  false;
    
    void changeDisplaySetting(int height, int width,  int relativeDpi);
    void revertDisplaySetting();
    void changeDisplayScale(int  relativeDpi);
};

#elif __APPLE__
// macOS display setting structure
struct DisplaySetting{
    int relativeDpi; // scale
    void* originalDm; // placeholder for macOS display mode

    bool dpiChanged =  false;
    bool resChanged =  false;
    
    void changeDisplaySetting(int height, int width,  int relativeDpi);
    void revertDisplaySetting();
    void changeDisplayScale(int  relativeDpi);
};

#else
// Generic/Linux display setting structure  
struct DisplaySetting{
    int relativeDpi; // scale
    void* originalDm; // placeholder for display mode

    bool dpiChanged =  false;
    bool resChanged =  false;
    
    void changeDisplaySetting(int height, int width,  int relativeDpi);
    void revertDisplaySetting();
    void changeDisplayScale(int  relativeDpi);
};

#endif