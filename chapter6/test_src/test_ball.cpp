#include "CppUTest/CommandLineTestRunner.h"
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <stdarg.h>

extern "C"
{
	#include "../include/console.h"
	#include "../include/pad.h"
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


// Mock console.h circle_at()
void circle_at(double rad, double x, double y, CONS_COLOR c)
{
	mock().actualCall("circle_at")
		.withDoubleParameter("rad", rad)
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


TEST(BallTests, test_update_ball_stationary_stays_same_spot)
{
	BALL* b = init_ball(.01, .0, .0, .0, .0);

	mock().enable();
	mock().expectOneCall("clear_area")
		.withDoubleParameter("left_x", -.02)
		.withDoubleParameter("top_y", -.02)
		.withDoubleParameter("right_x", .02)
		.withDoubleParameter("bot_y", .02);

	mock().expectOneCall("circle_at")
		.withDoubleParameter("rad", 0.01)
		.withDoubleParameter("x", .0)
		.withDoubleParameter("y", .0)
		.withIntParameter("c", BALL_COLOR);
		
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
		.withDoubleParameter("left_x", -.02)
		.withDoubleParameter("top_y", -.02)
		.withDoubleParameter("right_x", .02)
		.withDoubleParameter("bot_y", .02);

	mock().expectOneCall("circle_at")
		.withDoubleParameter("rad", 0.01)
		.withDoubleParameter("x", .1)
		.withDoubleParameter("y", .0)
		.withIntParameter("c", BALL_COLOR);
		
	update_ball(b, 1.); // update after 1 sec

	mock().checkExpectations();
	mock().clear();
	mock().disable();

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

	dispose_ball(b);
}

TEST(BallTests, test_update_ball_moving_ball_moved_to_expected_spot_in_Y)
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
	DOUBLES_EQUAL(.5, b->y, NORM_MIN_DIFF);
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
	DOUBLES_EQUAL(.5, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_top_wall)
{
	double rad = .01;
	BALL *b = init_ball(rad, .0, .0, .1, -.2); // moving in x & y

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_top_wall(b, -.45);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_top_wall(b, -.45);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_top_wall(b, -.45);

	CHECK(bounced);
	DOUBLES_EQUAL(.3, b->x, NORM_MIN_DIFF);
	
	// w/o bounce y would be -.6, -.1 more than -.45
	DOUBLES_EQUAL(-.45 + rad, b->y, NORM_MIN_DIFF); 

	// Y velocity reversed direction!
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF); 

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_bottom_wall)
{
	double rad = 0.01;
	BALL *b = init_ball(rad, .0, .0, .1, .2); // moving in x & y

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_bottom_wall(b, .45);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_bottom_wall(b, .45);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_bottom_wall(b, .45);

	CHECK(bounced);
	DOUBLES_EQUAL(.3, b->x, NORM_MIN_DIFF);

	// w/o bounce y would be .6, .1 more than .45
	DOUBLES_EQUAL(.45 - rad, b->y, NORM_MIN_DIFF); 

	// Y velocity reversed direction!
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF); 

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_left_wall)
{
	BALL* b = init_ball(.01, .0, .0, -.1, -.2); // moving in x & y 

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_left_wall(b, -.5);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(-.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_left_wall(b, -.5);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(-.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 4.); // update after 4 more secs
	bounced = bounce_if_touching_left_wall(b, -.5);

	CHECK(bounced);

	// w/o bounce x would be -.6, -.1 more than -.5
	DOUBLES_EQUAL(-.5, b->x, NORM_MIN_DIFF); 

	DOUBLES_EQUAL(-.5, b->y, NORM_MIN_DIFF); 

	// x velocity reversed direction!
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_right_wall)
{
	BALL* b = init_ball(.01, .0, .0, .1, .2); // moving in x & y 

	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_right_wall(b, .5);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_right_wall(b, .5);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.4, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 4.); // update after 4 more secs
	bounced = bounce_if_touching_right_wall(b, .5);

	CHECK(bounced);

	// w/o bounce x would be .6, .1 more than .5
	DOUBLES_EQUAL(.5, b->x, NORM_MIN_DIFF); 

	DOUBLES_EQUAL(.5, b->y, NORM_MIN_DIFF); 

	// x velocity reversed direction!
	DOUBLES_EQUAL(-.1, b->vx, NORM_MIN_DIFF);

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_top_pad)
{
	PAD *p = init_pad(.2, .1, 0., -.4, 1.);
	BALL *b = init_ball(.1, .0, .0, .1, -.2); // moving in x & y

	DOUBLES_EQUAL(.1, b->rad, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_pad(b, p);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.1, b->rad, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_pad(b, p);

	CHECK_TRUE(bounced);
	DOUBLES_EQUAL(.1, b->rad, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);

	// changed by bounce to boundary pad and ball
	DOUBLES_EQUAL(-.25, b->y, NORM_MIN_DIFF); 

	// changed by side spin
	DOUBLES_EQUAL(.1 + SIDE_SPIN_FACTOR*p->v, b->vx, NORM_MIN_DIFF); 

	// reversed by bounce
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	dispose_ball(b);
}

TEST(BallTests, test_bounce_if_touching_bottom_pad)
{
	PAD *p = init_pad(.2, .1, 0., .4, 1.);
	BALL *b = init_ball(.1, .0, .0, .1, .2); // moving in x & y

	DOUBLES_EQUAL(.1, b->rad, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.0, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after 1 sec
	bool bounced = bounce_if_touching_pad(b, p);

	CHECK_FALSE(bounced);
	DOUBLES_EQUAL(.1, b->rad, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->x, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->y, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.1, b->vx, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->vy, NORM_MIN_DIFF);

	update_ball(b, 1.); // update after another sec
	bounced = bounce_if_touching_pad(b, p);

	CHECK_TRUE(bounced);
	DOUBLES_EQUAL(.1, b->rad, NORM_MIN_DIFF);
	DOUBLES_EQUAL(.2, b->x, NORM_MIN_DIFF);

	// changed by bounce to boundary pad and ball
	DOUBLES_EQUAL(.25, b->y, NORM_MIN_DIFF); 

	// changed by side spin
	DOUBLES_EQUAL(.1 + SIDE_SPIN_FACTOR*p->v, b->vx, NORM_MIN_DIFF); 

	// reversed by bounce
	DOUBLES_EQUAL(-.2, b->vy, NORM_MIN_DIFF);

	dispose_ball(b);
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
