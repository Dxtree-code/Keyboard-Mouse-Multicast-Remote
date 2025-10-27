#pragma once
#include <atomic>
#include "mmki/mouse/mouse_capture.hpp"

using std::atomic;
class MouseTracker{
    
    atomic<bool> isRunning;

    protected:
    MouseTracker();

    public:
    virtual void pollMouse(MouseCapture &cap);
    
    static MouseTracker& getMouseTracker();// This function should return Child of MouseTracker, windows, or mac or other platform

    virtual ~MouseTracker();

    void setIsRunning(bool value);
    bool getIsRunning();

    virtual void stop();
};

class MouseExecutor{
    public:
    MouseState prevMouseState;

    virtual void  executeMouse(MouseState &state);

    static MouseExecutor& getMouseExecutor();
};