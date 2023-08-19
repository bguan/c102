#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <strings.h>
#include "game.h"

#define GAME_THROTTLE 0.02 // default pause 20 millisec after every update so game don't consume too much CPU

double get_tstmp()
{
	struct timespec tspec;
	clock_gettime(CLOCK_MONOTONIC, &tspec);
	return tspec.tv_nsec * 1e-9 + tspec.tv_sec;
}

void sleep_for_secs(double secs)
{
	usleep(secs * 1e6);
}

int main() {
	init_game();
	double tstmp = get_tstmp();
	double pause = 0;
	for (;;)
	{
		double now = get_tstmp();
		pause = update_game_and_pause(now - tstmp - pause);
		if (pause < 0.0)
			break;
		tstmp = now;
		sleep_for_secs(pause + GAME_THROTTLE);
	}
	return 0;
}