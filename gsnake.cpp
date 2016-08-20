#include "gsnake.h"
#include "matrixscreen.h"
#include "joystick.h"
#include "gmenu.h"


int countDownDelay = 20;
int snakeLevel = 1;
int xSnake[129] = {1,0,-1};
int ySnake[129] = {0,0,-1};
byte currentSnakeDirection = JOYSTICK_DIRECTION_RIGHT;
byte applePosition[2] = {2,0};


boolean moveTheSnake(byte direction){
  //find the tail (-1,-1)
  int offset = 0;
  while(xSnake[offset]>-1){
    offset++;
  }
  //make the new tail
  xSnake[offset+2] = -1;
  ySnake[offset+2] = -1;
  //count back and move all the array values down
  while(offset>-1){
    xSnake[offset+1] = xSnake[offset]; 
    ySnake[offset+1] = ySnake[offset]; 
    offset--;
  }
  
  //insert the new coord at the head (0,0)
  if(direction == JOYSTICK_DIRECTION_UP){
      ySnake[0]--;
      if(ySnake[0]<0){
        //hit the end of the screen
        return false; 
      }
  }
  if(direction == JOYSTICK_DIRECTION_DOWN){
      ySnake[0]++;
      if(ySnake[0]>16){
        //hit the end of the screen
        return false; 
      }  
  }
  if(direction == JOYSTICK_DIRECTION_LEFT){
      xSnake[0]--;
      if(xSnake[0]<0){
        //hit the end of the screen
        return false; 
      }  
  }
  if(direction == JOYSTICK_DIRECTION_RIGHT){
    xSnake[0]++;
    if(xSnake[0]>8){
      //hit the end of the screen
      return false; 
    }   
  }
  return true;
}

byte readNewSnakeDirection(){
  for(int i=0; i<(countDownDelay - snakeLevel ); i++){
    byte readDirection = getJoystickDirection();
    if((readDirection != JOYSTICK_DIRECTION_NONE) && (readDirection != JOYSTICK_DIRECTION_CENTER)){
      currentSnakeDirection = readDirection;  
    }
    delay(50);
  }
  return currentSnakeDirection;
}


void gameModeSnakeLoop(){
  //move the snake in the recorded direction
  moveTheSnake(currentSnakeDirection);

  //draw the moved snake

  
  //See if the snake hit itself (look for duplicate coords)

  //handle eating the apple
    //Detect the collide
    //Extend the length of the snake

  //Read the new direction
  readNewSnakeDirection();
}


