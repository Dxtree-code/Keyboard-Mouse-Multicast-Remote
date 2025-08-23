#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include "mouse.h"
#include "mouseTracker.h"

#define SLEEP_DURATION 20 // ms

// Poll the mouse and push to capture only on change
void PollMouseWindows(MouseCapture& cap) {
    POINT prevPos = { -1, -1 };
    bool prevLeft = false, prevRight = false, prevMiddle = false;
    short prevWheel = 0;

    while (true) {
        POINT pos;
        GetCursorPos(&pos);

        bool leftNow   = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        bool rightNow  = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        bool middleNow = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

        // Wheel tracking
        short wheelNow = HIWORD(GetAsyncKeyState(VK_MBUTTON)); // or track WM_MOUSEWHEEL in a real window
        int dScroll = wheelNow - prevWheel;

        int dx = pos.x - prevPos.x;
        int dy = pos.y - prevPos.y;

        // Only push if there is a change
        // if (dx != 0 || dy != 0 || leftNow != prevLeft || rightNow != prevRight
        //     || middleNow != prevMiddle || dScroll != 0) {

        cap.push(dx, dy, dScroll, leftNow, rightNow, middleNow);

        prevPos = pos;
        prevLeft = leftNow;
        prevRight = rightNow;
        prevMiddle = middleNow;
        prevWheel = wheelNow;
        
        std::cout<<"dx: "<<dx<<" dy: "<<dy<<std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION));
    }
}


void WinApplyMouseState(const MouseState& state, MouseState& prevState) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    // --- Move mouse relative ---
    if (state.dx != 0 || state.dy != 0) {
        input.mi.dx = state.dx;
        input.mi.dy = state.dy;
        input.mi.dwFlags = MOUSEEVENTF_MOVE;
        SendInput(1, &input, sizeof(INPUT));
    }

    // --- Scroll wheel ---
    if (state.dScroll != 0) {
        input.mi = {};
        input.type = INPUT_MOUSE;
        input.mi.mouseData = state.dScroll * WHEEL_DELTA; // 1 = 120 units
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        SendInput(1, &input, sizeof(INPUT));
    }

    // --- Left click ---
    if (state.leftClick != prevState.leftClick) {
        // Down
        if (state.leftClick ==1){
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(INPUT));
        }
        else{
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        prevState.leftClick = state.leftClick;
    }


    // --- Right click ---
    if (state.rightClick != prevState.rightClick) {
        if(state.rightClick == 1){
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            SendInput(1, &input, sizeof(INPUT));
        }else{
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        prevState.rightClick = state.rightClick;
    }

    // --- Middle click ---
    if (state.midClick != prevState.midClick) {
        input.mi = {};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
        SendInput(1, &input, sizeof(INPUT));

        input.mi = {};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
        SendInput(1, &input, sizeof(INPUT));
    }
}
