//Ini yang buat queue keyboard statenya 100% sama kyk yang mouse nanti di buat template ya VU
// biar gk duplicate codenya. karena yang sekarang hardcode dan duplicate

#pragma once 
#include<memory>

#include "KeyboardWindows.h"
#include "../config.hpp"

// A struct to save keyboard state changes per key
struct KeyboardState{
    bool press; // key state: 0 key up, 1 key down
    int code ; // code of pressed key

    inline static void setKeyboardState(KeyboardState &s, bool press, int code);
};

// an implementation of cilcular queue; -> used in KeyboardCapture
struct KeyboardStateQueue{
    private:
        KeyboardState keyboardStateArr[KEYBOARD_STATE_BUFFER] = {};
        int head =0;
        int tail =0;
        int count =0;
    public:
  
        bool push(bool press, int code);

        bool pop(KeyboardState& outState);

    inline bool isEmpty() const { return count == 0; };
    inline bool isFull() const { return count == KEYBOARD_STATE_BUFFER; };
};

// This is an Object used for communication between HOOK or Information Capture with Thread that has Responsibility to send data;
// This Should ONLY USED IN SPSC (Single Producer Single Consumer) MODEL.
// Some data might NOT BUFFERED if The BUFFER FULL, then check the config and increase the size
class KeyboardCapture {
    private:
        static std::unique_ptr<KeyboardCapture> instance;
        KeyboardStateQueue queue;

    public:
        static KeyboardCapture * GetInstance();
        inline bool push(bool press, int code) {
            return queue.push(press, code);
        }

        inline bool poll(KeyboardState &outState) {
            return queue.pop(outState);
        }
};


