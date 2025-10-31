#include "mmki/keyboard/keyboard_factory.hpp"

KeyboardExecutor& KeyboardFactory::getKeyboardExecutorByOs(){
    #ifdef _WIN32
        static KeyboardExecutorWindows executorInstance;
    #else
        static KeyboardTracker executorInstance;
    #endif
    
    return executorInstance;
}

KeyboardTracker& KeyboardFactory::getKeyboardTrackerByOs(shared_ptr<KeyboardCapture> capturer){
    #ifdef _WIN32
        static KeyboardTrackerWindows trackerInstance(capturer);
    #else
        static KeyboardTracker trackerInstance;
    #endif

    return trackerInstance;
}