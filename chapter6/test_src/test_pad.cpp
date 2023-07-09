#include "CppUTest/CommandLineTestRunner.h"
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdarg.h>

extern "C"
{
	#include "../include/console.h"
	#include "../include/pad.h"
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

// Mock console.h clear_area()
void clear_area(double left_x, double top_y, double right_x, double bot_y)
{
	mock().actualCall("clear_area")
		.withDoubleParameter("left_x", left_x)
		.withDoubleParameter("top_y", top_y)
		.withDoubleParameter("right_x", right_x)
		.withDoubleParameter("bot_y", bot_y);
}

TEST_GROUP(PadTests){
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

TEST(PadTests, test_init_pad_calls_rect_at)
{
	mock().enable();
	mock().expectOneCall("rect_at")
		.withDoubleParameter("w", .2)
		.withDoubleParameter("h", .1)
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", -.4)
		.withIntParameter("c", TOP_PAD_COLOR);
		
	PAD* p = init_pad(.2, .1, 0., -.4, 0.);
	CHECK(p != NULL);

	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0., p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0., p->v, NORM_MIN_DIFF);

	mock().checkExpectations();
	mock().disable();

	dispose_pad(p);
}

TEST(PadTests, test_dispose_pad_calls_clear_area)
{
	PAD* p = init_pad(.2, .1, 0., .4, 0.);

	mock().enable();
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", -.1)
		.withDoubleParameter("top_y", .35)
		.withDoubleParameter("right_x", .1)
		.withDoubleParameter("bot_y", .45);
		
	dispose_pad(p);

	mock().checkExpectations();
	mock().disable();
}

TEST(PadTests, test_update_pad_stationary_stays_same_spot)
{
	PAD* p = init_pad(.2, .1, -.4, .4, 0.);

	// check initial state is as expected
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0., p->v, NORM_MIN_DIFF);

	mock().enable();
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", p->x - p->w)
		.withDoubleParameter("top_y", p->y - p->h)
		.withDoubleParameter("right_x", p->x + p->w)
		.withDoubleParameter("bot_y", p->y + p->h);

	mock().expectOneCall("rect_at")
		.withDoubleParameter("w", p->w)
		.withDoubleParameter("h", p->h)
		.withDoubleParameter("x", p->x)
		.withDoubleParameter("y", p->y)
		.withIntParameter("c", BOT_PAD_COLOR);

	// 100 secs elapsed, update pad state
	update_pad(p, 100.);

	// check no change to state
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0., p->v, NORM_MIN_DIFF);

	mock().checkExpectations();
	mock().disable();

	dispose_pad(p);
}

TEST(PadTests, test_update_pad_moved_to_expected_spot)
{
	PAD* p = init_pad(.1, .1, 0., .4, PAD_SPEED);

	// check initial state is as expected
	DOUBLES_EQUAL(.1, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0., p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(PAD_SPEED, p->v, NORM_MIN_DIFF);

	mock().enable();

	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", p->x - p->w)
		.withDoubleParameter("top_y", p->y - p->h)
		.withDoubleParameter("right_x", p->x + p->w)
		.withDoubleParameter("bot_y", p->y + p->h);

	mock().expectOneCall("rect_at")
		.withDoubleParameter("w", p->w)
		.withDoubleParameter("h", p->h)
		.withDoubleParameter("x", PAD_SPEED)
		.withDoubleParameter("y", p->y)
		.withIntParameter("c", BOT_PAD_COLOR);

	update_pad(p, 1.);

	mock().checkExpectations();
	mock().disable();

	DOUBLES_EQUAL(.1, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0. + PAD_SPEED*1.0, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(PAD_SPEED, p->v, NORM_MIN_DIFF);

	// but another update pad is already at rightmost limit, so stuck at 0.5
	update_pad(p, 1.);
	
	DOUBLES_EQUAL(.1, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0.5, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(PAD_SPEED, p->v, NORM_MIN_DIFF);

	dispose_pad(p);
}

TEST(PadTests, test_pad_stop_change_velocity_to_zero)
{
	PAD* p = init_pad(.2, .1, -.4, .4, PAD_SPEED);

	// check initial state is as expected
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(PAD_SPEED, p->v, NORM_MIN_DIFF);

	mock().enable();
	mock().expectNoCall("clear_area");
	mock().expectNoCall("rect_at");

	pad_stop(p);

	// check no change to state except velocity changed to 0
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(0., p->v, NORM_MIN_DIFF);

	mock().checkExpectations();
	mock().disable();

	dispose_pad(p);
}

TEST(PadTests, test_pad_go_left_change_velocity_negative)
{
	PAD* p = init_pad(.2, .1, -.4, .4, PAD_SPEED);

	// check initial state is as expected
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(PAD_SPEED, p->v, NORM_MIN_DIFF);

	mock().enable();
	mock().expectNoCall("clear_area");
	mock().expectNoCall("rect_at");

	pad_go_left(p);

	// check no change to state except velocity changed to negative
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-PAD_SPEED, p->v, NORM_MIN_DIFF);

	mock().checkExpectations();
	mock().disable();

	// call pad_go_left() again
	pad_go_left(p);

	// check no change to state, velocity stays negative
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-PAD_SPEED, p->v, NORM_MIN_DIFF);

	dispose_pad(p);
}

TEST(PadTests, test_pad_go_right_change_velocity_positive)
{
	PAD* p = init_pad(.2, .1, -.4, .4, -PAD_SPEED);

	// check initial state is as expected
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-PAD_SPEED, p->v, NORM_MIN_DIFF);

	mock().enable();
	mock().expectNoCall("clear_area");
	mock().expectNoCall("rect_at");

	pad_go_right(p);

	// check no change to state except velocity changed to positive
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(PAD_SPEED, p->v, NORM_MIN_DIFF);

	mock().checkExpectations();
	mock().disable();

	// call pad_go_right() again
	pad_go_right(p);

	// check no change to state, velocity stays positive
	DOUBLES_EQUAL(.2, p->w, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, p->h, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, p->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, p->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(PAD_SPEED, p->v, NORM_MIN_DIFF);

	dispose_pad(p);
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
