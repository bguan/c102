#include <ncurses.h>
#include <math.h>
#include "console.h"

static int _console_width = 0;
static int _console_height = 0;
static float _console_aspect = 0.0;

static const int _CIRCLE_COLOR = 1;
static const int _RECT_COLOR = 2;
static const int _TEXT_COLOR = 3;

void init_console()
{
	initscr();
	_console_width = COLS;
	_console_height = LINES;
	_console_aspect = CONSOLE_FONT_ASPECT * COLS / LINES;

	start_color();

	// initialize 3 color schemes: Circle, Rectangle, Text
	init_pair(_CIRCLE_COLOR, COLOR_BLACK, COLOR_RED);
	init_pair(_RECT_COLOR, COLOR_BLACK, COLOR_GREEN);
	init_pair(_TEXT_COLOR, COLOR_BLUE, COLOR_BLACK);

	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
}

void end_console() {
	endwin();
}

int get_dev_width()
{
	return _console_width;
}

int get_dev_height()
{
	return _console_height;
}

float get_dev_aspect()
{
	return _console_aspect;
}

int key_pressed()
{
	return wgetch(stdscr);
}

float bound_x(float norm_x)
{
	return fmax(-0.5, fmin(norm_x, 0.5));
}

float bound_y(float norm_y)
{
	return fmax(-0.5, fmin(norm_y, 0.5));
}

float bound_height(float norm_ht)
{
	return fmax(-0.0, fmin(norm_ht, 1.0));
}

float bound_width(float norm_wth)
{
	return fmax(-0.0, fmin(norm_wth, 1.0));
}

int to_dev_x(float norm_x)
{
	return (bound_x(norm_x) + 0.5) * _console_width;
}

int to_dev_y(float norm_y)
{
	return (bound_y(norm_y) + 0.5) * _console_height;
}

int to_dev_width(float norm_x)
{
	return bound_width(norm_x) * _console_width;
}

int to_dev_height(float norm_y)
{
	return bound_height(norm_y) * _console_height;
}

void text_at(char* str, float x, float y)
{
	attron(COLOR_PAIR(_TEXT_COLOR));
	mvprintw(to_dev_y(y), to_dev_x(x), "%s", str);
	attroff(COLOR_PAIR(_TEXT_COLOR));
}

void repeat_char(char * buffer, char c, int num)
{
	for (int i = 0; i < num; i++)
	{
		buffer[i] = c;
	}
	buffer[num] = '\0';
}

void circle_at(float radius, float x, float y)
{
	int rad_x = to_dev_width(radius) / _console_aspect;
	int rad_y = to_dev_height(radius);
	int dev_x = to_dev_x(x);
	int dev_y = to_dev_y(y);

	float a_squared = rad_x * rad_x;
	float b_squared = rad_y * rad_y;
	bool too_small = rad_x < 3 || rad_y < 3;
	char buffer[_console_width + 1];
	attron(COLOR_PAIR(_CIRCLE_COLOR));
	for (int dy = -rad_y; dy <= rad_y; dy++)
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

			float dx_squared = a_squared * (1.0 - (dy * dy / b_squared));
			if (dx_squared > 0)
			{
				int dx = sqrt(dx_squared);
				repeat_char((char *)&buffer, ' ', dx < 1 ? 1 : dx * 2);
				mvprintw(dev_y + dy, dev_x - dx, "%s", buffer);
			}
		}
	}
	attroff(COLOR_PAIR(_CIRCLE_COLOR));
}

void rect_at(float width, float height, float x, float y)
{
	int dev_x = to_dev_x(bound_x(x));
	int dev_y = to_dev_y(bound_y(y));
	int dev_w = to_dev_width(bound_width(width));
	int dev_h = to_dev_height(bound_height(height));
	char buffer[_console_width + 1];
	attron(COLOR_PAIR(_RECT_COLOR));
	for (int dy = dev_y - dev_h / 2; dy <= dev_y + dev_h / 2; dy++)
	{
		repeat_char((char*)&buffer, ' ', dev_w);
		mvprintw(dy, dev_x-dev_w/2, "%s", buffer);
	}
	attroff(COLOR_PAIR(_RECT_COLOR));
}

void clear_area(float left_x, float top_y, float right_x, float bot_y)
{
	int dev_left_x = to_dev_x(bound_x(left_x));
	int dev_right_x = to_dev_x(bound_x(right_x));

	char buffer[_console_width + 1];
	repeat_char((char *)&buffer, ' ', dev_right_x - dev_left_x);

	int dev_top_y = to_dev_y(bound_y(top_y));
	int dev_bot_y = to_dev_y(bound_y(bot_y));
	for (int y = dev_top_y; y <= dev_bot_y; y++)
	{
		mvprintw(y, dev_left_x, "%s", buffer);
	}
}
