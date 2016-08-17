#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "Arduino.h"

const byte JOYSTICK_DIRECTION_NONE = 5;
const byte JOYSTICK_DIRECTION_UP = 2;
const byte JOYSTICK_DIRECTION_DOWN = 4;
const byte JOYSTICK_DIRECTION_LEFT = 1;
const byte JOYSTICK_DIRECTION_RIGHT = 3;
const byte JOYSTICK_DIRECTION_CENTER = 0;


#define NUMBER_OF_PINS 5

extern byte lastDirection;


void setupJoystickPins();
byte getJoystickDirection();



#endif

