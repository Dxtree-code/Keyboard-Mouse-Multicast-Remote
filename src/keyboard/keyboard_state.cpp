#include "mmki/keyboard/keyboard_state.hpp"

void KeyboardState::setKeyboardState(KeyboardState &s, bool press, int code){
    s.press =press;
    s.code = code;
}
