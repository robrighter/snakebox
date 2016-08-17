#include "Arduino.h"
#include "joystick.h"

int buttonPins[NUMBER_OF_PINS] = {8,9,10,11,12};
extern byte lastDirection = JOYSTICK_DIRECTION_NONE;

void setupJoystickPins(){
  for(byte i=0; i<NUMBER_OF_PINS; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

byte getJoystickDirection(){
  for(byte i=0; i<NUMBER_OF_PINS;i++){
    if(digitalRead(buttonPins[i]) == LOW){
      return i;
    }
  }
  return JOYSTICK_DIRECTION_NONE;
}

