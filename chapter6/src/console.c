#include <ncurses.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "console.h"

static int _console_width = 0;
static int _console_height = 0;
static double _console_aspect = 0.0;

void init_console()
{
	initscr();
	_console_width = COLS;
	_console_height = LINES;
	_console_aspect = CONS_FONT_ASPECT * COLS / LINES;

	start_color();

	// initialize 3 color schemes: Circle, Rectangle, Text
	init_pair(RED_ON_BLACK, COLOR_BLACK, COLOR_RED);
	init_pair(GREEN_ON_BLACK, COLOR_BLACK, COLOR_GREEN);
	init_pair(BLUE_ON_BLACK, COLOR_BLACK, COLOR_BLUE);
	init_pair(WHITE_ON_BLACK, COLOR_BLACK, COLOR_WHITE);
	init_pair(YELLOW_ON_BLACK, COLOR_BLACK, COLOR_YELLOW);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
}

void end_console() {
	endwin();
}

unsigned int get_dev_width()
{
	return _console_width;
}

unsigned int get_dev_height()
{
	return _console_height;
}

double get_dev_aspect()
{
	return _console_aspect;
}

int key_pressed()
{
	return tolower(wgetch(stdscr));
}

double bound_x(double norm_x)
{
	return fmax(-0.5, fmin(norm_x, 0.5));
}

double bound_y(double norm_y)
{
	return fmax(-0.5, fmin(norm_y, 0.5));
}

double bound_height(double norm_ht)
{
	return fmax(-0.0, fmin(norm_ht, 1.0));
}

double bound_width(double norm_wth)
{
	return fmax(-0.0, fmin(norm_wth, 1.0));
}

int to_dev_x(double norm_x)
{
	double dev_x = (bound_x(norm_x) + 0.5) * _console_width;
	return round(dev_x);
}

int to_dev_y(double norm_y)
{
	double dev_y = (bound_y(norm_y) + 0.5) * _console_height;
	return round(dev_y);
}

unsigned int to_dev_width(double norm_x)
{
	return round(bound_width(norm_x) * _console_width);
}

unsigned int to_dev_height(double norm_y)
{
	return round(bound_height(norm_y) * _console_height);
}

/** marking text_area as weak to allow mocking */
__attribute__((weak)) 
void text_at(char *str, double x, double y, CONS_COLOR c, CONS_TXT_ALIGN align)
{
	attron(A_REVERSE | COLOR_PAIR(c));
	int dev_y = to_dev_y(y);
	int dev_x = to_dev_x(x);
	int len = strlen(str);
	if (align == TXT_RIGHT)
		dev_x -= len;
	else if (align == TXT_CENTER)
		dev_x -= len / 2;
	mvprintw(dev_y, dev_x, "%s", str);
	attroff(A_REVERSE | COLOR_PAIR(c));
}

void repeat_char(char * buffer, char c, int num)
{
	for (int i = 0; i < num; i++)
	{
		buffer[i] = c;
	}
	buffer[num] = '\0';
}

/** marking circle_at as weak to allow mocking */
__attribute__((weak))
void circle_at(double radius, double x, double y, CONS_COLOR c)
{
	int rad_x = to_dev_width(radius) / _console_aspect;
	int rad_y = to_dev_height(radius);
	int dev_x = to_dev_x(x);
	int dev_y = to_dev_y(y);

	double a_squared = rad_x * rad_x;
	double b_squared = rad_y * rad_y;
	bool too_small = rad_x < 3 || rad_y < 3;
	char buffer[_console_width + 1];
	attron(COLOR_PAIR(c));
	for (int dy = -rad_y; dy < rad_y; dy++)
	{
		if (too_small)
		{
			// don't bother calculating, just draw rectangle
			repeat_char((char *)&buffer, ' ', rad_x < 1 ? 1 : rad_x * 2);
			mvprintw(dev_y+dy, dev_x - rad_x, "%s", buffer);
		}
		else
		{
			// using equation for ellipse (x^2)/(a^2) + (y^2)/(b^2) = 1
			// => x^2 = (a^2 * (1 - (y^2)/(b^2)))

			double dx_squared = a_squared * (1.0 - (dy * dy / b_squared));
			if (dx_squared > 0)
			{
				int dx = sqrt(dx_squared);
				repeat_char((char *)&buffer, ' ', dx < 1 ? 1 : dx * 2);
				mvprintw(dev_y + dy, dev_x - dx, "%s", buffer);
			}
		}
	}
	attroff(COLOR_PAIR(c));
}

/** marking rect_at as weak to allow mocking */
__attribute__((weak))
void rect_at(double width, double height, double x, double y, CONS_COLOR c)
{
	int dev_x = to_dev_x(bound_x(x));
	int dev_y = to_dev_y(bound_y(y));
	int dev_w = to_dev_width(bound_width(width));
	int dev_h = to_dev_height(bound_height(height));
	char buffer[_console_width + 1];
	repeat_char((char*)&buffer, ' ', dev_w);
	attron(COLOR_PAIR(c));
	for (
		int dy = dev_y - floor(dev_h / 2.0); 
		dy < dev_y + ceil(dev_h / 2.0); 
		dy++
	)
	{
		mvprintw(dy, dev_x-dev_w/2, "%s", buffer);
	}
	attroff(COLOR_PAIR(c));
}

/** marking console_beep as weak to allow mocking */
__attribute__((weak)) 
void console_beep()
{
	beep();
}

/** marking clear_area as weak to allow mocking */
__attribute__((weak))
void clear_area(double left_x, double top_y, double right_x, double bot_y)
{
	int dev_left_x = to_dev_x(bound_x(left_x));
	int dev_right_x = to_dev_x(bound_x(right_x));

	char buffer[_console_width + 1];
	repeat_char((char *)&buffer, ' ', dev_right_x - dev_left_x);

	int dev_top_y = to_dev_y(bound_y(top_y));
	int dev_bot_y = to_dev_y(bound_y(bot_y));
	for (int y = dev_top_y; y < dev_bot_y; y++)
	{
		mvprintw(y, dev_left_x, "%s", buffer);
	}
}

void console_refresh()
{
	refresh();
}