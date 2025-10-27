#pragma once

#include <thread>
#include <chrono>
#include <atomic>

#include <windows.h>
#include "mmki/mouse/mouse.hpp"
#include "mmki/mouse/mouse_capture.hpp"




class MouseTrackerWindows : public MouseTracker
{
    HHOOK hHook = NULL;
    static inline MouseTrackerWindows *instance = nullptr;
    std::atomic<int> scrollDelta;

    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    void messagePump();
    void startHook();

public:
    static MouseTrackerWindows *getInstance()
    {
        if (instance == nullptr)
        {
            instance = new MouseTrackerWindows();
        }
        return instance;
    }

    void pollMouse(MouseCapture &cap) override;
    void stop() override;

    ~MouseTrackerWindows() override;
};

class MouseExecutorWindows : public MouseExecutor{
    void executeMouse(MouseState &state) override;
};