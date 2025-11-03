#pragma once

#include "mmki/keyboard/keyboard.hpp"
#include "mmki/keyboard/keyboard_windows.hpp"
#ifdef __APPLE__
#include "mmki/keyboard/keyboard_mac.hpp"
#endif

class KeyboardFactory{

    public:
    static KeyboardTracker& getKeyboardTrackerByOs(shared_ptr<KeyboardCapture> capturer);
    static KeyboardExecutor& getKeyboardExecutorByOs();
};