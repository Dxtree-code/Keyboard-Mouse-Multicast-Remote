#ifdef __APPLE__
#pragma once

#include <CoreGraphics/CoreGraphics.h>
#include <memory>
#include <functional>

#include "mmki/keyboard/keyboard.hpp"
#include "mmki/keyboard/keyboard_capture.hpp"

using std::shared_ptr;
using std::function;

enum class KeyCode {
    Command = 0x37,
    Option = 0x3A,
    Control = 0x3B
};

class KeyboardTrackerMac : public KeyboardTracker {
private:
    CFMachPortRef eventTap = nullptr;
    CFRunLoopSourceRef runLoopSource = nullptr;
    
    typedef function<CGEventRef(CGEventTapProxy, CGEventType, CGEventRef, void*)> Proc;
    Proc proc;
    static inline std::unique_ptr<Proc> procPtr;
    
    static CGEventRef KeyboardProc(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon);
    bool setProcPtr(Proc& proc);
    
public:
    KeyboardTrackerMac(shared_ptr<KeyboardCapture> capturer);
    ~KeyboardTrackerMac();
    
    void pollKeyboard() override;
    void stop() override;
};

class KeyboardExecutorMac : public KeyboardExecutor {
public:
    void executeKeyboard(KeyboardState& state) override;
};

#endif