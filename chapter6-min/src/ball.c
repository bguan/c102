#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "console.h"
#include "ball.h"

void update_ball(BALL *b, double elapsed_secs)
{
	if (b == NULL)
		return;

	double new_x = b->x;
	double new_y = b->y;
	if (elapsed_secs > 0.)
	{
		new_x += b->vx * elapsed_secs;
		new_y += b->vy * elapsed_secs;

		cbound(new_x, 0.0, get_dev_width() - 1.0);
		cbound(new_y, 0.0, get_dev_height() - 1.0);
	}

	clear_area(round(b->x-1), round(b->y-1), round(b->x+1), round(b->y+1));
	b->x = new_x;
	b->y = new_y;
	dot_at(round(b->x), round(b->y));
}

bool bounce_if_touching_top_wall(BALL *b)
{
	if (b == NULL)
		return false;

	if (b->vy > 0.0 || b->y > 0.5)
		return false;

	// no change to b->x
	clear_area(round(b->x - 1), round(b->y - 1), round(b->x + 1), round(b->y + 1));
	b->y = 2*PAD_HEIGHT;
	b->vy = -b->vy; // reverse vy direction
	b->vx = 0;

	return true;
}

bool bounce_if_touching_pad(BALL *b, PAD *p)
{
	if (b == NULL || p == NULL)
		return false;

	int half_pw = (.5 * PAD_WIDTH);
	int half_ph = (.5 * PAD_HEIGHT);
	if (b->x < (p->x - half_pw) || b->x > (p->x + half_pw))
	{
		// ball is not within pad's width
		return false;
	}
	else if (b->vy > 0 && b->y > (p->y + half_ph))
	{
		// ball is traveling down but pad is above
		return false;
	}
	else if (b->vy<0 && b->y < (p->y - half_ph))
	{
		// ball is traveling up but pad is below
		return false;
	}
	else if (b->vy > 0 && b->y >= (p->y - half_ph) && b->y <= (p->y + half_ph))
	{
		// bounce bottom pad
		clear_area(round(b->x - 1), round(b->y - 1), round(b->x + 1), round(b->y + 1));
		b->y = p->y - half_ph;
	}
	else if (b->vy < 0 && b->y <= (p->y + half_ph) && b->y >= (p->y - half_ph))
	{
		// bounce top pad
		clear_area(round(b->x - 1), round(b->y - 1), round(b->x + 1), round(b->y + 1));
		b->y = p->y + half_ph;
	}
	else
	{
		// stationary ball
		return false;
	}

	// no change to b->x 
	b->vy = -b->vy;	// reverse Y velocity
	b->vx = b->vx + SIDE_SPIN_FACTOR * p->v; // vx adjusted with side_spin

	console_beep();
	return true;
}

bool bounce_if_touching_bottom_wall(BALL *b)
{
	if (b == NULL)
		return false;

	int h = get_dev_height() - 1;
	if (b->vy < 0 || b->y < get_dev_height()-1.5)
		return false;

	// no change to b->x
	clear_area(round(b->x - 1), round(b->y - 1), round(b->x + 1), round(b->y + 1));
	b->y = h - 2*PAD_HEIGHT;
	b->vy = -b->vy; // reverse Y velocity
	b->vx = 0;

	return true;
}

bool bounce_if_touching_left_wall(BALL *b)
{
	if (b == NULL)
		return false;

	if (b->x > 0)
		return false;

	// no change to b->y
	b->x = 0;
	b->vx = -b->vx;	   // reverse x velocity

	return true;
}

bool bounce_if_touching_right_wall(BALL *b)
{
	if (b == NULL)
		return false;

	int rightmost_x = get_dev_width() - 1;
	if (b->x < rightmost_x)
		return false;

	// no change to b->y
	b->x = rightmost_x;
	b->vx = -b->vx;	   // reverse x velocity

	return true;
}
