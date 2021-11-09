#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "game.h"

enum gba_state {
  START,
  PLAY,
  GAMEOVER,
};

enum gba_state state;
int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //

  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  state = START;

  int isReset = 1;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    waitForVBlank();
    if (KEY_DOWN(BUTTON_SELECT, currentButtons) || isReset) {
      reset(); // reset whenever select is hit or at the start
      state = START;
      isReset = 0; 
    } else if (KEY_DOWN(BUTTON_START, currentButtons)) {
      switch(state) {
      case START: // start the game
        state = PLAY;
        startGame();
        break;
      case GAMEOVER: // reset
        state = START;
        reset();
        break;
      case PLAY: // middle of game play, do nothing
        //do nothing
         break;
      }
    } else if (state == PLAY) {
      if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
        moveSnake(0, 1);
      } else if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
        moveSnake(0, -1);
      } else if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          moveSnake(-1, 0);
      } else if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
        moveSnake(1, 0);
      } else {
        moveSnake(0,0);
      }
    } 

    previousButtons = currentButtons; // Store the current state of the buttons
  }
  UNUSED(previousButtons);

  return 0;
}

int isStart(void) {
  if (state == START)
    return 1;
  return 0;
}

void stateLose(void) {
  state = GAMEOVER;
}