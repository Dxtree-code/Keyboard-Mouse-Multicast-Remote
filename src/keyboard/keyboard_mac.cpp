#ifdef __APPLE__
#include "mmki/keyboard/keyboard_mac.hpp"
#include <Carbon/Carbon.h>

bool KeyboardTrackerMac::setProcPtr(Proc& proc) {
    if (this->procPtr == nullptr) {
        this->procPtr = std::make_unique<Proc>(proc);
        return true;
    }
    return false;
}

KeyboardTrackerMac::KeyboardTrackerMac(shared_ptr<KeyboardCapture> capturer) : KeyboardTracker(capturer) {
    // Set up callback proc (similar to Windows)
    proc = [this](CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) -> CGEventRef {
        if (type == kCGEventKeyDown || type == kCGEventKeyUp) {
            CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
            bool press = (type == kCGEventKeyDown);
            
            this->capturer->push(press, static_cast<int>(keyCode));
        }
        return event;
    };
    this->procPtr = std::make_unique<Proc>(proc);
}

KeyboardTrackerMac::~KeyboardTrackerMac() {
    stop();
    if (KeyboardTrackerMac::procPtr.get() == &this->proc) {
        KeyboardTrackerMac::procPtr.reset();
    }
}

CGEventRef KeyboardTrackerMac::KeyboardProc(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
    if (KeyboardTrackerMac::procPtr.get() == nullptr) {
        return event;
    } else {
        return (*KeyboardTrackerMac::procPtr.get())(proxy, type, event, refcon);
    }
}

void KeyboardTrackerMac::pollKeyboard() {
    // Create event tap for keyboard events (similar to Windows hook)
    CGEventMask mask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp);
    eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        mask,
        KeyboardProc,
        nullptr
    );

    if (!eventTap) {
        return;
    }

    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);

    // Message pump equivalent (similar to Windows GetMessage loop)
    CFRunLoopRun();
    
    // Cleanup
    CGEventTapEnable(eventTap, false);
    CFRelease(eventTap);
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CFRelease(runLoopSource);
}

void KeyboardTrackerMac::stop() {
    this->setIsRunning(false);
    
    if (eventTap) {
        CGEventTapEnable(eventTap, false);
        eventTap = nullptr;
    }
    
    if (runLoopSource) {
        runLoopSource = nullptr;
    }
    
    // Stop the run loop (equivalent to PostQuitMessage in Windows)
    CFRunLoopStop(CFRunLoopGetCurrent());
}

void KeyboardExecutorMac::executeKeyboard(KeyboardState& state) {
    // Same pattern as Windows: use the state directly
    CGKeyCode keyCode = static_cast<CGKeyCode>(state.code);
    bool keyDown = state.press;
    
    CGEventRef keyEvent = CGEventCreateKeyboardEvent(nullptr, keyCode, keyDown);
    CGEventPost(kCGHIDEventTap, keyEvent);
    CFRelease(keyEvent);
}

#endif