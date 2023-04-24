#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "game.h"

int main() {
	GAME* g = init_game();
	if (g == NULL)
		exit(1);

	clock_t tstmp = clock();
	for (;;)
	{
		clock_t now = clock();
		double elapsed_secs = (now - tstmp)/100000.;
		tstmp = now;
		bool running = update_game(g, elapsed_secs);
		if (!running)
			break;
	}
	dispose_game(g);
	return 0;
}
