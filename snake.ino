#include <Wire.h>
#include "Arduino.h"
#include <stdlib.h>
#include "matrixscreen.h"
#include "joystick.h"
#include "gmenu.h"
#include "ganimator.h"
#include "gsnake.h"
#include "gbreakout.h"

void setup() {
  Serial.begin(9600);
  setupJoystickPins();
  pinMode(13,OUTPUT);
  matrix.begin(0x70);
  randomSeed(analogRead(0));
}

void loop() {
  
  switch(currentGameMode){
    case GAME_MODE_MENU:
      gameModeMenuLoop();
      break;
    case GAME_MODE_SNAKE:
      gameModeSnakeLoop();
      break;
    case GAME_MODE_ANIMATOR:
      gameModeAnimatorLoop();
      break;
    case GAME_MODE_BREAKOUT:
      gameModeBreakoutLoop();
      break;
  }
}

