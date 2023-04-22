#ifndef PAD_H
#define PAD_H

#include "console.h"

#define PAD_SPEED 0.5

#define BOT_PAD_COLOR BLUE_ON_BLACK
#define TOP_PAD_COLOR RED_ON_BLACK

/**
 * Data structure holding the pad's state with its:
 *  - w, h width and height
 *  - x, y position in normalized coordinates
 *  - v velocity in x axis in normalized coord/s 
*/
typedef struct {
	double w, h, x, y, v;
} PAD;

/**
 * Initialize a paddle
 */
PAD* init_pad(double w, double h, double x, double y, double v);

/*
 * Clear screen from pad and free memory used by Pad p.
 */
void dispose_pad(PAD* p);

/*
 * update pad based on current pos and velocity after elapsed secs
 */
void update_pad(PAD* p, double elapsed_secs);

/*
 * changed velocity of pad to 0
 */
void pad_stop(PAD* p);

/*
 * changed velocity of pad to go left, only if dir is LEFT_RIGHT
 */
void pad_go_left(PAD* p);

/*
 * changed velocity of pad to go right, only if dir is LEFT_RIGHT
 */
void pad_go_right(PAD* p);

#endif
