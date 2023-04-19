#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include "game.h"
#include "console.h"

#define MIN_WIDTH 10
#define MIN_HEIGHT 10

int main() {
	init_console();

	int w = get_dev_width();
	int h = get_dev_height();

	if (w < MIN_WIDTH || h < MIN_HEIGHT)
	{
		end_console();
		printf(
			"Console Width Height %d x %d less than min %d x %d\n",
			w, h, MIN_WIDTH, MIN_HEIGHT
		);
		return 1;
	}

	text_at("Hello Ball!", 0., 0.45, BLUE_ON_BLACK, CENTER);

	float rad = 0.0;
	for (;;)
	{
		char c = key_pressed();
		if (c == 'q' || c == 'Q') break;

		if (rad < 0.001) {
			clear_area(-.4, -.4, .4, .4);
		} 
		else
		{
			circle_at(rad, 0.0, 0.0, GREEN_ON_BLACK);
			rect_at(rad/2, rad/4, 0.0, 0.0, RED_ON_BLACK);
		}
		rad = (rad > 0.4 ? 0.0 : rad + 0.00001);
	}
	end_console();
	printf("Console Width Height is %d x %d\n", w, h);

	/*
		GAME* g = init_game();
		start_game(g);

		clock_t tstmp = clock();
		while (!is_over(g))
		{
			char c = key_pressed();
			clock_t now = clock();
			float elapsed_secs = (now - tstmp)/CLOCKS_PER_SEC;
			tstmp = now;
			update_game(g, c, elapsed_secs);
			render();
		}
		free_game(g);
	*/
	return 0;
}
