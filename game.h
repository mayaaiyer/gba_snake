#ifndef GAME_H
#define GAME_H

#include "gba.h"
#include "main.h"

// The size of a snake cell
#define SIZE 5

void reset(void);

void startGame(void);

void moveSnake(int dr, int dc);

void delay(int n);

void checkCollision(void);

void drawScore(void);

void lose(void);

#endif