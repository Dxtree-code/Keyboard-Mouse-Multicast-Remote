#pragma once
#include "mmki/mouse/mouse.hpp"
#include "mmki/mouse/mouse_windows.hpp"

class MouseFactory{
    

    public:
    static MouseExecutor& getMouseExecutorByOs();
    static MouseTracker& getMouseTrackerByOs(shared_ptr<MouseCapture> capturer);
};