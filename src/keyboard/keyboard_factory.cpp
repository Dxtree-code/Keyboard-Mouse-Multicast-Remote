#include "mmki/keyboard/keyboard_factory.hpp"

KeyboardExecutor& KeyboardFactory::getKeyboardExecutorByOs(){
    #ifdef _WIN32
        static KeyboardExecutorWindows executorInstance;
    #elif __APPLE__
        static KeyboardExecutorMac executorInstance;
    #else
        static KeyboardExecutor executorInstance;
    #endif
    
    return executorInstance;
}

KeyboardTracker& KeyboardFactory::getKeyboardTrackerByOs(shared_ptr<KeyboardCapture> capturer){
    #ifdef _WIN32
        static KeyboardTrackerWindows trackerInstance(capturer);
    #elif __APPLE__
        static KeyboardTrackerMac trackerInstance(capturer);
    #else
        static KeyboardTracker trackerInstance;
    #endif

    return trackerInstance;
}