#include "CppUTest/CommandLineTestRunner.h"
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdarg.h>
#include <ncurses.h>

extern "C"
{
	#include "../include/console.h"
	#include "../include/game.h"
}

/**
 * Mock console is 100x25 in dimension 
 */
static const int _MOCK_LINES = 25;
static const int _MOCK_COLS = 100;
static const double _MIN_DIFF = 0.001;

// Mock console.h init_console()
void init_console()
{
	mock().actualCall("init_console");
	LINES = _MOCK_LINES;
	COLS = _MOCK_COLS;
}

// Mock console.h end_console()
void end_console()
{
	mock().actualCall("end_console");
}

// Mock console.h rect_at()
void text_at(char* txt, double x, double y, CONS_COLOR c, CONS_TXT_ALIGN align)
{
	mock().actualCall("text_at")
		.withStringParameter("txt", txt)
		.withDoubleParameter("x", x)
		.withDoubleParameter("y", y)
		.withIntParameter("c", c)
		.withIntParameter("align", align);
}

// Mock console.h clear_area()
void clear_area(double left_x, double top_y, double right_x, double bot_y)
{
	mock().actualCall("clear_area")
		.withDoubleParameter("left_x", left_x)
		.withDoubleParameter("top_y", top_y)
		.withDoubleParameter("right_x", right_x)
		.withDoubleParameter("bot_y", bot_y);
}

// Mock console.h circle_at()
void circle_at(double rad, double x, double y, CONS_COLOR c)
{
	mock().actualCall("circle_at")
		.withDoubleParameter("rad", rad)
		.withDoubleParameter("x", x)
		.withDoubleParameter("y", y)
		.withIntParameter("c", c);
}

// Mock console.h rect_at()
void rect_at(double w, double h, double x, double y, CONS_COLOR c)
{
	mock().actualCall("rect_at")
		.withDoubleParameter("w", w)
		.withDoubleParameter("h", h)
		.withDoubleParameter("x", x)
		.withDoubleParameter("y", y)
		.withIntParameter("c", c);
}

// Mock console.h key_pressed()
int _next_key = 0; // set this global variable to control what key_pressed() returns
int key_pressed()
{
	mock().actualCall("key_pressed");
	return(_next_key);
}


TEST_GROUP(GameTests){
	void setup()
	{
		// make sure mocks are disabled by default, i.e. tests that use mock must explicitly enable it
		mock().disable();
	}

	void teardown()
	{
		mock().clear();
	}
};

TEST(GameTests, test_init_game)
{
	mock().enable();

	mock().expectOneCall("init_console");

	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", -.5)
		.withDoubleParameter("top_y", -.5)
		.withDoubleParameter("right_x", .5)
		.withDoubleParameter("bot_y", .5);

	mock().expectOneCall("text_at")
		.withStringParameter("txt", "Game Over. Score 0 : 0")
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", -.5)
		.withIntParameter("c", MSG_COLOR)
		.withIntParameter("align", TXT_CENTER);

	mock().expectOneCall("text_at")
		.withStringParameter("txt", "[SPACE] start, [q] quit, [a] left, [s] stay, [d] right")
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", .45)
		.withIntParameter("c", MSG_COLOR)
		.withIntParameter("align", TXT_CENTER);

	// ball draw
	mock().expectOneCall("circle_at")
		.withDoubleParameter("rad", BALL_SIZE)
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", 0.)
		.withIntParameter("c", BALL_COLOR);

	// top pad draw
	mock().expectOneCall("rect_at")
		.withDoubleParameter("w", PAD_WIDTH)
		.withDoubleParameter("h", PAD_HEIGHT)
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", -0.4)
		.withIntParameter("c", TOP_PAD_COLOR);

	// bottom pad draw
	mock().expectOneCall("rect_at")
		.withDoubleParameter("w", PAD_WIDTH)
		.withDoubleParameter("h", PAD_HEIGHT)
		.withDoubleParameter("x", 0)
		.withDoubleParameter("y", 0.4)
		.withIntParameter("c", BOT_PAD_COLOR);

	GAME* g = init_game();
	CHECK(g != NULL);

	CHECK_EQUAL(0, g->my_score);
	CHECK_EQUAL(0, g->oppo_score);
	CHECK(g->ball != NULL);
	CHECK(g->my_pad != NULL);
	CHECK(g->oppo_pad != NULL);

	mock().checkExpectations();
	mock().disable();

	dispose_game(g);
}

