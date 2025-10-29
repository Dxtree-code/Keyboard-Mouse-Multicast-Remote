#pragma once
#include <windows.h>
#include "mmki/keyboard/keyboard.hpp"

class KeyboardTrackerWindows:public KeyboardTracker{
    HHOOK hhook;
    static KeyboardTrackerWindows instance; 

    private: 
    LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    
    public:    
    void pollKeyboard(KeyboardCapture &capture) override;

    void stop() override;

};


void WinApplyKeyInput(bool isKeyDown, int vkCode);

