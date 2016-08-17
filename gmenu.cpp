#include "gmenu.h"
#include "matrixscreen.h"
#include "joystick.h"

extern byte currentGameMode = GAME_MODE_MENU;
extern byte menuCurrentSelected = GAME_MODE_SNAKE;


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
