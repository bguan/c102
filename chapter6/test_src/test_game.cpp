#include "CppUTest/CommandLineTestRunner.h"
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdarg.h>

extern "C"
{
	#include "../include/console.h"
	#include "../include/game.h"
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

TEST(GameTests, test_update_game_and_pause)
{
	GAME* g = init_game();
	CHECK(g != NULL);
	dispose_game(g);
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
