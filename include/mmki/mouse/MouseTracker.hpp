#pragma once
#include "Mouse.hpp"
#include <windows.h>

void pollMouseWindows(MouseCapture &cap);

extern HHOOK hHook;
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
void messagePump();

void startHook();

void winApplyMouseState(const MouseState &state, MouseState &prevMouseState);