TEST(GameTests, test_update_game_and_pause_returns_negative_when_KEY_QUIT_GAME_pressed)
{
	GAME* g = init_game();
	CHECK(g != NULL);
	mock().enable();

	_next_key = KEY_QUIT_GAME;
	mock().expectOneCall("key_pressed").andReturnValue(KEY_QUIT_GAME);

	double pause_duration_or_quit = update_game_and_pause(g, 0.1);
	CHECK(pause_duration_or_quit < 0.0);

	mock().checkExpectations();
	mock().disable();

	dispose_game(g);
}

TEST(GameTests, test_update_game_and_pause_returns_default_pause)
{
	GAME* g = init_game();
	CHECK(g != NULL);
	mock().enable();

	_next_key = 0;
	mock().expectOneCall("key_pressed").andReturnValue(DEFAULT_PAUSE);

	mock().expectOneCall("text_at")
		.withStringParameter("txt", "Game Over. Score 0 : 0")
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", -.5)
		.withIntParameter("c", MSG_COLOR)
		.withIntParameter("align", TXT_CENTER);

	mock().expectOneCall("text_at")
		.withStringParameter("txt", "[SPACE] start, [q] quit, [a] left, [s] stay, [d] right")
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", .45)
		.withIntParameter("c", MSG_COLOR)
		.withIntParameter("align", TXT_CENTER);

	double pause_duration_or_quit = update_game_and_pause(g, 0.01);

	CHECK(is_over(g));

	DOUBLES_EQUAL(pause_duration_or_quit, DEFAULT_PAUSE, _MIN_DIFF);

	mock().checkExpectations();
	mock().disable();

	dispose_game(g);
}

TEST(GameTests, test_update_game_starts_game_when_start_key_pressed_returns_extended_pause)
{
	GAME* g = init_game();
	CHECK(g != NULL);
	mock().enable();

	_next_key = KEY_START_GAME;
	mock().expectOneCall("key_pressed").andReturnValue(KEY_START_GAME);

	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", -.5)
		.withDoubleParameter("top_y", -.5)
		.withDoubleParameter("right_x", .5)
		.withDoubleParameter("bot_y", .5);

	CHECK(is_over(g));
	DOUBLES_EQUAL(g->ball->x, 0., _MIN_DIFF);
	DOUBLES_EQUAL(g->ball->y, 0., _MIN_DIFF);
	DOUBLES_EQUAL(g->ball->vx, 0., _MIN_DIFF);
	DOUBLES_EQUAL(g->ball->vy, 0., _MIN_DIFF);

	double pause_duration_or_quit = update_game_and_pause(g, 0.1);

	CHECK(!is_over(g));

	// since game just started there is NO update
	DOUBLES_EQUAL(g->ball->vx, 0., _MIN_DIFF);
	CHECK(g->ball->vy > 0);
	DOUBLES_EQUAL(g->ball->x, 0., _MIN_DIFF);
	DOUBLES_EQUAL(g->ball->y, 0., _MIN_DIFF);

	DOUBLES_EQUAL(pause_duration_or_quit, EXTENDED_PAUSE, _MIN_DIFF);

	mock().checkExpectations();
	mock().disable();

	dispose_game(g);
}

