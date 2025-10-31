#include "mmki/mouse/mouse_factory.hpp"


MouseExecutor& MouseFactory::getMouseExecutorByOs(){
    #ifdef _WIN32
        static MouseExecutorWindows executorInstance;
    #else
        static MouseExecutor executorInstance;
    #endif
    return executorInstance;
}

MouseTracker& MouseFactory::getMouseTrackerByOs(shared_ptr<MouseCapture> capturer){
    #ifdef _WIN32
        static MouseTrackerWindows trackerInstance(capturer);
    #else
        static MouseTracker trackerInstance;
    #endif
    return trackerInstance;
}