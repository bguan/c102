#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "console.h"
#include "ball.h"
#include "pad.h"
#include "game.h"

void game_over_screen(GAME* g)
{
	char msg[MAX_MSG_LEN];
	sprintf((char *)msg, "Game Over. Score %d : %d", g->my_score, g->oppo_score);
	text_at((char *)msg, 0., -.5, MSG_COLOR, TXT_CENTER);
	sprintf((char *)msg,
			"[SPACE] start, [%c] quit, [%c] left, [%c] stay, [%c] right",
			KEY_QUIT_GAME, KEY_GO_LEFT, KEY_STAY_PUT, KEY_GO_RIGHT);
	text_at((char *)msg, 0., .45, MSG_COLOR, TXT_CENTER);
}

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

	double rad = BALL_SIZE;
	
	// keep increasing ball size until its visible
	while (to_dev_width(rad) < 1)
	{
		rad *= 1.5;
	}

	g->my_score = 0;
	g->oppo_score = 0;
	g->ball = init_ball(rad, 0., 0., 0., 0.);
	g->my_pad = init_pad(PAD_WIDTH, PAD_HEIGHT, 0., 0.4, 0.);
	g->oppo_pad = init_pad(PAD_WIDTH, PAD_HEIGHT, 0., -0.4, 0.);
	if (g->ball == NULL || g->my_pad == NULL || g->oppo_pad == NULL)
	{
		perror("Failed to allocate memory for Ball and/or Pads!");
		exit(1);
	}

	game_over_screen(g);

	return g;
}

void start_game(GAME *g)
{
	if (g == NULL || g->ball == NULL || g->my_pad == NULL || g->oppo_pad == NULL)
		exit(1);

	clear_area(-.5, -.5, .5, .5);
	g->my_score = 0;
	g->oppo_score = 0;
	g->ball->x = 0.;
	g->ball->y = -0.3;
	g->ball->vy = BALL_INIT_SPEED;
	g->ball->vx = 0.;
	g->my_pad->x = 0.;
	g->my_pad->v = PAD_SPEED * 0.5; // give some speed to force redraw
	g->oppo_pad->x = 0.;
	g->oppo_pad->v = PAD_SPEED * 0.5; // give some speed to force redraw
}

bool is_over(GAME *g)
{
	if (g == NULL)
		exit(1);

	return (fabs(g->ball->vy) < NORM_MIN_DIFF || g->my_score >= GAME_MAX_SCORE || g->oppo_score >= GAME_MAX_SCORE);
}

double update_game_and_pause(GAME *g, double elapsed_secs)
{
	if (g == NULL)
		exit(1);

	double pause = 0.002; // default to pausing 2 millisec after every update

	if (elapsed_secs <= 0)
		return pause;

	char c = key_pressed();
	if (c == KEY_QUIT_GAME)
	{
		return -1.0;
	}

	if (is_over(g))
	{
		if (c == KEY_START_GAME)
		{
			console_beep();
			start_game(g);
			pause = 1.0;
		}
		else
		{
			game_over_screen(g);
		}
	} 
	else
	{
		update_ball(g->ball, elapsed_secs);
		update_pad(g->my_pad, elapsed_secs);
		update_pad(g->oppo_pad, elapsed_secs);

		bounce_if_touching_left_wall(g->ball, -.45);
		bounce_if_touching_right_wall(g->ball, .45);

		if (bounce_if_touching_top_wall(g->ball, -.45))
		{
			g->my_pad->x = 0.;
			g->oppo_pad->x = 0.;
			g->my_score++;
			g->ball->x = 0.;
			g->ball->y = -0.3;
			g->ball->vx = 0.;

			clear_area(-.5, -.5, .5, .5);
			update_ball(g->ball, -1.);
			update_pad(g->my_pad, -1.);
			update_pad(g->oppo_pad, -1.);
			console_beep();
			pause = 1.0;
		}
		else if (bounce_if_touching_bottom_wall(g->ball, .45))
		{
			g->my_pad->x = 0.;
			g->oppo_pad->x = 0.;
			g->oppo_score++;
			g->ball->x = 0.;
			g->ball->y = 0.3;
			g->ball->vx = 0.;

			clear_area(-.5, -.5, .5, .5);
			update_ball(g->ball, -1.);
			update_pad(g->my_pad, -1.);
			update_pad(g->oppo_pad, -1.);
			console_beep();
			pause = 1.0;
		}

		bounce_if_touching_pad(g->ball, g->oppo_pad);
		bounce_if_touching_pad(g->ball, g->my_pad);

		if (c == KEY_GO_LEFT)
			pad_go_left(g->my_pad);
		else if (c == KEY_GO_RIGHT)
			pad_go_right(g->my_pad);
		else if (c == KEY_STAY_PUT)
			pad_stop(g->my_pad);

		if (g->ball->x < (g->oppo_pad->x - .2 * g->oppo_pad->w))
			pad_go_left(g->oppo_pad);
		else if (g->ball->x > (g->oppo_pad->x + .2 * g->oppo_pad->w))
			pad_go_right(g->oppo_pad);
		else if (fabs(g->oppo_pad->y - g->ball->y) > 0.1)
			pad_stop(g->oppo_pad);

		char msg[MAX_MSG_LEN];
		sprintf((char *)msg, "Score %d : %d", g->my_score, g->oppo_score);
		text_at((char *)msg, 0., -.5, MSG_COLOR, TXT_CENTER);

		sprintf((char *)msg, "Dim: %d x %d", get_dev_width(), get_dev_height());
		text_at((char *)msg, -.5, -.5, MSG_COLOR, TXT_LEFT);

		sprintf((char *)msg, "Interval: %.9f", elapsed_secs);
		text_at((char *)msg, .5, -.5, MSG_COLOR, TXT_RIGHT);
	}

	return pause;
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
