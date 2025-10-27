#include "mmki/mouse/mouse.hpp"


#ifdef _WIN32
#include "mmki/mouse/windows_mouse.hpp"
#elif __APPLE__
#include "MouseTrackerMac.h"
#endif


MouseTracker::MouseTracker():
    isRunning(true)
{

}
MouseTracker::~MouseTracker() = default;

MouseTracker& MouseTracker::getMouseTracker() {
    #ifdef _WIN32
        static MouseTrackerWindows instance;
    #elif __APPLE__
        static MouseTrackerMac instance;
    #else
        static MouseTracker instance; 
    #endif
        return instance;
}

void MouseTracker::setIsRunning(bool value){
    this->isRunning = value;
}

bool MouseTracker::getIsRunning(){
    return this->isRunning.load(std::memory_order_acquire);
}

MouseExecutor& MouseExecutor::getMouseExecutor(){
    #ifdef _WIN32
        static MouseExecutorWindows instance;
    #elif __APPLE__

    #endif
    return instance;
}