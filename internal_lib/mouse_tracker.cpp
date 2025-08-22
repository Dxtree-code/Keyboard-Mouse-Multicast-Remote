#include "mouse_tracker.h"
#include "mouse.h"
#include <windows.h>
#include <thread>
#include <atomic>

#define SLEEP_DURATION 10 // ms

static std::thread trackerThread;
static std::atomic<bool> running(false);
static MouseCapture* cap = nullptr;

static void TrackerLoop(MouseCapture& cap) {
    POINT prevPos = { -1, -1 };
    bool prevLeft = false, prevRight = false, prevMiddle = false;
    short prevWheel = 0;

    while (running.load()) {
        POINT pos;
        GetCursorPos(&pos);

        bool leftNow   = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        bool rightNow  = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        bool middleNow = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

        short wheelNow = GET_WHEEL_DELTA_WPARAM(GetAsyncKeyState(VK_MBUTTON));
        int dScroll = wheelNow - prevWheel;

        int dx = pos.x - prevPos.x;
        int dy = pos.y - prevPos.y;

        if (dx != 0 || dy != 0 || leftNow != prevLeft ||
            rightNow != prevRight || middleNow != prevMiddle || dScroll != 0) {
            
            cap.push(dx, dy, dScroll, leftNow, rightNow, middleNow);

            prevPos = pos;
            prevLeft = leftNow;
            prevRight = rightNow;
            prevMiddle = middleNow;
            prevWheel = wheelNow;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
    }
}

extern "C" __declspec(dllexport) int StartMouseTracker() {
    if (running.load()) return 0; // already running
    cap = MouseCapture::GetInstance();
    running.store(true);
    trackerThread = std::thread(TrackerLoop, std::ref(*cap));
    return 1;
}

extern "C" __declspec(dllexport) void StopMouseTracker() {
    running.store(false);
    if (trackerThread.joinable()) {
        trackerThread.join();
    }
}

extern "C" __declspec(dllexport) int PollMouseEvent(MouseEvent* outEvent) {
    if (!cap) return 0;
    MouseState st;
    if (cap->poll(st)) {
        outEvent->dx = st.dx;
        outEvent->dy = st.dy;
        outEvent->dScroll = st.dScroll;
        outEvent->leftClick = st.leftClick;
        outEvent->rightClick = st.rightClick;
        outEvent->midClick = st.midClick;
        return 1;
    }
    return 0;
}
