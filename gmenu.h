#ifndef GMENU_H
#define GMENU_H

#include "Arduino.h"

const byte GAME_MODE_MENU = 1;
const byte GAME_MODE_SNAKE = 2;
const byte GAME_MODE_ANIMATOR = 3;
const byte GAME_MODE_BREAKOUT = 4;
const byte GAME_MODE_TETRIS = 5;
const byte LAST_GAME_MODE = GAME_MODE_BREAKOUT;
const byte FIRST_GAME_MODE = GAME_MODE_SNAKE;

extern byte currentGameMode;
extern byte menuCurrentSelected;


void gameModeMenuLoop();


#endif
