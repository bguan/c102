#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "../include/console.h"

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

// Mock console.h clear_area()
void clear_area(double left_x, double top_y, double right_x, double bot_y)
{
	mock().actualCall("clear_area")
		.withDoubleParameter("left_x", left_x)
		.withDoubleParameter("top_y", top_y)
		.withDoubleParameter("right_x", right_x)
		.withDoubleParameter("bot_y", bot_y);
}
