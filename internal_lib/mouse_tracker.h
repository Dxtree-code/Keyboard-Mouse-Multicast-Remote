#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int dx;
    int dy;
    int dScroll;
    int leftClick;
    int rightClick;
    int midClick;
} MouseEvent;

int StartMouseTracker();
void StopMouseTracker();
int PollMouseEvent(MouseEvent* outEvent);

#ifdef __cplusplus
}
#endif
