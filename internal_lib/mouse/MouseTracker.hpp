#pragma once
#include "Mouse.hpp"
#include "windows.h"

//This a function to poll windows mouse position and click state
void PollMouseWindows(MouseCapture& cap);

extern HHOOK hHook;
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
void MessagePump();

void startHook();

void WinApplyMouseState(const MouseState& state, MouseState &prevMouseState);