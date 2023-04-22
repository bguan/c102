#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "console.h"
#include "ball.h"

BALL* init_ball(double rad, double x, double y, double vx, double vy)
{
	BALL* b = malloc(sizeof(BALL));
	if (b == NULL) {
		perror("Failed to allocate memory for BALL!");
		return NULL;
	}

	b->rad = rad;
	b->x = x;
	b->y = y;
	b->vx = vx;
	b->vy = vy;

	circle_at(b->rad, b->x, b->y, BALL_COLOR);

	return b;
}

void dispose_ball(BALL* b)
{
	if (b != NULL) 
	{
		clear_area(b->x - b->rad, b->y - b->rad, b->x + b->rad, b->y + b->rad);
		free(b);
	}
	return;
}

void update_ball(BALL* b, double elapsed_secs)
{
	double new_x = b->x + b->vx * elapsed_secs;
	double new_y = b->y + b->vy * elapsed_secs;

	// if there's meaningful diff btw old and new x y, clear & redraw
	if (fabs(new_x - b->x) > NORM_MIN_DIFF || fabs(new_y- b->y) > NORM_MIN_DIFF) 
	{
		clear_area(b->x - b->rad, b->y - b->rad, b->x + b->rad, b->y + b->rad);
		b->x = new_x;
		b->y = new_y;
		circle_at(b->rad, b->x, b->y, BALL_COLOR);
	}
}

bool bounce_if_touching_top(BALL* b, double top_y, double side_spin)
{
	if (b->y > top_y) return false;

	// no change to b->x
	b->y = top_y;
	b->vy = -b->vy; // reverse Y velocity
	b->vx += side_spin; // vx adjusted with side_spin

	return true;
}

bool bounce_if_touching_bottom(BALL* b, double bot_y, double side_spin)
{
	if (b->y < bot_y) return false;

	// no change to b->x
	b->y = bot_y;
	b->vy = -b->vy; // reverse Y velocity
	b->vx += side_spin; // vx adjusted with side_spin

	return true;
}

bool bounce_if_touching_left(BALL* b, double left_x, double top_spin)
{
	if (b->x > left_x) return false;

	// no change to b->y
	b->x = left_x;
	b->vx = -b->vx; // reverse x velocity
	b->vy += top_spin; // vx adjusted with side_spin

	return true;
}

bool bounce_if_touching_right(BALL* b, double right_x, double top_spin)
{
	if (b->x < right_x) return false;

	// no change to b->y
	b->x = right_x;
	b->vx = -b->vx; // reverse x velocity
	b->vy += top_spin; // vx adjusted with side_spin

	return true;
}
