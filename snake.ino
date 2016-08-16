#include <Wire.h>
#include "Arduino.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <stdlib.h>

const byte JOYSTICK_DIRECTION_NONE = 5;
const byte JOYSTICK_DIRECTION_UP = 2;
const byte JOYSTICK_DIRECTION_DOWN = 4;
const byte JOYSTICK_DIRECTION_LEFT = 1;
const byte JOYSTICK_DIRECTION_RIGHT = 3;
const byte JOYSTICK_DIRECTION_CENTER = 0;

const byte NUMBER_OF_PINS = 5;
const int buttonPins[NUMBER_OF_PINS] = {8,9,10,11,12};
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

//application variables
byte pointerPosition[2] = {0,0};
byte lastDirection = 0;
boolean screenMatrix[8][16] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

void setup() {
  Serial.begin(9600);
  for(byte i=0; i<NUMBER_OF_PINS; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(13,OUTPUT);
  matrix.begin(0x70);  // pass in the address

}

byte getJoystickDirection(){
  for(byte i=0; i<NUMBER_OF_PINS;i++){
    if(digitalRead(buttonPins[i]) == LOW){
      return i;
    }
  }
  return JOYSTICK_DIRECTION_NONE;
}

void drawPixel(byte x,byte y){
  matrix.drawPixel(x,y, LED_ON);
}

void movePointerByJoystickDirection(byte direction){
  if(direction == JOYSTICK_DIRECTION_UP){
     pointerPosition[1]--;   
  }
  if(direction == JOYSTICK_DIRECTION_DOWN){
    pointerPosition[1]++;  
  }
  if(direction == JOYSTICK_DIRECTION_LEFT){
    pointerPosition[0]--;  
  }
  if(direction == JOYSTICK_DIRECTION_RIGHT){
    pointerPosition[0]++;  
  }
}

void drawMatrix(){
  for(int x=0;x<8;x++){
    for(int y=0;y<16;y++){
      if(screenMatrix[x][y] == true){
        drawPixel(x,y);  
      }
    }
  }
}

void loop() {
  matrix.clear();
  byte direction = getJoystickDirection();
  if(direction != lastDirection){
    if(direction == JOYSTICK_DIRECTION_UP){
      Serial.print("Up\n");  
    }
    if(direction == JOYSTICK_DIRECTION_DOWN){
      Serial.print("Down\n");  
    }
    if(direction == JOYSTICK_DIRECTION_LEFT){
      Serial.print("Left\n");  
    }
    if(direction == JOYSTICK_DIRECTION_RIGHT){
      Serial.print("Right\n");  
    }
    if(direction == JOYSTICK_DIRECTION_CENTER){
      Serial.print("Center\n");
      screenMatrix[pointerPosition[0]][pointerPosition[1]] = true;  
    }
    lastDirection = direction;
    movePointerByJoystickDirection( direction);  
  }
  drawMatrix();
  drawPixel(pointerPosition[0],pointerPosition[1]);
  matrix.writeDisplay();
  delay(30);
}
