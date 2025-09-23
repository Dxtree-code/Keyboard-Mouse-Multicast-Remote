#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <CoreGraphics/CoreGraphics.h>

#include "Mouse.hpp"
#include "MouseTracker.hpp"

#define SLEEP_DURATION 20 // ms

// Global state for scroll wheel deltas
std::atomic<int> scrollDelta(0);

// ---------------------------------------------------------
// Poll mouse and push to capture only on change
// ---------------------------------------------------------
void PollMouseMac(MouseCapture& cap) {
    CGPoint prevPos = { -1, -1 };
    bool prevLeft = false, prevRight = false, prevMiddle = false;

    while (true) {
        // Get current cursor position
        CGEventRef event = CGEventCreate(nullptr);
        CGPoint pos = CGEventGetLocation(event);
        CFRelease(event);

        // Detect button state (using event source)
        bool leftNow   = CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState,
                                               kVK_Command); // approximate
        bool rightNow  = CGEventSourceButtonState(kCGEventSourceStateCombinedSessionState,
                                                  kCGMouseButtonRight);
        bool middleNow = CGEventSourceButtonState(kCGEventSourceStateCombinedSessionState,
                                                  kCGMouseButtonCenter);

        int dx = (int)pos.x;
        int dy = (int)pos.y;
        int dz = scrollDelta.exchange(0); // reset scroll accumulator

        if (dx != (int)prevPos.x || dy != (int)prevPos.y ||
            leftNow != prevLeft || rightNow != prevRight ||
            middleNow != prevMiddle || dz != 0) {
            
            cap.push(dx, dy, dz, leftNow, rightNow, middleNow);

            prevPos = pos;
            prevLeft = leftNow;
            prevRight = rightNow;
            prevMiddle = middleNow;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
    }
}

// ---------------------------------------------------------
// Apply mouse state (simulate input)
// ---------------------------------------------------------
void MacApplyMouseState(const MouseState& state, MouseState& prevState) {
    // --- Move mouse ---
    if (state.dx != 0 || state.dy != 0) {
        CGEventRef move = CGEventCreateMouseEvent(
            nullptr, kCGEventMouseMoved,
            CGPointMake(state.dx, state.dy), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, move);
        CFRelease(move);
    }

    // --- Scroll wheel ---
    if (state.dScroll != 0) {
        CGEventRef scroll = CGEventCreateScrollWheelEvent(
            nullptr, kCGScrollEventUnitLine, 1, state.dScroll);
        CGEventPost(kCGHIDEventTap, scroll);
        CFRelease(scroll);
    }

    // --- Left click ---
    if (state.leftClick != prevState.leftClick) {
        CGEventType type = state.leftClick ? kCGEventLeftMouseDown : kCGEventLeftMouseUp;
        CGEventRef click = CGEventCreateMouseEvent(
            nullptr, type, CGPointZero, kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, click);
        CFRelease(click);
        prevState.leftClick = state.leftClick;
    }

    // --- Right click ---
    if (state.rightClick != prevState.rightClick) {
        CGEventType type = state.rightClick ? kCGEventRightMouseDown : kCGEventRightMouseUp;
        CGEventRef click = CGEventCreateMouseEvent(
            nullptr, type, CGPointZero, kCGMouseButtonRight);
        CGEventPost(kCGHIDEventTap, click);
        CFRelease(click);
        prevState.rightClick = state.rightClick;
    }

    // --- Middle click ---
    if (state.midClick != prevState.midClick) {
        CGEventType type = state.midClick ? kCGEventOtherMouseDown : kCGEventOtherMouseUp;
        CGEventRef click = CGEventCreateMouseEvent(
            nullptr, type, CGPointZero, kCGMouseButtonCenter);
        CGEventPost(kCGHIDEventTap, click);
        CFRelease(click);
        prevState.midClick = state.midClick;
    }
}
