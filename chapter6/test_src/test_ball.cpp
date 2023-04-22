#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdarg.h>

extern "C"
{
	#include "../include/ball.h"
}

TEST_GROUP(BallTests){
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

TEST(BallTests, test_init_ball_calls_circle_at)
{
	mock().enable();
	mock().expectOneCall("circle_at")
		.withDoubleParameter("rad", 0.01)
		.withDoubleParameter("x", 0.)
		.withDoubleParameter("y", 0.)
		.withIntParameter("c", BALL_COLOR);
		
	BALL* b = init_ball(.01, 0., 0., 0., 0.);

	CHECK(b != NULL);

	DOUBLES_EQUAL(.01, b->rad, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vy, NORM_MIN_DIFF);
		
	mock().checkExpectations();
	mock().disable();
	
	dispose_ball(b);
}

TEST(BallTests, test_dispose_ball_calls_clear_area)
{
	BALL* b = init_ball(.01, 0., 0., 0., 0.);

	mock().enable();
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", -.01)
		.withDoubleParameter("top_y", -.01)
		.withDoubleParameter("right_x", .01)
		.withDoubleParameter("bot_y", .01);
	
	dispose_ball(b);
	
	mock().checkExpectations();
	mock().disable();
}


TEST(BallTests, test_update_ball_stationary_stays_same_spot_no_draw)
{
	BALL* b = init_ball(.01, .0, .0, .0, .0);

	mock().enable();
	mock().expectNoCall("clear_area");
	mock().expectNoCall("circle_at");
	update_ball(b, 100.);
	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	mock().checkExpectations();
	mock().disable();

	dispose_ball(b);
}


TEST(BallTests, test_update_ball_moving_ball_moved_to_expected_spot_in_X)
{
	BALL* b = init_ball(.01, .0, .0, .1, .0); // moving in X axis only

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vy, NORM_MIN_DIFF);

	mock().enable();

	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", -.01)
		.withDoubleParameter("top_y", -.01)
		.withDoubleParameter("right_x", .01)
		.withDoubleParameter("bot_y", .01);

	mock().expectOneCall("circle_at")
		.withDoubleParameter("rad", 0.01)
		.withDoubleParameter("x", .1)
		.withDoubleParameter("y", .0)
		.withIntParameter("c", BALL_COLOR);
		
	update_ball(b, 1.); // update after 1 sec

	mock().checkExpectations();
	mock().clear();

	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec

	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vy, NORM_MIN_DIFF);

	update_ball(b, 3.); // update after another sec

	DOUBLES_EQUAL(.5, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vy, NORM_MIN_DIFF);

	mock().disable();

	dispose_ball(b);
}

TEST(BallTests, test_update_ball_moving_ball_moved_to_expected_spot_in_y)
{
	BALL* b = init_ball(.01, .0, .0, .0, .2); // moving in y axis only

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 3.); // update after another sec

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(1.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	dispose_ball(b);
}

TEST(BallTests, test_update_ball_moving_ball_moved_to_expected_spot_in_x_y)
{
	BALL* b = init_ball(.01, .0, .0, -.1, .2); // moving in y axis only

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec

	DOUBLES_EQUAL(-.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec

	DOUBLES_EQUAL(-.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 3.); // update after another sec

	DOUBLES_EQUAL(-.5, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(1.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_top)
{
	BALL* b = init_ball(.01, .0, .0, .1, -.2); // moving in x & y 

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_top(b, -.5, .05);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_top(b, -.5, .05);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_top(b, -.5, .05);

	CHECK(bounced);
	DOUBLES_EQUAL(.3, b->x, NORM_MIN_DIFF);
	
	// w/o bounce y would be -.6, -.1 more than -.5
	DOUBLES_EQUAL(-.5, b->y, NORM_MIN_DIFF); 

	// side spin added to ball's x velocity
	DOUBLES_EQUAL(.15, b->vx, NORM_MIN_DIFF); 

	// Y velocity reversed direction!
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF); 

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_bottom)
{
	BALL* b = init_ball(.01, .0, .0, .1, .2); // moving in x & y 

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_bottom(b, .5, .05);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_bottom(b, .5, .05);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_bottom(b, .5, .05);

	CHECK(bounced);
	DOUBLES_EQUAL(.3, b->x, NORM_MIN_DIFF);

	// w/o bounce y would be .6, .1 more than .5
	DOUBLES_EQUAL(.5, b->y, NORM_MIN_DIFF); 

	// side spin added to ball's x velocity
	DOUBLES_EQUAL(.15, b->vx, NORM_MIN_DIFF);

	// Y velocity reversed direction!
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF); 

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_left)
{
	BALL* b = init_ball(.01, .0, .0, -.1, -.2); // moving in x & y 

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_left(b, -.5, .05);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(-.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_left(b, -.5, .05);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(-.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 4.); // update after 4 more secs
	bounced = bounce_if_touching_left(b, -.5, .05);

	CHECK(bounced);

	// w/o bounce x would be -.6, -.1 more than -.5
	DOUBLES_EQUAL(-.5, b->x, NORM_MIN_DIFF); 

	DOUBLES_EQUAL(-1.2, b->y, NORM_MIN_DIFF); 

	// x velocity reversed direction!
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);

	// top spin added to y velocity  
	DOUBLES_EQUAL(-.15, b->vy, NORM_MIN_DIFF); 

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_right)
{
	BALL* b = init_ball(.01, .0, .0, .1, .2); // moving in x & y 

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_right(b, .5, .05);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_right(b, .5, .05);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 4.); // update after 4 more secs
	bounced = bounce_if_touching_right(b, .5, .05);

	CHECK(bounced);

	// w/o bounce x would be .6, .1 more than .5
	DOUBLES_EQUAL(.5, b->x, NORM_MIN_DIFF); 

	DOUBLES_EQUAL(1.2, b->y, NORM_MIN_DIFF); 

	// x velocity reversed direction!
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);

	// top spin added to y velocity
	DOUBLES_EQUAL(.25, b->vy, NORM_MIN_DIFF); 

	dispose_ball(b);
}