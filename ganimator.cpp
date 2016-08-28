#include "ganimator.h"
#include "matrixscreen.h"
#include "joystick.h"


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
      screenMatrix[pointerPosition[0]][pointerPosition[1]] = !screenMatrix[pointerPosition[0]][pointerPosition[1]];  
    }
    lastDirection = direction;
    movePointerByJoystickDirection( direction);  
  }
  drawMatrix();
  drawPixel(pointerPosition[0],pointerPosition[1]);
  matrix.writeDisplay();
  delay(30);  
}
