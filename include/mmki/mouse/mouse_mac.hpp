#ifdef __APPLE__
#pragma once

#include <CoreGraphics/CoreGraphics.h>
#include <memory>
#include <atomic>

#include "mmki/mouse/mouse.hpp"
#include "mmki/mouse/mouse_capture.hpp"

using std::shared_ptr;

class MouseTrackerMac : public MouseTracker {
private:
    std::atomic<int> scrollDelta{0};
    CFMachPortRef eventTap = nullptr;
    CFRunLoopSourceRef runLoopSource = nullptr;
    
    static CGEventRef scrollCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon);
    void startHook();
    
public:
    MouseTrackerMac(shared_ptr<MouseCapture> capturer);
    ~MouseTrackerMac();
    
    void pollMouse() override;
    void stop() override;
};

class MouseExecutorMac : public MouseExecutor {
public:
    void executeMouse(MouseState& state) override;
};

#endif