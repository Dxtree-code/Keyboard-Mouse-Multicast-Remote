#include "mmki/keyboard/keyboard.hpp"

KeyboardTracker::KeyboardTracker(shared_ptr<KeyboardCapture> capturer)
    :isRunning(true),
    capturer(capturer)
{

}

KeyboardTracker& KeyboardTracker::getKeyboardTracker(shared_ptr<KeyboardCapture> capturer){
    #ifdef _WIN32
        static KeyboardTrackerWindows instance(capturer);
    #else
        throw std::runtime_error(
            std::string("Unsupported platform: no valid \"KeyboardTracker\" implemented for this OS: ") +
            __FILE__ + ":" + std::to_string(__LINE__) +
            " in function " + __func__
        );
    #endif
    return instance;
}

void KeyboardTracker::setIsRunning(bool value){
    this->isRunning.store(value, std::memory_order_release);
}

bool KeyboardTracker::getIsRunning(){

    return this->isRunning.load(std::memory_order_acquire);
}