#pragma once

// A struct to save keyboard state changes per key
struct KeyboardState{
    bool press; // key state: 0 key up, 1 key down
    int code ; // code of pressed key

    inline static void setKeyboardState(KeyboardState &s, bool press, int code);
};
