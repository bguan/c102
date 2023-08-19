#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "console.h"
#include "ball.h"
#include "pad.h"
#include "game.h"

int my_score = 0, oppo_score = 0;
char last_input = -1;

/* initial values, x, y to be set properly in init_game()*/
PAD my_pad = {0, 0, 0.};
PAD oppo_pad = {0, 0, 0.};
BALL ball = {0, 0, 0, 0};

void game_over_screen()
{
	clear_screen();

	char msg[MAX_MSG_LEN];
	int w = get_dev_width();
	int h = get_dev_height();

	sprintf((char *)msg, "Game Over. Score %d : %d", my_score, oppo_score);
	text_at((char *)msg, w/2 -strlen(msg)/2, h/2 -1);
	sprintf((char *)msg,
			"[SPACE] start, [%c] quit, [%c] left, [%c] stay, [%c] right",
			KEY_QUIT_GAME, KEY_GO_LEFT, KEY_STAY_PUT, KEY_GO_RIGHT);
	text_at((char *)msg, w/2 -strlen(msg)/2, h/2 +1);
	update_ball(&ball, 0.);
	update_pad(&my_pad, 0.);
	update_pad(&oppo_pad, 0.);
}

void init_game()
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

	// clear the whole screen
	clear_screen();

	my_pad.x = .5 * w;
	my_pad.y = h - PAD_HEIGHT;
	oppo_pad.x = .5 * w;
	oppo_pad.y = PAD_HEIGHT;
	ball.x = .5 * w;
	ball.y = .25 * h;

	game_over_screen();

	return;
}

void start_game()
{
	clear_screen();

	int w = get_dev_width();
	int h = get_dev_height();
	clear_screen();
	my_score = 0;
	oppo_score = 0;
	my_pad.x = .5 * w;
	my_pad.v = 0;
	my_pad.y = h - PAD_HEIGHT;
	oppo_pad.x = .5 * w;
	oppo_pad.v = 0;
	oppo_pad.y = PAD_HEIGHT;
	ball.x = .5 * w;
	ball.y = .25 * h;
	ball.vy = BALL_INIT_SPEED;
	ball.vx = 0;

	update_ball(&ball, 0);
	update_pad(&my_pad, 0);
	update_pad(&oppo_pad, 0);
}

bool is_over()
{
	return (fabs(ball.vy) <= 0 || my_score >= GAME_MAX_SCORE || oppo_score >= GAME_MAX_SCORE);
}

double update_game_and_pause(double elapsed_secs)
{
	int w = get_dev_width();
	int h = get_dev_height();
	double pause = 0;

	char msg[MAX_MSG_LEN];

	if (elapsed_secs <= 0)
		return pause;

	char c = key_pressed();
	if (c > 0)
	{
		last_input = c;
	}

	if (c == KEY_QUIT_GAME)
	{
		return END_OF_GAME_PAUSE;
	}

	switch (c)
	{
	case KEY_GO_LEFT:
		pad_go_left(&my_pad);
		break;
	case KEY_GO_RIGHT:
		pad_go_right(&my_pad);
		break;
	case KEY_STAY_PUT:
		pad_stop(&my_pad);
		break;
	default:
		break;
		// don't change pad's trajectory
	}
	update_pad(&my_pad, elapsed_secs);

	if (is_over())
	{
		if (c == KEY_START_GAME)
		{
			console_beep();
			start_game();
			elapsed_secs = 0; 
			pause = EXTENDED_PAUSE;
		}
		else
		{
			game_over_screen();
		}
	} 
	else
	{
		update_ball(&ball, elapsed_secs);

		if (ball.x < (oppo_pad.x - .4 * PAD_WIDTH))
			pad_go_left(&oppo_pad);
		else if (ball.x > (oppo_pad.x + .4 * PAD_WIDTH))
			pad_go_right(&oppo_pad);
		else
			pad_stop(&oppo_pad);
		
		update_pad(&oppo_pad, elapsed_secs);

		bounce_if_touching_left_wall(&ball);
		bounce_if_touching_right_wall(&ball);

		bounce_if_touching_pad(&ball, &oppo_pad);
		bounce_if_touching_pad(&ball, &my_pad);

		if (bounce_if_touching_top_wall(&ball))
		{
			my_score++;

			// force update and redraw all
			clear_screen();
			update_ball(&ball, 0);
			update_pad(&my_pad, 0);
			update_pad(&oppo_pad, 0);
			console_beep();
			pause = 1.0;
		}
		else if (bounce_if_touching_bottom_wall(&ball))
		{
			oppo_score++;

			// force update and redraw all
			clear_screen();
			update_ball(&ball, 0);
			update_pad(&my_pad, 0);
			update_pad(&oppo_pad, 0);
			console_beep();
			pause = 1.0;
		}
	}

	sprintf((char *)msg, "Score %d : %d", my_score, oppo_score);
	text_at((char *)msg, get_dev_width()/2 - strlen(msg)/2, 0);

	// uncomment only for debugging...
	sprintf((char *)msg, "Input: %c. Dim: %d x %d", last_input, w, h);
	text_at((char *)msg, w - strlen(msg) - 1, 0);
	sprintf((char *)msg, "Interval: %.9f", elapsed_secs);
	text_at((char *)msg, w - strlen(msg) - 1, h-1);

	// console_refresh();

	return pause;
}