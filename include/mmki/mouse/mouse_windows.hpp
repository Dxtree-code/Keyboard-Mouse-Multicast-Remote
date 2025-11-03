#ifdef _WIN32

#pragma once

#include <thread>
#include <chrono>
#include <atomic>

#include <windows.h>
#include <memory>
#include <functional>

#include "mmki/mouse/mouse.hpp"
#include "mmki/mouse/mouse_capture.hpp"

using std::unique_ptr;
using std::make_unique;
using std::function;

class MouseTrackerWindows : public MouseTracker{

    HHOOK hHook = NULL;

    typedef  function<LRESULT(int nCode, WPARAM wParam, LPARAM lParam)> Proc;
    Proc proc;
    inline static unique_ptr<Proc> procPtr;

    std::atomic<int> scrollDelta;

    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    
    void messagePump();
    void startHook();

    public:
    MouseTrackerWindows(shared_ptr<MouseCapture> capturer);
    void pollMouse() override;
    void stop() override;

    ~MouseTrackerWindows() override;
};

class MouseExecutorWindows : public MouseExecutor{
    void executeMouse(MouseState &state) override;
};

#endif