#include "mmki/keyboard/keyboard_windows.hpp"


// This is windows hook used to record keyboard input, if there's keyboard input, push to KeyboardCapture
// Full documentation of this hook: https://learn.microsoft.com/en-us/windows/win32/winmsg/about-hooks#wh_keyboard_ll
// Or Just google it you should find better docs
HHOOK kHHook;
LRESULT CALLBACK KeyboardTrackerWindows::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {

        KeyboardCapture *kCapture = KeyboardCapture::GetInstance();
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;
        
        switch (wParam) {
            // Empty case without break are intended to record borth WM_KEYDOWN and WM_SYSKEYDOWN
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                kCapture->push(1, p->vkCode);
                break;

            // Empty case without break are intended to record borth WM_KEYUP and WM_SYSKEYUP
            case WM_KEYUP:
            case WM_SYSKEYUP:
                kCapture->push(0, p->vkCode);
                break;
        }
    }
    return CallNextHookEx(kHHook, nCode, wParam, lParam);
}

//Wrapper function to start message pump
//This should called on new thread 
void startKeyboardTrack(){
    HINSTANCE hInstance = GetModuleHandle(NULL);

    kHHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);
    if (!kHHook) {
        std::cerr << "Failed to install hook!" << std::endl;
        return;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(kHHook);
    return;
}


// This for winodws Client. To simulate requeted keyboard input on client machine
// Full Docs: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput
void WinApplyKeyInput(bool isKeyDown, int vkCode) {
    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = static_cast<WORD>(vkCode);
    input.ki.wScan = 0;                        
    input.ki.dwFlags = isKeyDown ? 0 : KEYEVENTF_KEYUP;
    input.ki.time = 0;
    input.ki.dwExtraInfo = GetMessageExtraInfo();

    SendInput(1, &input, sizeof(INPUT));
}

