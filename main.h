#ifndef MAIN_H
#define MAIN_H

#include "gba.h"
#include "game.h"

int isStart(void);

void stateLose(void);

struct snake {
  int length;
  int row;
  int col;
};

struct food {
  int row;
  int col;
};

#endif