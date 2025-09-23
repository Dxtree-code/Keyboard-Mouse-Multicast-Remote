#pragma once

#include "Mouse.hpp"
#include <CoreGraphics/CoreGraphics.h>
void PollMouseMac(MouseCapture& cap);
void MacApplyMouseState(const MouseState& state, MouseState& prevMouseState);
void startHook();     
void MessagePump();   

