#include <Windows.h>
#include <iostream>

void LeftClick(int x, int y)
{
    // Move cursor to the specified position
    SetCursorPos(x, y);

    // Prepare mouse down input
    INPUT inputDown = {0};
    inputDown.type = INPUT_MOUSE;
    inputDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // Prepare mouse up input
    INPUT inputUp = {0};
    inputUp.type = INPUT_MOUSE;
    inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;

    // Send inputs
    SendInput(1, &inputDown, sizeof(INPUT));
    SendInput(1, &inputUp, sizeof(INPUT));
}

int main()
{
    Sleep(3000); // Wait 2 seconds to switch to the target window
    LeftClick(500, 500); // Click at screen coordinates (500, 500)
    return 0;
}
