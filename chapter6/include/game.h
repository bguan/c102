#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "console.h"
#include "ball.h"
#include "pad.h"

#define MIN_WIDTH 20
#define MIN_HEIGHT 20
#define KEY_START_GAME ' '
#define KEY_QUIT_GAME 'q'
#define KEY_GO_LEFT 'z'
#define KEY_GO_RIGHT 'x'

#define BALL_SIZE 0.025
#define BALL_INIT_SPEED 0.5
#define PAD_WIDTH 0.1
#define PAD_HEIGHT 0.05
#define MAX_MSG_LEN 100
#define MSG_COLOR GREEN_ON_BLACK
#define GAME_MAX_SCORE 3

typedef struct {
	int my_score, oppo_score;
	PAD *my_pad;
	PAD *oppo_pad;
	BALL* ball;
} GAME;

GAME* init_game();
bool update_game(GAME* g, double elapsed_secs);
void dispose_game(GAME* g);

#endif
