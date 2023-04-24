#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "console.h"
#include "ball.h"
#include "pad.h"
#include "game.h"

GAME *init_game()
{
	init_console();

	int w = get_dev_width();
	int h = get_dev_height();

	if (w < MIN_WIDTH || h < MIN_HEIGHT)
	{
		end_console();
		printf(
			"Console Width Height %d x %d less than min %d x %d\n",
			w, h, MIN_WIDTH, MIN_HEIGHT);
		exit(1);
	}

	GAME *g = malloc(sizeof(GAME));
	if (g == NULL)
	{
		perror("Failed to allocate memory for GAME!");
		exit(1);
	}

	// clear the whole screen
	clear_area(-.5, -.5, .5, .5);

	g->my_score = 0;
	g->oppo_score = 0;
	g->ball = init_ball(BALL_SIZE, 0., 0., 0., 0.);
	g->my_pad = init_pad(PAD_WIDTH, PAD_HEIGHT, 0., 0.4, 0.);
	g->oppo_pad = init_pad(PAD_WIDTH, PAD_HEIGHT, 0., -0.4, 0.);
	if (g->ball == NULL || g->my_pad == NULL || g->oppo_pad == NULL)
	{
		perror("Failed to allocate memory for Ball and/or Pads!");
		exit(1);
	}

	char msg[MAX_MSG_LEN];
	sprintf((char *)msg, "My Score: %d.  Opponent: %d", g->my_score, g->oppo_score);
	text_at((char *)msg, 0., -.5, MSG_COLOR, TXT_CENTER);
	sprintf((char *)msg, "Game Over.  [SPACE] to start, [%c] to quit", KEY_QUIT_GAME);
	text_at((char *)msg, 0., .45, MSG_COLOR, TXT_CENTER);

	return g;
}

void start_game(GAME *g)
{
	if (g == NULL || g->ball == NULL || g->my_pad == NULL || g->oppo_pad == NULL)
		exit(1);

	clear_area(-.5, -.5, .5, -.45);
	clear_area(-.5, .45, .5, .5);
	g->my_score = 0;
	g->oppo_score = 0;
	g->ball->x = 0.;
	g->ball->y = 0.;
	g->ball->vy = BALL_INIT_SPEED;
	g->ball->vx = 0.;
	g->my_pad->x = 0.;
	g->my_pad->v = 0.;
	g->oppo_pad->x = 0.;
	g->oppo_pad->v = 0.;
}

bool is_over(GAME *g)
{
	if (g == NULL)
		exit(1);

	return (fabs(g->ball->vy) < NORM_MIN_DIFF || g->my_score >= GAME_MAX_SCORE || g->oppo_score >= GAME_MAX_SCORE);
}

bool update_game(GAME *g, double elapsed_secs)
{
	if (g == NULL)
		exit(1);

	char c = key_pressed();
	if (c == KEY_QUIT_GAME)
	{
		return false;
	}
	else if (is_over(g))
	{
		if (c == KEY_START_GAME)
			start_game(g);
	} 
	else
	{
		update_ball(g->ball, elapsed_secs);
		update_pad(g->my_pad, elapsed_secs);
		update_pad(g->oppo_pad, elapsed_secs);

		bounce_if_touching_left_wall(g->ball, -.45);
		bounce_if_touching_right_wall(g->ball, .45);
		bounce_if_touching_top_wall(g->ball, -.45);
		bounce_if_touching_bottom_wall(g->ball, .45);

		bounce_if_touching_top_pad(g->ball, g->oppo_pad);
		bounce_if_touching_bottom_pad(g->ball, g->my_pad);

		if (c == KEY_GO_LEFT)
			pad_go_left(g->my_pad);
		else if (c == KEY_GO_RIGHT)
			pad_go_right(g->my_pad);

		if (g->ball->x < (g->oppo_pad->x - .4 * g->oppo_pad->w))
			pad_go_left(g->oppo_pad);
		else if (g->ball->x > (g->oppo_pad->x + .4 * g->oppo_pad->w))
			pad_go_right(g->oppo_pad);

		char msg[MAX_MSG_LEN];
		sprintf((char *)msg, "%d  :  %d", g->my_score, g->oppo_score);
		text_at((char *)msg, 0., -.5, MSG_COLOR, TXT_CENTER);
	}
	return true;
}

void dispose_game(GAME *g)
{
	if (g == NULL)
		return;

	dispose_ball(g->ball);
	dispose_pad(g->my_pad);
	dispose_pad(g->oppo_pad);

	end_console();
	free(g);
}
