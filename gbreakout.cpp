#include "Arduino.h"
#include "gbreakout.h"
#include "matrixscreen.h"
#include "joystick.h"
#include "gmenu.h"

int8_t ballLocation[2] = {3,7};
int8_t ballVector[2] = {1,1};
int8_t paddleLocation = 2;
int8_t ballsLeft = 5;

boolean isBallOverPaddle(){
  if( (ballLocation[0]==paddleLocation) || (ballLocation[0]==(paddleLocation+1)) || (ballLocation[0]==(paddleLocation+2)) ){
    return true;
  }
  else{
    return false;
  }
}

boolean moveBall(){
  ballLocation[0] = ballLocation[0] + ballVector[0];
  ballLocation[1] = ballLocation[1] + ballVector[1];
  if(ballLocation[0]>7){
    //hit the right wall
    ballLocation[0] = 6;
    ballVector[0] = -1;
  }
  if(ballLocation[0]<0){
    //hit the left wall
    ballLocation[0] = 1;
    ballVector[0] = 1;
  }
  if(ballLocation[1]>14){
    //hit the bottom wall
    ballLocation[1] = 14;
    ballVector[1] = -1;
    return !isBallOverPaddle();
  }
  if(ballLocation[1]<0){
    //hit the top wall
    ballLocation[1] = 1;
    ballVector[1] = 1;
  }
  return false;
}

void drawTheball(){
  drawPixel(ballLocation[0],ballLocation[1]);
}

void drawPaddle(){
  for(byte i=0; i<3; i++){
    drawPixel(paddleLocation+i,15);  
  }
  
}

void handleControls(){
  byte direction = getJoystickDirection();
  if(direction == JOYSTICK_DIRECTION_CENTER){
    currentGameMode = GAME_MODE_MENU;
    delay(500);
    return;
  }
  else if(direction == JOYSTICK_DIRECTION_RIGHT){
    if(paddleLocation<5){
      paddleLocation++;  
    }
  }
  else if(direction == JOYSTICK_DIRECTION_LEFT){
    if(paddleLocation>0){
      paddleLocation--;  
    }
  }
}

void processLoss(){
  char toscroll[48];
  if(ballsLeft>0){
    sprintf(toscroll, "%d Balls Left", ballsLeft);
    scrollText(toscroll, false);
    ballsLeft--;    
  }
  else{
    sprintf(toscroll, "Game Over");
    scrollText(toscroll, false);
    ballsLeft = 5;  
  }
  ballLocation[0] = 3;
  ballLocation[0] = 6;
}

void gameModeBreakoutLoop(){

  matrix.clear();
  boolean isLoser = moveBall();
  drawTheball();
  drawPaddle();
  matrix.writeDisplay();
  handleControls();
  delay(110);
  if(isLoser){
    processLoss();  
  }
}

