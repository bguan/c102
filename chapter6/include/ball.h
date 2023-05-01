#ifndef BALL_H
#define BALL_H

#include <stdbool.h>

#include "console.h"
#include "pad.h"

#define BALL_COLOR WHITE_ON_BLACK
#define SIDE_SPIN_FACTOR 0.2

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
 * or beyond the boundary of the pad e.g. touching bottom line of top paddle.
 * Note that side_spin will be applied to ball's X velocity, applicable
 * if touching a moving paddle, or wanting to inject a little randomness
 * even when just bouncing off the ceiling.
 * Return true only if touching or exceeding.
 */
bool bounce_if_touching_pad(BALL* b, PAD* p);

/**
 * Update BALL b's state given current pos and velocity if its touching
 * or beyond the top wall, return true only if touching or exceeding.
 */
bool bounce_if_touching_top_wall(BALL* b, double top_y);

/**
 * Update BALL b's state given current pos and velocity if its touching
 * or beyond the bottom wall, return true only if touching or exceeding.
 */
bool bounce_if_touching_bottom_wall(BALL* b, double bot_y);

/**
 * Update BALL b's state given current pos and velocity if its touching
 * or beyond the left wall, return true only if touching or exceeding.
 */
bool bounce_if_touching_left_wall(BALL* b, double left_x);

/**
 * Update BALL b's state given current pos and velocity if its touching
 * or beyond the right wall, return true only if touching or exceeding.
 */
bool bounce_if_touching_right_wall(BALL* b, double right_x);

#endif
