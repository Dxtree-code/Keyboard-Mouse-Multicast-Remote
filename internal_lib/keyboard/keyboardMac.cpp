#include <ApplicationServices/ApplicationServices.h>
#include <iostream>
#include "keyboard.hpp"

// Global event tap reference
static CFMachPortRef keyboardEventTap = nullptr;

// Callback for keyboard events
static CGEventRef KeyboardCallback(CGEventTapProxy proxy,
                                   CGEventType type,
                                   CGEventRef event,
                                   void* userInfo) 
{
    if (type != kCGEventKeyDown && type != kCGEventKeyUp) {
        return event; // ignore other events
    }

    KeyboardCapture* kCapture = KeyboardCapture::GetInstance();

    // Get the keycode (hardware-dependent, not the same as Windows VK codes)
    int keyCode = (int)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (type == kCGEventKeyDown) {
        // std::cout << "Key Down: " << keyCode << std::endl;
        kCapture->push(true, keyCode);
    } else if (type == kCGEventKeyUp) {
        // std::cout << "Key Up: " << keyCode << std::endl;
        kCapture->push(false, keyCode);
    }

    return event; // pass event along
}

void startKeyboardTrack() {
    // Create an event tap for key presses and releases
    keyboardEventTap = CGEventTapCreate(kCGSessionEventTap,
                                        kCGHeadInsertEventTap,
                                        kCGEventTapOptionDefault,
                                        CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp),
                                        KeyboardCallback,
                                        nullptr);

    if (!keyboardEventTap) {
        std::cerr << "Failed to create keyboard event tap!" << std::endl;
        return;
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, keyboardEventTap, 0);

    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(keyboardEventTap, true);

    std::cout << "Keyboard tracking started (macOS)" << std::endl;

    // Run loop — will keep processing events until stopped
    CFRunLoopRun();

    // Cleanup if run loop exits
    CFRelease(runLoopSource);
    CFRelease(keyboardEventTap);
    keyboardEventTap = nullptr;
}

void MacApplyKeyInput(bool isKeyDown, int keyCode) {
    // Create a keyboard event
    CGEventRef event = CGEventCreateKeyboardEvent(nullptr, (CGKeyCode)keyCode, isKeyDown);
    if (!event) {
        std::cerr << "Failed to create CGEvent for key input!" << std::endl;
        return;
    }

    // Post it to the system
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
}
