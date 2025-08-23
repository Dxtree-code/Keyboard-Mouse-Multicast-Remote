#pragma once
// UNIVERSAL FORMAT FOR THIS PROGRAM
// byte[0]     = Operation Code 
// byte[1]     = Sub Operations Code
// byte[2-3]   = reserved for additional data (unused currently)
// byte[4-7]   = data1 [x]
// byte[8-11]  = data2 [y]
// byte[12-15] = data3 [z]
#include <stdint.h>
#include "mouse.h"
//Operation Codes
#define MOUSE_ACTION 0x01 

//Sub Operation Codes
#define MOUSE_MOVE 0x01
#define MOUSE_LEFT_CLICK 0x02
#define MOUSE_RIGHT_CLICK 0x04
#define MOUSE_MID_CLICK 0x08
#define MOUSE_SCROLL 0x10

//ConvertFunction MouseState To Byte
void clearBuff(uint8_t *buf, int len );

void formatMouseData(MouseState &mState,uint8_t *buf, int len);

void parseMouseData(MouseState &mState, const uint8_t *buf, int len);