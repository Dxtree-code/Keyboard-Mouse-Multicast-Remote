#pragma once

#include "Mouse.hpp"

// ---------------------------------------------------------
// Windows-specific declarations
// ---------------------------------------------------------
#ifdef _WIN32
#include <windows.h>

void PollMouseWindows(MouseCapture& cap);

extern HHOOK hHook;
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
void MessagePump();
void startHook();

void WinApplyMouseState(const MouseState& state, MouseState& prevMouseState);

#endif // _WIN32

// ---------------------------------------------------------
// macOS-specific declarations
// ---------------------------------------------------------
#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>

void PollMouseMac(MouseCapture& cap);
void MacApplyMouseState(const MouseState& state, MouseState& prevMouseState);

#endif // __APPLE__
