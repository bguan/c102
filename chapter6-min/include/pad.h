#ifndef PAD_H
#define PAD_H

#include "console.h"

#define PAD_WIDTH 10
#define PAD_HEIGHT 2
#define PAD_SPEED 40 // 40 squares per second

/**
 * Data structure holding the pad's state with its:
 *  - x, y position in device coordinates
 *  - v velocity in x axis in device coord/s 
*/
typedef struct {
	double x, y, v;  // position x,y and velocity (only in x axis)
} PAD;

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
