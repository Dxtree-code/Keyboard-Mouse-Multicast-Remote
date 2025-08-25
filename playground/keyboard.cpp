#include <windows.h>
#include <iostream>

HHOOK hHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        switch (wParam) {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                std::cout << "Key Down: " << p->vkCode << std::endl;
                break;

            case WM_KEYUP:
            case WM_SYSKEYUP:
                std::cout << "Key Up: " << p->vkCode << std::endl;
                break;
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
    if (!hHook) {
        std::cerr << "Failed to install hook!" << std::endl;
        return 1;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    return 0;
}
