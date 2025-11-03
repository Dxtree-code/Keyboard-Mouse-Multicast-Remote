#include "mmki/mouse/mouse_factory.hpp"


MouseExecutor& MouseFactory::getMouseExecutorByOs(){
    #ifdef _WIN32
        static MouseExecutorWindows executorInstance;
    #elif __APPLE__
        static MouseExecutorMac executorInstance;
    #else
        static MouseExecutor executorInstance;
    #endif
    return executorInstance;
}

MouseTracker& MouseFactory::getMouseTrackerByOs(shared_ptr<MouseCapture> capturer){
    #ifdef _WIN32
        static MouseTrackerWindows trackerInstance(capturer);
    #elif __APPLE__
        static MouseTrackerMac trackerInstance(capturer);
    #else
        static MouseTracker trackerInstance;
    #endif
    return trackerInstance;
}