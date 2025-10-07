#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <atomic>

#include "Mouse.hpp"
#include "MouseTracker.hpp"
#include "../config.hpp"



// Global Hook Data -> this is hook to record mouse scroll
HHOOK hHook = NULL;

std::atomic<int> scrollDelta(0);
//Windows Mouse Proc 
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_MOUSEWHEEL)
    {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;

        int delta = GET_WHEEL_DELTA_WPARAM(pMouse->mouseData);

        scrollDelta += delta;
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}


void MessagePump(){
    // ini ada bug, HInstance, hHook , etc harus dalam 1 fungsi kalau gk ngelag gk tau kenapa
    MSG msg;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void startHook(){
    std::thread eventPump(MessagePump);
    eventPump.detach();
}

// Poll the mouse and push to capture only on change
// Polling method used instead event hook in order to limiting the rate of event recorded
void PollMouseWindows(MouseCapture& cap) {
    //start hook untuk event mouse yang butuh hook
    startHook();

    POINT prevPos = { -1, -1 };
    bool prevLeft = false, prevRight = false, prevMiddle = false;

    while (true) {
        POINT pos;
        GetCursorPos(&pos);

        bool leftNow   = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        bool rightNow  = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        bool middleNow = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

        // Wheel tracking
        short wheelNow = HIWORD(GetAsyncKeyState(VK_MBUTTON)); // or track WM_MOUSEWHEEL in a real window
  
        int x = pos.x;
        int y = pos.y;
        int dz = scrollDelta.exchange(0);

        if (x != prevPos.x || y != prevPos.y|| leftNow != prevLeft || rightNow != prevRight
            || middleNow != prevMiddle || dz != 0) {

            cap.push(x, y, dz, leftNow, rightNow, middleNow);
            std::cout<<"scroll: "<<dz<<std::endl;

            prevPos = pos;
            prevLeft = leftNow;
            prevRight = rightNow;
            prevMiddle = middleNow;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(MOUSE_POLL_RATE));
    }
}

void WinApplyMouseState(const MouseState& state, MouseState& prevState) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    // --- Move mouse relative ---
    if (state.x != 0 || state.y != 0) {
        input.mi = {};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

        input.mi.dx = (state.x * 65535) / GetSystemMetrics(SM_CXSCREEN);
        input.mi.dy = (state.y * 65535) / GetSystemMetrics(SM_CYSCREEN);

        SendInput(1, &input, sizeof(INPUT));
    }

    // --- Scroll wheel ---
    if (state.dScroll != 0) {
        input.mi = {};
        input.type = INPUT_MOUSE;
        std::cout<<"Scroll: "<<state.dScroll<<std::endl;
        input.mi.mouseData = state.dScroll; // 1 = 120 units
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
        if (state.midClick == 1){
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            SendInput(1, &input, sizeof(INPUT));
        }else{
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        prevState.midClick = state.midClick;
    }
}
