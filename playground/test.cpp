#include<windows.h>
#include<atomic>
#include<iostream>
HHOOK hHook = NULL;
// Global Hool Data
std::atomic<int> scrollDelta(0);
//Windows Mouse Proc 
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_MOUSEWHEEL)
    {
        MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;

        // Extract wheel delta from mouseData (HIWORD)
        short delta = GET_WHEEL_DELTA_WPARAM(pMouse->mouseData);

        scrollDelta += delta; // accumulate scroll
        std::cout << "Scroll delta: " << delta << " | Total: " << scrollDelta << std::endl;
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main(){
    HINSTANCE hInstance = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);
    MSG msg;    
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

}