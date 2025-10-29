#ifdef _WIN32

#include "mmki/mouse/mouse_windows.hpp"

MouseTrackerWindows::MouseTrackerWindows(){
    proc = [this](int nCode, WPARAM wParam, LPARAM lParam)->LRESULT{
        if (nCode == HC_ACTION && wParam == WM_MOUSEWHEEL)
        {
            MSLLHOOKSTRUCT *pMouse = (MSLLHOOKSTRUCT *)lParam;

            int delta = GET_WHEEL_DELTA_WPARAM(pMouse->mouseData);
            this->scrollDelta += delta;
        }
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    };
    this->procPtr = make_unique<Proc>(proc);
}

MouseTrackerWindows::~MouseTrackerWindows(){
    // Cleanup if this proc is being used.
    if(MouseTrackerWindows::procPtr.get() ==  &this->proc){
        MouseTrackerWindows::procPtr.reset();
    }
}
LRESULT CALLBACK MouseTrackerWindows::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (MouseTrackerWindows::procPtr.get() == nullptr){
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
    else{
        return (*MouseTrackerWindows::procPtr.get())(nCode, wParam, lParam);
    } 
}

void MouseTrackerWindows::messagePump()
{
    // HInstance, hHook, etc. must be in the same function, otherwise it lags (not sure why).
    MSG msg;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    this->hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseTrackerWindows::MouseProc, hInstance, 0);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void MouseTrackerWindows::startHook()
{
    std::thread eventPump(messagePump);
    eventPump.detach();
}

// Poll the mouse and push to capture only on change
// Polling method used instead event hook in order to limiting the rate of event recorded
void MouseTrackerWindows::pollMouse(MouseCapture &cap)
{
    // Ensure the isRunning == true;
    this->setIsRunning(true);

    startHook();

    POINT prevPos = {-1, -1};
    bool prevLeft = false, prevRight = false, prevMiddle = false;

    while (this->getIsRunning())
    {
        POINT pos;
        GetCursorPos(&pos);

        bool leftNow = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        bool rightNow = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        bool middleNow = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

        int x = pos.x;
        int y = pos.y;
        int dz = scrollDelta.exchange(0);

        if (x != prevPos.x || y != prevPos.y || leftNow != prevLeft || rightNow != prevRight || middleNow != prevMiddle || dz != 0)
        {

            cap.push(x, y, dz, leftNow, rightNow, middleNow);

            prevPos = pos;
            prevLeft = leftNow;
            prevRight = rightNow;
            prevMiddle = middleNow;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(MOUSE_POLL_RATE));
    }
}


void MouseTrackerWindows::stop(){
    this->setIsRunning(false);
    if (this->hHook){
        UnhookWindowsHookEx(this->hHook);
        hHook = NULL;
    }
    PostQuitMessage(0);
}

MouseTrackerWindows::~MouseTrackerWindows(){
    this->stop();
}


void MouseExecutor::executeMouse(MouseState &state){
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    // --- Move mouse relative ---
    if (state.x != 0 || state.y != 0)
    {
        input.mi = {};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

        input.mi.dx = (state.x * 65535) / GetSystemMetrics(SM_CXSCREEN);
        input.mi.dy = (state.y * 65535) / GetSystemMetrics(SM_CYSCREEN);

        SendInput(1, &input, sizeof(INPUT));
    }

    // --- Scroll wheel ---
    if (state.dScroll != 0)
    {
        input.mi = {};
        input.type = INPUT_MOUSE;
        input.mi.mouseData = state.dScroll; // 1 = 120 units
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        SendInput(1, &input, sizeof(INPUT));
    }

    // --- Left click ---
    if (state.leftClick != prevMouseState.leftClick)
    {
        // Down
        if (state.leftClick == 1)
        {
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            SendInput(1, &input, sizeof(INPUT));
        }
        else
        {
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        prevMouseState.leftClick = state.leftClick;
    }

    // --- Right click ---
    if (state.rightClick != prevMouseState.rightClick)
    {
        if (state.rightClick == 1)
        {
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            SendInput(1, &input, sizeof(INPUT));
        }
        else
        {
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        prevMouseState.rightClick = state.rightClick;
    }

    // --- Middle click ---
    if (state.midClick != prevMouseState.midClick)
    {
        if (state.midClick == 1)
        {
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            SendInput(1, &input, sizeof(INPUT));
        }
        else
        {
            input.mi = {};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            SendInput(1, &input, sizeof(INPUT));
        }
        prevMouseState.midClick = state.midClick;
    }
}

#endif