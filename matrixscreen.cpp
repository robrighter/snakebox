#include "matrixscreen.h"
#include "joystick.h"

extern boolean screenMatrix[8][16] = {
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

extern Adafruit_8x16matrix matrix = Adafruit_8x16matrix();

void drawMatrix(){
  for(int x=0;x<8;x++){
    for(int y=0;y<16;y++){
      if(screenMatrix[x][y] == true){
        drawPixel(x,y);  
      }
    }
  }
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
    delay(50);
  }
}
