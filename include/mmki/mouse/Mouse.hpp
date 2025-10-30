#pragma once

#include <memory>
#include <atomic>

#include "mmki/mouse/mouse_capture.hpp"

using std::atomic;
using std::shared_ptr;

class MouseTracker{
    
    atomic<bool> isRunning;

    protected:
    MouseTracker(shared_ptr<MouseCapture> capturer);

    shared_ptr<MouseCapture> capturer;

    public:
    virtual void pollMouse();
    
    static MouseTracker& getMouseTracker(shared_ptr<MouseCapture> capturer);// This function should return Child of MouseTracker, windows, or mac or other platform

    virtual ~MouseTracker();

    void setIsRunning(bool value);
    bool getIsRunning();

    virtual void stop();
};

class MouseExecutor{
    protected:
    MouseState prevMouseState;

    public:
    virtual void  executeMouse(MouseState &state);

    static MouseExecutor& getMouseExecutor();
};

