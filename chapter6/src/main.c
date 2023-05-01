#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include "game.h"

double get_tstmp()
{
	struct timespec tspec;
	clock_gettime(CLOCK_MONOTONIC, &tspec);
	return tspec.tv_nsec * 1e-9 + tspec.tv_sec;
}

void sleep_for_secs(double secs)
{
	for (; usleep(secs * 1e6) != 0; ) {}
}

int main() {
	GAME* g = init_game();
	if (g == NULL)
		exit(1);

	double tstmp = get_tstmp();
	double pause = 0;
	for (;;)
	{
		double now = get_tstmp();
		pause = update_game_and_pause(g, now - tstmp - floor(pause));
		if (pause < 0.0)
			break;

		tstmp = now;
		sleep_for_secs(pause);
	}
	dispose_game(g);
	return 0;
}
