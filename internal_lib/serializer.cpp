#include "serializer.hpp"

void clearBuff(uint8_t *buf, int len ){
    for (int i=0; i<len; i++){
        buf[i]= 0x00;
    }
}

void formatMouseData(MouseState &mState, uint8_t *buf, int len) {
    clearBuff(buf, len);
    buf[0] = MOUSE_ACTION;

    if (mState.dx != 0 ||  mState.dy != 0){
        buf[1] |= MOUSE_MOVE;        
        memcpy(buf + 4, &mState.dx, sizeof(mState.dx));   // x
        memcpy(buf + 8, &mState.dy, sizeof(mState.dy));   // y
    }
    if (mState.dScroll !=0 ){
        buf[1] |= MOUSE_SCROLL;
        memcpy(buf + 12, &mState.dScroll, sizeof(mState.dScroll));  // z   
    }
    
    if (mState.leftClick) buf[1] |= MOUSE_LEFT_CLICK;
    if (mState.rightClick) buf[1] |= MOUSE_RIGHT_CLICK;
    if (mState.midClick) buf[1] |= MOUSE_MID_CLICK;

    // Assuming mState has int x, y, z
    
}

void parseMouseData(MouseState &mState, const uint8_t *buf, int len) {
    // Reset all fields first
    mState.dx = 0;
    mState.dy = 0;
    mState.dScroll = 0;
    mState.leftClick = false;
    mState.rightClick = false;
    mState.midClick = false;

    // Operation code (currently unused)
    uint8_t opCode = buf[0];

    // Sub-operation code
    uint8_t subOp = buf[1];

    // Movement
    if ( (subOp & MOUSE_MOVE) == MOUSE_MOVE) {
        memcpy(&mState.dx, buf + 4, sizeof(mState.dx));
        memcpy(&mState.dy, buf + 8, sizeof(mState.dy));
    }

    // Clicks
    if ( (subOp & MOUSE_LEFT_CLICK) == MOUSE_LEFT_CLICK) mState.leftClick = true;
    if ( (subOp & MOUSE_RIGHT_CLICK) == MOUSE_RIGHT_CLICK) mState.rightClick = true;
    if ( (subOp & MOUSE_MID_CLICK) == MOUSE_MID_CLICK) mState.midClick = true;

    // Scroll (if used)
    if ( (subOp & MOUSE_SCROLL) == MOUSE_SCROLL) {
        memcpy(&mState.dScroll, buf + 12, sizeof(mState.dScroll));
    }
}