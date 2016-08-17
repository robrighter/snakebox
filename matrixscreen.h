#ifndef MATRIXSCREEN_H
#define MATRIXSCREEN_H

#include "Arduino.h"
#include <stdlib.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"


#define SCREEN_WIDTH 9
#define CHAR_WIDTH 14

extern boolean screenMatrix[8][16];
extern Adafruit_8x16matrix matrix;
extern byte pointerPosition[2];


void drawMatrix();
void drawPixel(byte x,byte y);
void scrollText(String toscroll, boolean endOnJoystickMove);
void movePointerByJoystickDirection(byte direction);

#endif
