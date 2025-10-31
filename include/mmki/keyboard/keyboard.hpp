#pragma once 

#include <atomic>
#include <string>
#include <memory>
#include "mmki/keyboard/keyboard_capture.hpp"

using std::shared_ptr;
using std::atomic;

class KeyboardTracker{
    
    atomic<bool> isRunning;

    protected:
    KeyboardTracker(shared_ptr<KeyboardCapture> capturer);

    shared_ptr<KeyboardCapture> capturer;
    
    public:
    virtual void pollKeyboard() = 0;

    virtual ~KeyboardTracker();

    void setIsRunning(bool value);

    bool getIsRunning();
    
    virtual void stop();  
};

class KeyboardExecutor{
    public:
    virtual void executeKeyboard(KeyboardState &state);
};