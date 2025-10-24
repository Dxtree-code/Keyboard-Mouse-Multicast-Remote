#include"mmki/mouse/Mouse.hpp"

void MouseState::setState(MouseState & container,int x, int y, int dscroll,
                  bool lClick, bool rClick, bool midClick)
{
    container.x = x;
    container.y = y;
    container.dScroll = dscroll;
    container.leftClick = lClick;
    container.rightClick = rClick;
    container.midClick = midClick;
};