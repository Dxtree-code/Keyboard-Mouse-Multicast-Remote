#ifdef __APPLE__
#include "mmki/mouse/mouse_mac.hpp"
#include "mmki/config.hpp"
#include <chrono>
#include <thread>

MouseTrackerMac::MouseTrackerMac(shared_ptr<MouseCapture> capturer) : MouseTracker(capturer), scrollDelta(0) {
}

MouseTrackerMac::~MouseTrackerMac() {
    stop();
}

CGEventRef MouseTrackerMac::scrollCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
    if (type == kCGEventScrollWheel) {
        MouseTrackerMac* tracker = static_cast<MouseTrackerMac*>(refcon);
        int64_t delta = CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1);
        tracker->scrollDelta.fetch_add(static_cast<int>(delta));
    }
    return event;
}

void MouseTrackerMac::startHook() {
    // Create event tap for scroll wheel detection (similar to Windows hook)
    CGEventMask mask = CGEventMaskBit(kCGEventScrollWheel);
    eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        mask,
        scrollCallback,
        this
    );

    if (eventTap) {
        runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
        CGEventTapEnable(eventTap, true);
    }
}

void MouseTrackerMac::pollMouse() {
    // Ensure isRunning == true (same as Windows)
    this->setIsRunning(true);
    
    startHook();
    
    CGPoint prevPos = { -1, -1 };
    bool prevLeft = false, prevRight = false, prevMiddle = false;

    while (this->getIsRunning()) {
        // Get current cursor position
        CGEventRef event = CGEventCreate(nullptr);
        CGPoint pos = CGEventGetLocation(event);
        CFRelease(event);

        // Detect button state (same pattern as Windows)
        bool leftNow = CGEventSourceButtonState(kCGEventSourceStateCombinedSessionState, kCGMouseButtonLeft);
        bool rightNow = CGEventSourceButtonState(kCGEventSourceStateCombinedSessionState, kCGMouseButtonRight);
        bool middleNow = CGEventSourceButtonState(kCGEventSourceStateCombinedSessionState, kCGMouseButtonCenter);

        int x = (int)pos.x;
        int y = (int)pos.y;
        int dz = scrollDelta.exchange(0); // Same as Windows: scrollDelta.exchange(0)

        // Same condition check as Windows
        if (x != (int)prevPos.x || y != (int)prevPos.y || 
            leftNow != prevLeft || rightNow != prevRight || middleNow != prevMiddle || dz != 0) {
            
            this->capturer->push(x, y, dz, leftNow, rightNow, middleNow);

            prevPos = pos;
            prevLeft = leftNow;
            prevRight = rightNow;
            prevMiddle = middleNow;
        }

        // Use same polling rate as Windows
        std::this_thread::sleep_for(std::chrono::milliseconds(MOUSE_POLL_RATE));
    }
}

void MouseTrackerMac::stop() {
    this->setIsRunning(false);
    
    // Clean up event tap (similar to Windows hook cleanup)
    if (eventTap) {
        CGEventTapEnable(eventTap, false);
        CFRelease(eventTap);
        eventTap = nullptr;
    }
    
    if (runLoopSource) {
        CFRunLoopRemoveSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
        CFRelease(runLoopSource);
        runLoopSource = nullptr;
    }
}

void MouseExecutorMac::executeMouse(MouseState& state) {
    // --- Move mouse absolute --- (same pattern as Windows)
    if (state.x != prevMouseState.x || state.y != prevMouseState.y) {
        CGEventRef move = CGEventCreateMouseEvent(
            nullptr, kCGEventMouseMoved,
            CGPointMake(state.x, state.y), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, move);
        CFRelease(move);
        
        prevMouseState.x = state.x;
        prevMouseState.y = state.y;
    }

    // --- Scroll wheel --- (same pattern as Windows)
    if (state.dScroll != 0) {
        CGEventRef scroll = CGEventCreateScrollWheelEvent(
            nullptr, kCGScrollEventUnitLine, 1, state.dScroll);
        CGEventPost(kCGHIDEventTap, scroll);
        CFRelease(scroll);
    }

    // --- Left click --- (same pattern as Windows)
    if (state.leftClick != prevMouseState.leftClick) {
        if (state.leftClick == 1) {
            CGEventRef click = CGEventCreateMouseEvent(
                nullptr, kCGEventLeftMouseDown, CGPointMake(state.x, state.y), kCGMouseButtonLeft);
            CGEventPost(kCGHIDEventTap, click);
            CFRelease(click);
        } else {
            CGEventRef click = CGEventCreateMouseEvent(
                nullptr, kCGEventLeftMouseUp, CGPointMake(state.x, state.y), kCGMouseButtonLeft);
            CGEventPost(kCGHIDEventTap, click);
            CFRelease(click);
        }
        prevMouseState.leftClick = state.leftClick;
    }

    // --- Right click --- (same pattern as Windows)
    if (state.rightClick != prevMouseState.rightClick) {
        if (state.rightClick == 1) {
            CGEventRef click = CGEventCreateMouseEvent(
                nullptr, kCGEventRightMouseDown, CGPointMake(state.x, state.y), kCGMouseButtonRight);
            CGEventPost(kCGHIDEventTap, click);
            CFRelease(click);
        } else {
            CGEventRef click = CGEventCreateMouseEvent(
                nullptr, kCGEventRightMouseUp, CGPointMake(state.x, state.y), kCGMouseButtonRight);
            CGEventPost(kCGHIDEventTap, click);
            CFRelease(click);
        }
        prevMouseState.rightClick = state.rightClick;
    }

    // --- Middle click --- (same pattern as Windows)
    if (state.midClick != prevMouseState.midClick) {
        if (state.midClick == 1) {
            CGEventRef click = CGEventCreateMouseEvent(
                nullptr, kCGEventOtherMouseDown, CGPointMake(state.x, state.y), kCGMouseButtonCenter);
            CGEventPost(kCGHIDEventTap, click);
            CFRelease(click);
        } else {
            CGEventRef click = CGEventCreateMouseEvent(
                nullptr, kCGEventOtherMouseUp, CGPointMake(state.x, state.y), kCGMouseButtonCenter);
            CGEventPost(kCGHIDEventTap, click);
            CFRelease(click);
        }
        prevMouseState.midClick = state.midClick;
    }
}

#endif