#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "ball.h"
#include "pad.h"

typedef struct {
	int my_score, oppo_score;
	PAD* my_pad, oppo_pad;
	BALL* ball;
} GAME;

GAME* init_game();
void start_game(GAME* g);
void update_game(GAME* g, char input, double elapsed_secs);
bool is_over(GAME* g);
void free_game(GAME* g);

#endif
