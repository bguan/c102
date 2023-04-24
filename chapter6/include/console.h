#ifndef CONSOLE_H
#define CONSOLE_H

/**
 * The console represents the "screen" to draw in, and
 * gives you the key pressed without blocking execution thread.
 *
 * All coordinates i.e. x, y are given in NORMALIZED COORDINATES
 *  - (0.0, 0.0) represents the center of the screen
 *  - leftmost x is -0.5, rightmost is +0.5, the screen width is 1.0
 *  - topmost y is -0.5, bottom is +0.5, the screen height is 1.0
 * This allows for console of different device dimension, from
 * character based terminal of say 80 chars by 25 lines to
 * high resolution graphical window of thousands by thousands of dots
 *
 * Normalized coordinates are doubles. 
 * When used, device coordinates are unsigned ints.
 *
 * Using normalized coordinates and time unit of seconds in double,
 * velocity is also a double, e.g. vx of -1 means horizontal speed
 * traveling to the left can cross the screen in 1 second.
 */
typedef enum
{
	WHITE_ON_BLACK = 1,
	RED_ON_BLACK = 2,
	GREEN_ON_BLACK = 3,
	BLUE_ON_BLACK = 4,
	YELLOW_ON_BLACK = 5,
} CONS_COLOR;

/**
 * defines how text is aligned wrt to a position 
 */
typedef enum { TXT_LEFT, TXT_CENTER, TXT_RIGHT } CONS_TXT_ALIGN;

#define CONS_FONT_ASPECT 0.5 // width to height

#define NORM_MIN_DIFF 0.001 // threshhold for normalized coord diff

/**
 * @brief: initialize the console, i.e. the terminal 
 */
void init_console();

/**
 * @brief: end the console, i.e. the terminal, properly
 */
void end_console();

/**
 * @brief: get the console device horizontal resolution
 *
 * In the case of terminal, max chars per line
 */
unsigned int get_dev_width();

/**
 * @brief: get the console device vertical resolution
 *
 * In the case of terminal, max lines per screen
 */
unsigned int get_dev_height();

/**
 * @brief: get the console estimatedd aspect ratio i.e. width:height
 *
 * In the case of terminal, (estimated letter width:height) x COLS / LINES
 */
double get_dev_aspect();

/**
 * @brief: char corresponding to current key pressed, 0 if none
 */
int key_pressed();

/**
 * @brief: convert norm x to device x pos
 */
int to_dev_x(double norm_x);

/**
 * @brief: convert norm y to device y pos
 */
int to_dev_y(double norm_y);

/**
 * @brief: convert norm width to device width
 */
unsigned int to_dev_width(double norm_wth);

/**
 * @brief: convert norm height to device height
 */
unsigned int to_dev_height(double norm_hgt);

double bound_x(double norm_x);

double bound_y(double norm_y);

double bound_height(double norm_ht);

double bound_width(double norm_wth);

/**
 * @brief: draw text string starting at x, y for next round of render
 */
void text_at(char *str, double x, double y, CONS_COLOR c, CONS_TXT_ALIGN align);

/**
 * @brief: draw circle centered at x, y for next round of render
 */
void circle_at(double radius, double x, double y, CONS_COLOR c);

/**
 * @brief: draw width height rectangle centered at x, y for next render
 */
void rect_at(double width, double height, double x, double y, CONS_COLOR c);

/**
 * @brief: clear area from top left to bottom right for next render
 */
void clear_area(double left_x, double top_y, double right_x, double bot_y);

/**
 * @brief: sound an audible beep
 */
void console_beep();

#endif
