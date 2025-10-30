#ifdef _WIN32
#pragma once

#include <windows.h>
#include <functional>
#include <memory>

#include "mmki/keyboard/keyboard.hpp"

using std::unique_ptr;
using std::function;


class KeyboardTrackerWindows:public KeyboardTracker{
    HHOOK hhook;
    typedef  function<LRESULT(int nCode, WPARAM wParam, LPARAM lParam)> Proc;
    Proc proc;
    static unique_ptr<Proc> procPtr;
    bool setProcPtr(Proc &proc);

    
    public:
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    KeyboardTrackerWindows(shared_ptr<KeyboardCapture> capturer);
    ~KeyboardTrackerWindows();
    
    void pollKeyboard() override;

    void stop() override;

};

class KeyboardExecutorWindows: public KeyboardExecutor{
    void executeKeyboard(KeyboardState& state) override;
};

#endif

