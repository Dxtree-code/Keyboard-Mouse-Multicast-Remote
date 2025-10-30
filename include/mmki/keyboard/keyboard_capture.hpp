#pragma once

#include <memory>
#include "mmki/keyboard/keyboard_state.hpp"
#include "mmki/config.hpp"

// an implementation of cilcular queue; -> used in KeyboardCapture
class KeyboardStateQueue
{
private:
    KeyboardState keyboardStateArr[KEYBOARD_STATE_BUFFER] = {};
    int head = 0;
    int tail = 0;
    int count = 0;

public:
    bool push(bool press, int code);

    bool pop(KeyboardState &outState);

    inline bool isEmpty() const { return count == 0; };
    inline bool isFull() const { return count == KEYBOARD_STATE_BUFFER; };
};

// This is an Object used for communication between HOOK or Information Capture with Thread that has Responsibility to send data;
// This Should ONLY USED IN SPSC (Single Producer Single Consumer) MODEL.
// Some data might NOT BUFFERED if The BUFFER FULL, then check the config and increase the size
class KeyboardCapture
{
private:
    KeyboardStateQueue queue;

public:
    inline bool push(bool press, int code)
    {
        return queue.push(press, code);
    }

    inline bool poll(KeyboardState &outState)
    {
        return queue.pop(outState);
    }
};
