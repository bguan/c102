#ifndef CONSOLE_H
#define CONSOLE_H

/** Useful macros for picking min or max of 2 values, and envorce upper lower bounds */
#define cmax(x, y) (((x) >= (y)) ? (x) : (y))
#define cmin(x, y) (((x) <= (y)) ? (x) : (y))
#define cbound(v, min, max) (cmax((min), cmin((max), (v))))

/**
 * The console represents the "screen" to draw in, and
 * gives you the key pressed without blocking execution thread.
 *
 * All coordinates i.e. x, y are given in Device COORDINATES in integers
 *  - (0.0, 0.0) represents the top left of the screen
 *  - (COLS-1, LINES-1) represents the bottom right of the screen
 */

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
 * @brief: char corresponding to current key pressed, 0 if none
 */
unsigned int key_pressed();

/**
 * @brief: draw text string starting at x, y 
 */
void text_at(char *str, unsigned int x, unsigned int y);

/**
 * @brief: draw dot at x, y
 */
void dot_at(unsigned int x, unsigned int y);

/**
 * @brief: draw width height rectangle centered at x, y
 */
void rect_at(unsigned int width, unsigned int height, unsigned int x, unsigned int y);

/**
 * @brief: clear whole screen
 */
void clear_screen();

/**
 * @brief: clear area from top left to bottom right
 */
void clear_area(unsigned int left_x, unsigned int top_y, unsigned int right_x, unsigned int bot_y);

/** marking console_refresh as weak to allow mocking */
void console_refresh();

/**
 * @brief: make a beep
 */
void console_beep();

#endif
