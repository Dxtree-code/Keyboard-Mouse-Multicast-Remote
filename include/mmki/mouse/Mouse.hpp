#pragma once

#include <memory>
#include <atomic>
#include "../config.hpp"

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


// an implementation of cilcular queue; -> used in MouseCapture
class MouseStateQueue{
    private:
        MouseState mouseStateArr[MOUSE_STATE_BUFFER] = {};
        int head = 0;
        int tail = 0;
        std::atomic<int> count{0};

    
    public:
        MouseStateQueue();
        bool push(int x, int y, int dScroll, bool leftClick, bool rightClick, bool midClick);
        bool pop(MouseState &outState);

        inline bool isEmpty() const { return count == 0; };
        inline bool isFull() const { return count.load(std::memory_order_acquire) == MOUSE_STATE_BUFFER; };
};

// This is an Object used for communication between HOOK or Information Capture with Thread that has Responsibility to send data;
// This Should ONLY USED IN SPSC (Single Producer Single Consumer) MODEL.
// Some data might NOT BUFFERED if The BUFFER FULL, then check the config and increase the size
class MouseCapture{
    private:
        static std::unique_ptr<MouseCapture> instance;
        MouseStateQueue queue;
    public:
        MouseCapture();
        static MouseCapture *GetInstance();

        inline bool push(int x, int y, int dScroll, bool left, bool right, bool mid)
        {
            return queue.push(x, y, dScroll, left, right, mid);
        }

        inline bool poll(MouseState &outState)
        {
            return queue.pop(outState);
        }
};