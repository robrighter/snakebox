#include "Arduino.h"
#include "gsnake.h"
#include "matrixscreen.h"
#include "joystick.h"
#include "gmenu.h"



byte countDownDelay = 15;
byte snakeLevel = 1;
byte snakeLength = 2;
int8_t xSnake[128] = {1,0,-1};
int8_t ySnake[128] = {0,0,-1};
byte currentSnakeDirection = JOYSTICK_DIRECTION_RIGHT;
int8_t applePosition[2] = {2,5};
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
  char coordprint[12];
  for(byte i=0;i<20; i++){
    sprintf(coordprint, "(%d,%d), ",xSnake[i], ySnake[i]);
    Serial.print(coordprint);
  }
  Serial.print("\n\n");
  return false;
}

boolean isCoordInSnake(int8_t x, int8_t y, byte startFrom=0){
  //char toprint[100];
  //sprintf(toprint, "isCoordInSnake x=%d y=%d starting from %d", x, y, startFrom);
  //Serial.println(toprint);
  while(xSnake[startFrom] != -1){
    if((xSnake[startFrom] == x) && (ySnake[startFrom] == y)){
      //Serial.println("Returning True");
      return true;  
    }
    startFrom++;
  }
  //Serial.println("Returning False");
  return false;
}

boolean doesSnakeOverlap(){
  byte i = 0;
  while(xSnake[i] != -1){
    if(isCoordInSnake(xSnake[i], ySnake[i], i+1)){
      return true;
    }
    i++;
  }
  return false;
}


void setNewApplePosition(byte direction){
  byte possibleLocationsForTheApple = snakeLength - 128;
  long choosenLocation = random(possibleLocationsForTheApple);
  int8_t x;
  int8_t y;
  //iterate through the possible locations until we get to the choosen location
  int i=0;
  for(y=15; y>-1; y--){
    for(x=0; x<8; x++){
      
      if(!isCoordInSnake(x, y)){
        i++;
        if(i==choosenLocation){
          applePosition[0] = x;
          applePosition[1] = y;
          return;  
        }  
      }
    }  
  }
  //should never get here
}


byte readNewSnakeDirection(){
  Serial.print("readNewSnakeDirection - Listening for new direction\n"); 
  for(byte i=0; i<(countDownDelay - snakeLevel ); i++){
    byte readDirection = getJoystickDirection();
    if((readDirection != JOYSTICK_DIRECTION_NONE) && (readDirection != JOYSTICK_DIRECTION_CENTER)){
      currentSnakeDirection = readDirection;  
    }
    delay(10);
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
  sprintf(toscroll, "Game Over length %d Level %d ", snakeLength, snakeLevel);
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
      currentGameMode = GAME_MODE_MENU;
      return;
    }  
  }
  
}



void gameModeSnakeLoop(){
  
  boolean isGameOverEvent = false;
  Serial.print("move the snake in the recorded direction\n"); 
  isGameOverEvent = moveTheSnake(currentSnakeDirection, shouldGrow);
  if(shouldGrow){
    //last time we ate the apple so lets move it to a new location
    setNewApplePosition(currentSnakeDirection);
  }

  Serial.print("draw the moved snake\n"); 
  matrix.clear();
  drawSnake();
  drawTheApple();
  matrix.writeDisplay();
  
  //See if the snake hit itself (look for duplicate coords)
  if(!isGameOverEvent){
    isGameOverEvent = doesSnakeOverlap();
  }
  
  if(isGameOverEvent){
    snakeGameOver();
    return;
  }

  //handle eating the apple
  if( isCoordInSnake(applePosition[0], applePosition[1]) ){
    //The snake ate the apple
    shouldGrow = true;
    snakeLength++;
  }
  else{
    shouldGrow = false;
  }

  Serial.print("Read the new direction\n");
  readNewSnakeDirection();
}


