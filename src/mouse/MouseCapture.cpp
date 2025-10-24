#include "mmki/mouse/Mouse.hpp"

MouseStateQueue::MouseStateQueue(){
    this->count.store(0);
}

MouseCapture::MouseCapture(){}
// MouseStateQueue and
bool MouseStateQueue::push(int x, int y, int dScroll, bool leftClick, bool rightClick, bool midClick){
    if (count.load(std::memory_order_acquire) == MOUSE_STATE_BUFFER)
    {
        return false; // buffer full
    }

    MouseState::setState(mouseStateArr[tail], x, y, dScroll, leftClick, rightClick, midClick);

    tail = (tail + 1) % MOUSE_STATE_BUFFER;
    count.fetch_add(1);
    return true;
}

bool MouseStateQueue::pop(MouseState &outState)
{
    if (count.load(std::memory_order_acquire) <= 0)
    {
        return false; // buffer full
    }
    outState = mouseStateArr[head];
    head = (head + 1) % MOUSE_STATE_BUFFER;
    count.fetch_sub(1);
    return true;
}

// MouseCapture Impl
std::unique_ptr<MouseCapture> MouseCapture::instance = nullptr;
MouseCapture *MouseCapture::GetInstance()
{
    if (!instance)
    {
        instance = std::make_unique<MouseCapture>();
    }
    return instance.get();
}
