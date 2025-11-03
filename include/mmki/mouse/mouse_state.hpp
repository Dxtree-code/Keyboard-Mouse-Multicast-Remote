#pragma once

#include <memory>
#include <atomic>
#include "mmki/config.hpp"

// A struct to save mouse position and click action
struct MouseState
{
    int x = 0;
    int y = 0;
    int dScroll = 0;

    bool midClick = false;
    bool rightClick = false;
    bool leftClick = false;

    static void setState(MouseState &container, int x, int y, int dscroll,
                                bool lClick, bool rClick, bool midClick);
};
