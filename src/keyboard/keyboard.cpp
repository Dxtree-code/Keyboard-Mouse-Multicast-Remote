#include "mmki/keyboard/keyboard.hpp"

KeyboardTracker::KeyboardTracker(shared_ptr<KeyboardCapture> capturer)
    :isRunning(true),
    capturer(capturer)
{

}

void KeyboardTracker::setIsRunning(bool value){
    this->isRunning.store(value, std::memory_order_release);
}

bool KeyboardTracker::getIsRunning(){

    return this->isRunning.load(std::memory_order_acquire);
}