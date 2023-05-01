#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "console.h"
#include "pad.h"

CONS_COLOR pick_color(PAD *p)
{
	return p == NULL || p->y < 0 ? TOP_PAD_COLOR : BOT_PAD_COLOR;
}

PAD *init_pad(double w, double h, double x, double y, double v)
{
	PAD *p = malloc(sizeof(PAD));
	if (p == NULL)
	{
		perror("Failed to allocate memory for PAD!");
		return NULL;
	}

	p->w = w;
	p->h = h;
	p->x = x;
	p->y = y;
	p->v = v;

	rect_at(w, h, x, y, pick_color(p));

	return p;
}

void dispose_pad(PAD *p)
{
	if (p == NULL)
		return;

	clear_area(p->x - p->w / 2, p->y - p->h / 2, p->x + p->w / 2, p->y + p->h / 2);

	free(p);
}

void update_pad(PAD *p, double elapsed_secs)
{
	if (p == NULL)
		return;

	double new_x = p->x;
	if (elapsed_secs > 0.)
	{
		new_x += p->v * elapsed_secs;
	}

	// if elapsed_secs negative force redraw or if
	// there's meaningful diff btw old and new x, clear & redraw
	if (elapsed_secs < 0. || fabs(new_x - p->x) > NORM_MIN_DIFF)
	{
		clear_area(p->x - p->w, p->y - p->h, p->x + p->w, p->y + p->h);
		p->x = bound_x(new_x);

		if (p->x - p->w / 1.5 < -.5)
			p->x = -.5 + p->w / 1.5;
		else if (p->x + p->w / 1.5 > .5)
			p->x = .5 - p->w / 1.5;

		rect_at(p->w, p->h, p->x, p->y, pick_color(p));
	}
}

void pad_stop(PAD *p)
{
	if (p != NULL)
	{
		p->v = 0.;
	}
}

void pad_go_left(PAD *p)
{
	if (p != NULL)
	{
		p->v = -PAD_SPEED;
	}
}

void pad_go_right(PAD *p)
{
	if (p != NULL)
	{
		p->v = PAD_SPEED;
	}
}
