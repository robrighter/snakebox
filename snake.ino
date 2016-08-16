#include <Wire.h>
#include "Arduino.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <stdlib.h>

#define SCREEN_WIDTH 9
#define CHAR_WIDTH 14

const byte JOYSTICK_DIRECTION_NONE = 5;
const byte JOYSTICK_DIRECTION_UP = 2;
const byte JOYSTICK_DIRECTION_DOWN = 4;
const byte JOYSTICK_DIRECTION_LEFT = 1;
const byte JOYSTICK_DIRECTION_RIGHT = 3;
const byte JOYSTICK_DIRECTION_CENTER = 0;

const byte GAME_MODE_MENU = 1;
const byte GAME_MODE_SNAKE = 2;
const byte GAME_MODE_ANIMATOR = 3;
const byte GAME_MODE_BREAKOUT = 4;
const byte GAME_MODE_TETRIS = 5;
const byte LAST_GAME_MODE = GAME_MODE_ANIMATOR;
const byte FIRST_GAME_MODE = GAME_MODE_SNAKE;


const byte NUMBER_OF_PINS = 5;
const int buttonPins[NUMBER_OF_PINS] = {8,9,10,11,12};
Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

//application variables
byte pointerPosition[2] = {0,0};
byte lastDirection = JOYSTICK_DIRECTION_NONE;
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
byte currentGameMode = GAME_MODE_MENU;
byte menuCurrentSelected = GAME_MODE_SNAKE;

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

void scrollText(String toscroll, boolean endOnJoystickMove) {
  matrix.clear();
  int spacing = toscroll.length()*CHAR_WIDTH;
  matrix.setTextSize(2);
  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextColor(LED_ON);
  matrix.setRotation(0);
  for (int x=(SCREEN_WIDTH); x>(0-(spacing)); x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print(toscroll);
    matrix.writeDisplay();
    byte direction = getJoystickDirection();
    if(direction != JOYSTICK_DIRECTION_NONE){
      lastDirection = direction;
      if(endOnJoystickMove){
        return;  
      }
    }
    delay(80);
  }
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

void gameModeAnimatorLoop(){
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

void gameModeSnakeLoop(){
  //not implemented
  scrollText("not implemented", false);
  currentGameMode = GAME_MODE_MENU;
}

void gameModeMenuLoop(){
  if(lastDirection != JOYSTICK_DIRECTION_NONE){
    if((lastDirection == JOYSTICK_DIRECTION_DOWN) && (menuCurrentSelected<LAST_GAME_MODE)){
      menuCurrentSelected++;
    }
    else if((lastDirection == JOYSTICK_DIRECTION_UP) && (menuCurrentSelected>FIRST_GAME_MODE)){
      menuCurrentSelected--;
    }
    else if(lastDirection == JOYSTICK_DIRECTION_CENTER){
      currentGameMode = menuCurrentSelected;
      menuCurrentSelected = FIRST_GAME_MODE;
      lastDirection = JOYSTICK_DIRECTION_NONE;
      delay(100);
      return;  
    }
    lastDirection = JOYSTICK_DIRECTION_NONE;
  }
  if(menuCurrentSelected == GAME_MODE_SNAKE){
    scrollText("Snake", true);
  }
  else if(menuCurrentSelected == GAME_MODE_ANIMATOR){
    scrollText("Animator", true);
  }
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
  }
  

  
}
