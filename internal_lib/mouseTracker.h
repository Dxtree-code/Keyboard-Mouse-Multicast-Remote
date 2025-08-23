#pragma once
#include "mouse.h"
void PollMouseWindows(MouseCapture& cap);


void WinApplyMouseState(const MouseState& state, MouseState &prevMouseState);