#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "gba.h"
#include "main.h"

#include "images/snake_start.h"
#include "images/snake_background.h"
#include "images/snake_lose.h"


struct snake head;
struct food nom;
int headCellIndex;
int backCellIndex;
struct snake cells[2000];
enum snake_direction {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NONE
};

enum snake_direction direction;

void reset(void) {
	drawFullScreenImageDMA(snake_start);
	drawString(100, 70, "Hit Start to Play", WHITE);
	head.length = 1;
	head.row = randint(3, 13) * 10;
	head.col = randint(3, 21) * 10;
	nom.row = randint(3, 13) * 10;
	nom.col = randint(3, 21) * 10;
	headCellIndex = 0;
	backCellIndex = 0;
	cells[backCellIndex] = head;
	direction = NONE;
}

void startGame(void) {
	drawFullScreenImageDMA(snake_background);
	drawRectDMA(0, 0, WIDTH, 20, GRAY);
	drawRectDMA(HEIGHT - 20, 0, WIDTH, 20, GRAY);
	drawRectDMA(0, 0, 20, HEIGHT, GRAY);
	drawRectDMA(0, WIDTH - 20, 20, HEIGHT, GRAY);
	drawScore();
	drawRectDMA(head.row, head.col, SIZE, SIZE, BLUE);
	drawRectDMA(nom.row, nom.col, SIZE, SIZE, RED);
}

void moveSnake(int dr, int dc) {
	if (dr == 1) { //down
		if (direction != UP || head.length == 1) {
			direction = DOWN;
			struct snake tail = cells[backCellIndex];
			drawPartialImageDMA(tail.row, tail.col, SIZE, SIZE, snake_background); //delete the tail
			backCellIndex++;
			struct snake newHead;
			newHead.row = head.row + SIZE;
			newHead.col = head.col;
			headCellIndex++;
			cells[headCellIndex] = newHead;
			head = newHead;
			drawRectDMA(head.row, head.col, SIZE, SIZE, BLUE); //draw the new head
			checkCollision();
			delay(3);
		}

	} else if (dr == -1) { //up
		if (direction != DOWN || head.length == 1) {
			direction = UP;
			struct snake tail = cells[backCellIndex];
			drawPartialImageDMA(tail.row, tail.col, SIZE, SIZE, snake_background); //delete the tail
			backCellIndex++;
			struct snake newHead;
			newHead.row = head.row - SIZE;
			newHead.col = head.col;
			headCellIndex++;
			cells[headCellIndex] = newHead;
			head = newHead;
			drawRectDMA(head.row, head.col, SIZE, SIZE, BLUE); //draw the new head
			checkCollision();
			delay(3);
		}

	} else if (dc == 1) { //right
		if (direction != LEFT || head.length == 1) {
			direction = RIGHT;
			struct snake tail = cells[backCellIndex];
			drawPartialImageDMA(tail.row, tail.col, SIZE, SIZE, snake_background); //delete the tail
			backCellIndex++;
			struct snake newHead;
			newHead.row = head.row;
			newHead.col = head.col + SIZE;
			headCellIndex++;
			cells[headCellIndex] = newHead;
			head = newHead;
			drawRectDMA(head.row, head.col, SIZE, SIZE, BLUE); //draw the new head
			checkCollision();
			delay(3);
		}

	} else if (dc == -1) { //left
		if (direction != RIGHT || head.length == 1) {
			direction = LEFT;
			struct snake tail = cells[backCellIndex];
			drawPartialImageDMA(tail.row, tail.col, SIZE, SIZE, snake_background); //delete the tail
			backCellIndex++;
			struct snake newHead;
			newHead.row = head.row;
			newHead.col = head.col - SIZE;
			headCellIndex++;
			cells[headCellIndex] = newHead;
			head = newHead;
			drawRectDMA(head.row, head.col, SIZE, SIZE, BLUE); //draw the new head
			checkCollision();
			delay(3);
		}

	} else { //previous direction
		if (direction == RIGHT) 
        	moveSnake(0, 1);
	    else if (direction == LEFT) {
	        moveSnake(0, -1);
	    } else if (direction == UP) {
	        moveSnake(-1, 0);
	    } else if (direction == DOWN) {
	      moveSnake(1, 0);
		}
	}
	if (!isStart())
		drawScore();
}

void delay(int n) {
        // delay for n tenths of a second
	volatile int x = 0;
	for (int i=0; i<n*8000; i++)
	        x++;
}

void checkCollision(void) {
	//check if collide with boundary
	if (head.row < 20) {
		drawRectDMA(head.row, head.col, SIZE, SIZE, YELLOW); //draw the collision
		direction = NONE;
		delay(20);
		lose();
		return;
	} else if (head.row >= HEIGHT - 20) {
		drawRectDMA(head.row, head.col, SIZE, SIZE, YELLOW); //draw the collision
		direction = NONE;
		delay(20);
		lose();
		return;
	} else if (head.col < 20) {
		drawRectDMA(head.row, head.col, SIZE, SIZE, YELLOW); //draw the collision
		delay(20);
		lose();
		return;
	} else if (head.col >= WIDTH - 20) {
		drawRectDMA(head.row, head.col, SIZE, SIZE, YELLOW); //draw the collision
		delay(20);
		lose();
		return;
	}

	//check if collide with snake
	for (int i = backCellIndex; i < headCellIndex; i++) {
		struct snake toCheck = cells[i];
		if (toCheck.row == head.row) {
			if (toCheck.col == head.col) {
				//LOSE
				drawRectDMA(head.row, head.col, SIZE, SIZE, YELLOW); //draw the collision
				delay(20);
				lose();
			}
		}
	}
	//check if collide with food
	if (nom.row == head.row) {
		if (nom.col == head.col) {
			//FOOD
			backCellIndex--;
			head.length++;
			struct snake toDraw = cells[backCellIndex];
			drawRectDMA(toDraw.row, toDraw.col, SIZE, SIZE, BLUE);
			//GENERATE NEW FOOD
			int doneCheck = 1;
			while (doneCheck == 1) {
				nom.row = randint(3, 13) * 10;
				nom.col = randint(3, 21) * 10;
				if (nom.row == head.row && nom.col == head.col) {
					//do  nothing
				} else {
					doneCheck = 0;
					drawRectDMA(nom.row, nom.col, SIZE, SIZE, RED);
				}
			}
		}
	}
}

void drawScore(void) {
	drawRectDMA(0, 0, WIDTH, 20, GRAY);
	drawString(5, 2, "Score: ", WHITE);
	int number = head.length;
	char digits[10];
	int i = 0;
	while (number != 0) {
	    digits[i] = number % 10 + '0';
	    number = number / 10;
	    i++;
	}
	digits[i] = '0';
	digits[++i] = 0;
	char scoreString[i];
	for (int j = 0; j < i; j++) {
		scoreString[j] = digits[i-j-1];
	}

	drawString(5, 37, scoreString, WHITE);
}

void lose(void) {
	stateLose();
	drawImageDMA(50, 70, 100, 60, snake_lose);
	drawString(120, 70, "Hit Start to Play", RED);
}