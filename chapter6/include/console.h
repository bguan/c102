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
 * Normalized coordinates are floats. 
 * When used, device coordinates are unsigned ints.
 *
 * Using normalized coordinates and time unit of seconds in float,
 * velocity is also a float, e.g. vx of -1 means horizontal speed
 * traveling to the left can cross the screen in 1 second.
 */

#define CONSOLE_FONT_ASPECT 0.5 // width to height

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
int get_dev_width();

/**
 * @brief: get the console device vertical resolution
 *
 * In the case of terminal, max lines per screen
 */
int get_dev_height();

/**
 * @brief: get the console estimatedd aspect ratio i.e. width:height
 *
 * In the case of terminal, (estimated letter width:height) x COLS / LINES
 */
float get_dev_aspect();

/**
 * @brief: char corresponding to current key pressed, 0 if none
 */
int key_pressed();

/**
 * @brief: convert norm x to device x pos
 */
int to_dev_x(float norm_x);

/**
 * @brief: convert norm y to device y pos
 */
int to_dev_y(float norm_y);

/**
 * @brief: convert norm width to device width
 */
int to_dev_width(float norm_x);

/**
 * @brief: convert norm height to device x height
 */
int to_dev_height(float norm_y);

/**
 * @brief: draw text string starting at x, y for next round of render
 */
void text_at(char *str, float x, float y);

/**
 * @brief: draw circle centered at x, y for next round of render
 */
void circle_at(float radius, float x, float y);

/**
 * @brief: draw width height rectangle centered at x, y for next render
 */
void rect_at(float width, float height, float x, float y);

/**
 * @brief: clear area from top left to bottom right for next render
 */
void clear_area(float left_x, float top_y, float right_x, float bot_y);

#endif
