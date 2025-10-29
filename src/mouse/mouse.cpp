#include "mmki/mouse/mouse.hpp"

#ifdef _WIN32
#include "mmki/mouse/mouse_windows.hpp"
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
    #else
        throw std::runtime_error(
            std::string("Unsupported platform: no valid \"MouseTracker\" implemented for this OS: ") +
            __FILE__ + ":" + std::to_string(__LINE__) +
            " in function " + __func__
        );
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
        return instance;
    #elif __APPLE__

    #endif
    return instance;
}