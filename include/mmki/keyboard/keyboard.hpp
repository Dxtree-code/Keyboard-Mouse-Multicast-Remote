#pragma once 
#include <atomic>
#include <string>
#include "mmki/keyboard/keyboard_capture.hpp"

#ifdef _WIN32
#include "mmki/keyboard/keyboard_windows.hpp"
#endif

using std::atomic;

class KeyboardTracker{
    
    atomic<bool> isRunning;

    protected:
    KeyboardTracker();

    public:
    virtual void pollKeyboard(KeyboardCapture &cap) = 0;
    
    static KeyboardTracker& getKeyboardTracker();// This function should return Child of KeyboardTracker, windows, or mac or other platform

    virtual ~KeyboardTracker();

    void setIsRunning(bool value);

    bool getIsRunning();
    
    virtual void stop();  
};

