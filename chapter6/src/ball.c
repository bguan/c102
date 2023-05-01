#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "console.h"
#include "ball.h"

BALL *init_ball(double rad, double x, double y, double vx, double vy)
{
	BALL *b = malloc(sizeof(BALL));
	if (b == NULL)
	{
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

void dispose_ball(BALL *b)
{
	if (b == NULL)
		return;

	clear_area(b->x - b->rad, b->y - b->rad, b->x + b->rad, b->y + b->rad);
	free(b);
}

void update_ball(BALL *b, double elapsed_secs)
{
	if (b == NULL)
		return;

	double new_x = b->x;
	double new_y = b->y;
	if (elapsed_secs > 0.)
	{
		new_x = bound_x(b->x + b->vx * elapsed_secs);
		new_y = bound_y(b->y + b->vy * elapsed_secs);
	}

	// if there's meaningful diff btw old and new x y, clear & redraw
	if (elapsed_secs < 0. || fabs(new_x - b->x) > NORM_MIN_DIFF || fabs(new_y - b->y) > NORM_MIN_DIFF)
	{
		clear_area(b->x - 2 * b->rad, b->y - 2 * b->rad, b->x + 2 * b->rad, b->y + 2 * b->rad);
		b->x = new_x;
		b->y = new_y;
		circle_at(b->rad, b->x, b->y, BALL_COLOR);
	}
}

bool bounce_if_touching_top_wall(BALL *b, double top_y)
{
	if (b == NULL)
		return false;

	if (b->y - b->rad > top_y)
		return false;

	// no change to b->x
	b->y = top_y + b->rad;
	b->vy = -b->vy;		// reverse Y velocity

	return true;
}

bool bounce_if_touching_pad(BALL *b, PAD *p)
{
	if (b == NULL || p == NULL)
		return false;

	if ((b->x + b->rad) < (p->x - p->w/2) || (b->x - b->rad) > (p->x + p->w/2))
	{
		// ball is not within pad's width
		return false;
	}
	else if (b->vy > 0)
	{
		// check against bottom pad
		if ((b->y + b->rad) < (p->y - p->h / 2) || (b->y - b->rad) > (p->y + p->h / 2))
		{
			return false;
		}
		else
		{
			b->y = p->y - b->rad - p->h / 2 - NORM_MIN_DIFF/10.;
		}
	} 
	else if (b->vy < 0)
	{
		// check against top pad
		if ((b->y - b->rad) > (p->y + p->h / 2) || (b->y + b->rad) < (p->y - p->h / 2))
		{
			return false;
		}
		else
		{
			b->y = p->y + b->rad + p->h / 2 + NORM_MIN_DIFF / 10.;
		}
	}
	else
	{
		// stationary ball
		return false;
	}

	// no change to b->x 
	b->vy = -b->vy;	// reverse Y velocity
	b->vx = b->vx + SIDE_SPIN_FACTOR * p->v; // vx adjusted with side_spin

	// make sure vx is not too crazy
	if (fabs(b->vx) > 2.0) 
	{
		b->vx = (b->vx > 0. ? 2.0 : -2.0);
	}

	console_beep();
	return true;
}

bool bounce_if_touching_bottom_wall(BALL *b, double bot_y)
{
	if (b == NULL)
		return false;

	if (b->y + b->rad < bot_y)
		return false;

	// no change to b->x
	b->y = bot_y - b->rad;
	b->vy = -b->vy;		// reverse Y velocity

	return true;
}

bool bounce_if_touching_left_wall(BALL *b, double left_x)
{
	if (b == NULL)
		return false;

	if (b->x > left_x)
		return false;

	// no change to b->y
	b->x = left_x;
	b->vx = -b->vx;	   // reverse x velocity

	return true;
}

bool bounce_if_touching_right_wall(BALL *b, double right_x)
{
	if (b == NULL)
		return false;

	if (b->x < right_x)
		return false;

	// no change to b->y
	b->x = right_x;
	b->vx = -b->vx;	   // reverse x velocity

	return true;
}