TEST(GameTests, test_update_running_game_no_key_pressed_update_ball_pads_returns_default_pause)
{
	GAME* g = init_game();
	CHECK(g != NULL);

	_next_key = KEY_START_GAME;
	update_game_and_pause(g, 0.1);

	CHECK(!is_over(g));

	mock().enable();

	_next_key = 0;
	mock().expectOneCall("key_pressed").andReturnValue(0);

	// ball clear
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", g->ball->x - 2*g->ball->rad)
		.withDoubleParameter("top_y", g->ball->y - 2*g->ball->rad)
		.withDoubleParameter("right_x", g->ball->x + 2*g->ball->rad)
		.withDoubleParameter("bot_y", g->ball->y + 2*g->ball->rad);

	// ball draw
	mock().expectOneCall("circle_at")
		.withDoubleParameter("rad", g->ball->rad)
		.withDoubleParameter("x", g->ball->x)
		.withDoubleParameter("y", g->ball->y + 0.1 * g->ball->vy)
		.withIntParameter("c", BALL_COLOR);

	// top pad clear
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", g->oppo_pad->x - g->oppo_pad->w)
		.withDoubleParameter("top_y", g->oppo_pad->y - g->oppo_pad->h)
		.withDoubleParameter("right_x", g->oppo_pad->x + g->oppo_pad->w)
		.withDoubleParameter("bot_y", g->oppo_pad->y + g->oppo_pad->h);

	// top pad draw
	mock().expectOneCall("rect_at")
		.withDoubleParameter("w", g->oppo_pad->w)
		.withDoubleParameter("h", g->oppo_pad->h)
		.withDoubleParameter("x", g->oppo_pad->x)
		.withDoubleParameter("y", g->oppo_pad->y)
		.withIntParameter("c", TOP_PAD_COLOR);

	// bottom pad clear
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", g->my_pad->x - g->my_pad->w)
		.withDoubleParameter("top_y", g->my_pad->y - g->my_pad->h)
		.withDoubleParameter("right_x", g->my_pad->x + g->my_pad->w)
		.withDoubleParameter("bot_y", g->my_pad->y + g->my_pad->h);

	// bottom pad draw
	mock().expectOneCall("rect_at")
		.withDoubleParameter("w", g->my_pad->w)
		.withDoubleParameter("h", g->my_pad->h)
		.withDoubleParameter("x", g->my_pad->x)
		.withDoubleParameter("y", g->my_pad->y)
		.withIntParameter("c", BOT_PAD_COLOR);

	// Score updated
	mock().expectOneCall("text_at")
		.withStringParameter("txt", "Score 0 : 0")
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", -.5)
		.withIntParameter("c", MSG_COLOR)
		.withIntParameter("align", TXT_CENTER);

	double pause_duration_or_quit = update_game_and_pause(g, 0.1);

	// ball should have moved
	DOUBLES_EQUAL(g->ball->vx, 0., _MIN_DIFF);
	CHECK(g->ball->vy > 0);
	DOUBLES_EQUAL(g->ball->x, 0., _MIN_DIFF);
	DOUBLES_EQUAL(g->ball->y, 0.1 * g->ball->vy, _MIN_DIFF);

	DOUBLES_EQUAL(pause_duration_or_quit, DEFAULT_PAUSE, _MIN_DIFF);
	
	mock().checkExpectations();
	mock().disable();

	dispose_game(g);
}

TEST(GameTests, test_dispose_game_calls_end_console)
{
	GAME* g = init_game();
	CHECK(g != NULL);

	mock().enable();

	// clear ball area
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", g->ball->x - g->ball->rad)
		.withDoubleParameter("top_y", g->ball->y - g->ball->rad)
		.withDoubleParameter("right_x", g->ball->x + g->ball->rad)
		.withDoubleParameter("bot_y", g->ball->y + g->ball->rad);

	// clear player pad area
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", g->my_pad->x - g->my_pad->w /2)
		.withDoubleParameter("top_y", g->my_pad->y - g->my_pad->h /2)
		.withDoubleParameter("right_x", g->my_pad->x + g->my_pad->w /2)
		.withDoubleParameter("bot_y", g->my_pad->y + g->my_pad->h /2);

	// clear computer pad area
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", g->oppo_pad->x - g->oppo_pad->w /2)
		.withDoubleParameter("top_y", g->oppo_pad->y - g->oppo_pad->h /2)
		.withDoubleParameter("right_x", g->oppo_pad->x + g->oppo_pad->w /2)
		.withDoubleParameter("bot_y", g->oppo_pad->y + g->oppo_pad->h /2);

	mock().expectOneCall("end_console");

	dispose_game(g);

	mock().checkExpectations();
	mock().disable();
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
