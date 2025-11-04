#include "mmki/mouse/mouse.hpp"

#ifdef _WIN32
#include "mmki/mouse/mouse_windows.hpp"
#elif __APPLE__
#include "mmki/mouse/mouse_mac.hpp"
#endif


MouseTracker::MouseTracker(shared_ptr<MouseCapture> capturer):
    isRunning(true),
    capturer(capturer)
{

}
MouseTracker::~MouseTracker() = default;

void MouseTracker::setIsRunning(bool value){
    this->isRunning = value;
}

bool MouseTracker::getIsRunning(){
    return this->isRunning.load(std::memory_order_acquire);
}
