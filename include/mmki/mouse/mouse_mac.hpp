#ifdef  __APPLE__
#pragma once
#include <iostream
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>

#include <CoreGraphics/CoreGraphics.h>

#include "mmki/mouse/mouse.hpp
#include "mmki/mouse/mouse_capture.hpp"
#include "mmki/config.hpp"


class MouseTrackerMac : public MouseTracker{
    std::atomic<int> scrollDelta(0);
    CGEventRef scrollCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon);
    void startHook();
    
    public:
    MouseTrackerMac(shared_ptr<MouseCapture> capturer) 
    void pollMouse() override;

    void stop() override;

    ~MouseTrackerMac();

};

class MouseExecutorMac: public MouseExecutor{
    void executeMouse(MouseState & state) override
};

#endif