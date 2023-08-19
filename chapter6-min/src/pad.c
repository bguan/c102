#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "console.h"
#include "pad.h"

void update_pad(PAD *p, double elapsed_secs)
{
	if (p == NULL)
		return;

	double half_w = ceil(.5 * PAD_WIDTH);
	double half_h = ceil(.5 * PAD_HEIGHT);
	
	double new_x = p->x;
	if (elapsed_secs > 0.)
	{
		new_x += p->v * elapsed_secs;
		new_x = cbound(new_x, half_w, get_dev_width() - half_w);
	}

	clear_area(round(p->x - half_w), round(p->y - half_h), round(p->x + half_w)+1, round(p->y + half_h)+1);

	p->x = new_x;
	rect_at(PAD_WIDTH, PAD_HEIGHT, round(p->x), round(p->y));
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
