#include <Windows.h>
#include <iostream>

void MouseDrag(int startX, int startY, int endX, int endY, int steps = 20)
{
    // Move cursor to the start position
    SetCursorPos(startX, startY);

    // Press left mouse button down
    INPUT inputDown = {0};
    inputDown.type = INPUT_MOUSE;
    inputDown.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &inputDown, sizeof(INPUT));

    // Calculate incremental movement
    double dx = (endX - startX) / (double)steps;
    double dy = (endY - startY) / (double)steps;

    for (int i = 1; i <= steps; i++)
    {
        int x = startX + (int)(dx * i);
        int y = startY + (int)(dy * i);

        INPUT inputMove = {0};
        inputMove.type = INPUT_MOUSE;
        inputMove.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
        inputMove.mi.dx = (x * 65535) / GetSystemMetrics(SM_CXSCREEN); // normalize to 0–65535
        inputMove.mi.dy = (y * 65535) / GetSystemMetrics(SM_CYSCREEN);
        SendInput(1, &inputMove, sizeof(INPUT));

        Sleep(10); // small delay fowr smooth dragging
    }

    // Release left mouse button
    INPUT inputUp = {0};
    inputUp.type = INPUT_MOUSE;
    inputUp.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &inputUp, sizeof(INPUT));
}

int main()
{
    Sleep(5000); // give 2 seconds to switch to target window
    MouseDrag(100, 100, 500, 500); // drag from (100,100) to (500,500)
    return 0;
}
