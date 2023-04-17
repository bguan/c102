#ifndef BALL_H
#define BALL_H

#include <stdbool.h>

typedef struct {
	float rad, x, y, vx, vy;
} BALL;

BALL* init_ball(float rad, float x, float y, float vx, float vy);

void free_ball(BALL* b);

void update_ball(BALL* b, float elapse_secs);

bool bounce_if_touching_top(BALL* b, float top_y);
bool bounce_if_touching_bottom(BALL* b, float bot_y);
bool bounce_if_touching_left(BALL* b, float left_x);
bool bounce_if_touching_right(BALL* b, float right_x);

#endif
