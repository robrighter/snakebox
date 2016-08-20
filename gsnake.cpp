#include "Arduino.h"
#include "gsnake.h"
#include "matrixscreen.h"
#include "joystick.h"
#include "gmenu.h"



byte countDownDelay = 20;
byte snakeLevel = 1;
byte snakeLength = 2;
int8_t xSnake[128] = {1,0,-1};
int8_t ySnake[128] = {0,0,-1};
byte currentSnakeDirection = JOYSTICK_DIRECTION_RIGHT;
byte applePosition[2] = {2,5};
boolean shouldGrow = false;

void resetTheGame(){
  snakeLevel = 1;
  snakeLength = 2;
  xSnake[0] = 1;
  xSnake[1] = 0;
  xSnake[2] = -1;
  ySnake[0] = 0;
  ySnake[1] = 0;
  ySnake[2] = -1;
  currentSnakeDirection = JOYSTICK_DIRECTION_RIGHT;
  applePosition[0] = 2;
  applePosition[1] = 5;
  shouldGrow = false;
}

boolean moveTheSnake(byte direction, boolean grow){
  //find the tail (-1,-1)
  Serial.print("moveTheSnake: find the tail (-1,-1)\n");
  int8_t offset = 0;
  while(xSnake[offset]>-1){
    offset++;
  }
  int8_t xTail = offset + (grow ? 1 : 0);
  int8_t yTail = offset + (grow ? 1 : 0);
  
  Serial.print("moveTheSnake: count back and move all the array values down\n");
  while(offset>-1){
    xSnake[offset+1] = xSnake[offset]; 
    ySnake[offset+1] = ySnake[offset]; 
    offset--;
  }

  Serial.print("moveTheSnake: make the new tail\n");
  xSnake[xTail] = -1;
  ySnake[yTail] = -1;

  char coordprint[12];
  for(byte i=0;i<20; i++){
    sprintf(coordprint, "(%d,%d), ",xSnake[i], ySnake[i]);
    Serial.print(coordprint);
  }
  Serial.print("\n\n");
  
  Serial.print("moveTheSnake: insert the new coord at the head (0,0)\n");
  if(direction == JOYSTICK_DIRECTION_UP){
      ySnake[0]--;
      if(ySnake[0]<0){
        //hit the end of the screen
        return true; 
      }
  }
  if(direction == JOYSTICK_DIRECTION_DOWN){
      ySnake[0]++;
      if(ySnake[0]>15){
        //hit the end of the screen
        return true; 
      }  
  }
  if(direction == JOYSTICK_DIRECTION_LEFT){
      xSnake[0]--;
      if(xSnake[0]<0){
        //hit the end of the screen
        return true; 
      }  
  }
  if(direction == JOYSTICK_DIRECTION_RIGHT){
    xSnake[0]++;
    if(xSnake[0]>7){
      //hit the end of the screen
      return true; 
    }   
  }
  return false;
}

byte readNewSnakeDirection(){
  Serial.print("readNewSnakeDirection - Listening for new direction\n"); 
  for(byte i=0; i<(countDownDelay - snakeLevel ); i++){
    byte readDirection = getJoystickDirection();
    if((readDirection != JOYSTICK_DIRECTION_NONE) && (readDirection != JOYSTICK_DIRECTION_CENTER)){
      currentSnakeDirection = readDirection;  
    }
    delay(50);
  }
  Serial.println("readNewSnakeDirection - Returning direction of "+currentSnakeDirection);
  return currentSnakeDirection;
}

void drawSnake(){
  for(int8_t i=0; xSnake[i]>-1; i++){
     drawPixel(xSnake[i],ySnake[i]); 
  }
}

void drawTheApple(){
  drawPixel(applePosition[0],applePosition[1]);
}

void snakeGameOver(){
  char toscroll[48];
  sprintf(toscroll, "Game Over at Level %d, snake length %d ", snakeLevel, snakeLength);
  while(true){
    lastDirection == JOYSTICK_DIRECTION_NONE;
    scrollText(toscroll, true);
    if(lastDirection == JOYSTICK_DIRECTION_CENTER){
      //reset the game and return
      resetTheGame();
      return;  
    }
    if(lastDirection == JOYSTICK_DIRECTION_UP){
      //reset to the main menu and return
      //TODO
    }  
  }
  
}



void gameModeSnakeLoop(){
  
  boolean isGameOverEvent = false;
  Serial.print("move the snake in the recorded direction\n"); 
  isGameOverEvent = moveTheSnake(currentSnakeDirection, shouldGrow);

  Serial.print("draw the moved snake\n"); 
  matrix.clear();
  drawSnake();
  drawTheApple();
  matrix.writeDisplay();
  
  //See if the snake hit itself (look for duplicate coords)

  if(isGameOverEvent){
    snakeGameOver();
    return;
  }

  //handle eating the apple
  if((xSnake[0] == applePosition[0]) && (ySnake[0] == applePosition[1])){
    //The snake ate the apple
    shouldGrow = true;
  }
  else{
    shouldGrow = false;
  }

  Serial.print("Read the new direction\n");
  readNewSnakeDirection();
}


