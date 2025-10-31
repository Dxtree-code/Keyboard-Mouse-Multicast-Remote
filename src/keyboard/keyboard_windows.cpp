#include "mmki/keyboard/keyboard_windows.hpp"

bool KeyboardTrackerWindows::setProcPtr(Proc &proc){
    if (this->procPtr == nullptr){
        this->procPtr = std::make_unique<Proc>(proc);
        return true;
    }
    return false;
    
}

KeyboardTrackerWindows::KeyboardTrackerWindows(shared_ptr<KeyboardCapture> capturer):
    KeyboardTracker(capturer)
{
    proc = [this](int nCode, WPARAM wParam, LPARAM lParam)->LRESULT {
        if (nCode == HC_ACTION)
        {
            KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;

            switch (wParam)
            {
            // Empty case without break are intended to record borth WM_KEYDOWN and WM_SYSKEYDOWN
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                this->capturer->push(1, p->vkCode);
                break;

            // Empty case without break are intended to record borth WM_KEYUP and WM_SYSKEYUP
            case WM_KEYUP:
            case WM_SYSKEYUP:
                this->capturer->push(0, p->vkCode);
                break;
            }
        }
        return CallNextHookEx(this->hhook, nCode, wParam, lParam);
    };

    this->setProcPtr(proc);
}

// This is windows hook used to record keyboard input, if there's keyboard input, push to KeyboardCapture
// Full documentation of this hook: https://learn.microsoft.com/en-us/windows/win32/winmsg/about-hooks#wh_keyboard_ll
// Or Just google it you should find better docs
LRESULT CALLBACK KeyboardTrackerWindows::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (KeyboardTrackerWindows::procPtr != nullptr){
        return (*KeyboardTrackerWindows::procPtr.get())(nCode, wParam, lParam);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Wrapper function to start message pump
// This should called on new thread
void KeyboardTrackerWindows::pollKeyboard()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);

    this->hhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardTrackerWindows::KeyboardProc, hInstance, 0);
    if (!this->hhook)
    {
        // std::cerr << "Failed to install hook!" << std::endl;
        return;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(this->hhook);
    return;
}

// This for winodws Client. To simulate requeted keyboard input on client machine
// Full Docs: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput
void KeyboardExecutorWindows::executeKeyboard(KeyboardState &state)
{
    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = static_cast<WORD>(state.code);
    input.ki.wScan = 0;
    input.ki.dwFlags = state.press ? 0 : KEYEVENTF_KEYUP;
    input.ki.time = 0;
    input.ki.dwExtraInfo = GetMessageExtraInfo();

    SendInput(1, &input, sizeof(INPUT));
}
