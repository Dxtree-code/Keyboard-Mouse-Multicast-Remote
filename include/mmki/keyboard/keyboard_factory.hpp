#pragma once

#include "mmki/keyboard/keyboard.hpp"
#include "mmki/keyboard/keyboard_windows.hpp"

class KeyboardFactory{

    public:
    static KeyboardTracker& getKeyboardTrackerByOs(shared_ptr<KeyboardCapture> capturer);
    static KeyboardExecutor& getKeyboardExecutorByOs();
};