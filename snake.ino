#include <Wire.h>
#include "Arduino.h"
#include <stdlib.h>
#include "matrixscreen.h"
#include "joystick.h"

const byte GAME_MODE_MENU = 1;
const byte GAME_MODE_SNAKE = 2;
const byte GAME_MODE_ANIMATOR = 3;
const byte GAME_MODE_BREAKOUT = 4;
const byte GAME_MODE_TETRIS = 5;
const byte LAST_GAME_MODE = GAME_MODE_ANIMATOR;
const byte FIRST_GAME_MODE = GAME_MODE_SNAKE;


//application variables
byte pointerPosition[2] = {0,0};
byte currentGameMode = GAME_MODE_MENU;
byte menuCurrentSelected = GAME_MODE_SNAKE;

void setup() {
  Serial.begin(9600);
  setupJoystickPins();
  pinMode(13,OUTPUT);
  matrix.begin(0x70);  // pass in the address

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
