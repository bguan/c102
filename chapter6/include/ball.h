#ifndef BALL_H
#define BALL_H

#include <stdbool.h>

#include "console.h"

#define BALL_COLOR WHITE_ON_BLACK

typedef struct {
	double rad, x, y, vx, vy;
} BALL;

/**
 * Initialize a struct holding the states of a ball with its:
 * 
 *  - radius
 *  - x, y position in normalized coordinates
 *  - vx, vy velocity for both x & y axis in normalized coord/s 
*/
BALL* init_ball(double rad, double x, double y, double vx, double vy);

/**
 * Clear ball from screen and Free memory used by BALL 
 */
void dispose_ball(BALL* b);

/**
 * update BALL b's state given current pos and velocity after elapsed time.
 */
void update_ball(BALL* b, double elapse_secs);

/**
 * Update BALL b's state given current pos and velocity if its touching
 * or beyond the top most limit e.g. touching ceiling or a top paddle.
 * Note that side_spin will be applied to ball's X velocity, applicable
 * if touching a moving paddle, or wanting to inject a little randomness
 * even when just bouncing off the ceiling.
 * Return true only if touching or exceeding.
 */
bool bounce_if_touching_top(BALL* b, double top_y, double side_spin);


/**
 * Update BALL b's state given current pos and velocity if its touching
 * or beyond the bottom most limit e.g. touching floor or a bottom paddle.
 * Note that side_spin will be applied to ball's X velocity, applicable
 * if touching a moving paddle, or wanting to inject a little randomness
 * even when just bouncing off the floor.
 * Return true only if touching or exceeding.
 */
bool bounce_if_touching_bottom(BALL* b, double bot_y, double side_spin);


/**
 * Update BALL b's state given current pos and velocity if its touching
 * or beyond the left most limit e.g. touching left wall or a left paddle.
 * Note that top_spin will be applied to ball's y velocity, applicable
 * if touching a moving paddle, or wanting to inject a little randomness
 * even when just bouncing off the left wall.
 * Return true only if touching or exceeding.
 */
bool bounce_if_touching_left(BALL* b, double left_x, double top_spin);

/**
 * Update BALL b's state given current pos and velocity if its touching
 * or beyond the right most limit e.g. touching right wall or right paddle.
 * Note that top_spin will be applied to ball's y velocity, applicable
 * if touching a moving paddle, or wanting to inject a little randomness
 * even when just bouncing off the right wall.
 * Return true only if touching or exceeding.
 */
bool bounce_if_touching_right(BALL* b, double right_x, double top_spin);

#endif
