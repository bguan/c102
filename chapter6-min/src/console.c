#include <ncurses.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "console.h"

/** marking init_console as weak to allow mocking */
__attribute__((weak)) 
void init_console()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
}

/** marking end_console as weak to allow mocking */
__attribute__((weak)) void end_console() {
	endwin();
}

/** marking get_dev_width as weak to allow mocking */
__attribute__((weak)) unsigned int get_dev_width()
{
	return COLS;
}

/** marking get_dev_height as weak to allow mocking */
__attribute__((weak)) unsigned int get_dev_height()
{
	return LINES;
}

/** marking key_pressed as weak to allow mocking */
__attribute__((weak)) unsigned int key_pressed()
{
	return tolower(wgetch(stdscr));
}
/** marking text_at as weak to allow mocking */
__attribute__((weak)) void text_at(char *str, unsigned int x, unsigned int y)
{
	double w = strnlen(str, COLS - 1);
	x = cbound(x, 0, COLS - 1 - w);
	y = cbound(y, 0, LINES - 1);
	mvaddstr(y, x, str);
}

void repeat_char(char *buffer, char c, unsigned int num)
{
	for (int i = 0; i < num; i++)
	{
		buffer[i] = c;
	}
	buffer[num] = '\0';
}

/** marking rect_at as weak to allow mocking */
__attribute__((weak)) void dot_at(unsigned int x, unsigned int y)
{
	x = cbound(x, 0, COLS - 1);
	y = cbound(y, 0, LINES - 1);

	text_at("@", x, y);
}

/** marking rect_at as weak to allow mocking */
__attribute__((weak)) void rect_at(unsigned int w, unsigned int h, unsigned int x, unsigned int y)
{
	w = cbound(w, 0, COLS - 1);
	h = cbound(h, 0, LINES - 1);
	double half_w = .5 * w;
	double half_h = .5 * h;
	x = cbound(x, half_w, COLS - 1 - half_w);
	y = cbound(y, half_h, LINES - 1 - half_h);

	char buffer[get_dev_width() + 1];
	repeat_char((char*)&buffer, '#', w);
	for (int dy = y - half_h; dy < y + half_h; dy++)
	{
		text_at(buffer, x - half_w, dy);
	}
}

/** marking clear_area as weak to allow mocking */
__attribute__((weak)) void clear_screen()
{
	clear();
}

/** marking clear_area as weak to allow mocking */
__attribute__((weak)) void clear_area(unsigned int left_x, unsigned int top_y, unsigned int right_x, unsigned int bot_y)
{
	left_x = cmax(0, left_x);
	right_x = cmin(COLS - 1, right_x);
	top_y = cmax(0, top_y);
	bot_y = cmin(LINES - 1, bot_y);

	char buffer[get_dev_width() + 1];
	repeat_char((char *)&buffer, ' ', right_x - left_x);
	for (int y = top_y; y < bot_y; y++)
	{
		mvprintw(y, left_x, "%s", buffer);
	}
}

/** marking console_refresh as weak to allow mocking */
__attribute__((weak)) void console_refresh()
{
	refresh();
}

/** marking console_beep as weak to allow mocking */
__attribute__((weak)) void console_beep()
{
	beep();
}