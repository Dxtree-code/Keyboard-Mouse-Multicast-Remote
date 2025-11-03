#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <CoreGraphics/CoreGraphics.h>

#include "Mouse.hpp"
#include "MouseTracker.hpp"

#define SLEEP_DURATION 10 // ms

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
bool leftNow   = CGEventSourceButtonState(kCGEventSourceStateCombinedSessionState,
                                          kCGMouseButtonLeft);

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
CGEventRef scrollCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
    if (type == kCGEventScrollWheel) {
        int64_t delta = CGEventGetIntegerValueField(event, kCGScrollWheelEventDeltaAxis1);
        std::cout << "Scroll delta: " << delta << std::endl;
    }
    return event;
}
void startHook() {
    CGEventMask mask = CGEventMaskBit(kCGEventScrollWheel);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        mask,
        scrollCallback,
        nullptr
    );

    // Try the privileged HID event tap first. If it fails (commonly due to
    // missing Input Monitoring / Accessibility permission), try a less
    // privileged session-level, listen-only tap as a fallback. The
    // fallback may or may not be sufficient depending on macOS version
    // and privacy settings.
    if (!eventTap) {
        std::cerr << "kCGHIDEventTap creation failed - trying kCGSessionEventTap (listen-only) as fallback" << std::endl;
        eventTap = CGEventTapCreate(
            kCGSessionEventTap,
            kCGHeadInsertEventTap,
            kCGEventTapOptionListenOnly,
            mask,
            scrollCallback,
            nullptr
        );
    }

    if (!eventTap) {
        std::cerr << "Failed to create event tap. This usually means the process lacks permission to monitor input events." << std::endl;
        std::cerr << "On macOS you must grant 'Input Monitoring' or 'Accessibility' permission to this app or to the Terminal/launcher you are using." << std::endl;
        std::cerr << "Open System Settings -> Privacy & Security -> Input Monitoring (or Security & Privacy -> Privacy -> Accessibility on older macOS), add your app or Terminal, then restart the app/Terminal and try again." << std::endl;
        return;
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    CFRunLoopRun(); // acts like MessagePump
}

void MessagePump() {
    // On macOS, startHook already runs a CFRunLoop
    startHook();
}
