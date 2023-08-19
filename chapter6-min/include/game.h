#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "console.h"
#include "ball.h"
#include "pad.h"

#define MIN_WIDTH 20
#define MIN_HEIGHT 20
#define KEY_START_GAME ' '
#define KEY_QUIT_GAME 'q'
#define KEY_GO_LEFT 'a'
#define KEY_STAY_PUT 's'
#define KEY_GO_RIGHT 'd'

#define EXTENDED_PAUSE 2.0 // a longer pause after a score
#define END_OF_GAME_PAUSE -1.0 // a special value to signify end of game

#define MAX_MSG_LEN 100
#define MSG_COLOR GREEN_ON_BLACK
#define GAME_MAX_SCORE 3

/**
 * @brief initialize game
 */
void init_game();

/**
 * @brief check if game is over
 * @return true iff game is over
 */
bool is_over();

/**
 * @brief move the state of the game by elapsed seconds, incl detecting key press, and drawing.
 *
 * @return the recommended amount of seconds to pause until next update.  If negative, quit game.
 */
double update_game_and_pause(double elapsed_secs);

#endif