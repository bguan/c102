#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <ncurses.h>
#include <stdarg.h>

extern "C"
{
	#include "console.h"
}

TEST_GROUP(ConsoleMock){
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

static const int _MOCK_LINES = 50;
static const int _MOCK_COLS = 100;

// Mock ncurses.h initscr() call
WINDOW* initscr()
{
	mock().actualCall("initscr");
	// artificially setting these for easier calc
	LINES = _MOCK_LINES; 
	COLS = _MOCK_COLS;
	return NULL;
}

// Mock ncurses.h cbreak() call
int cbreak()
{
	return mock().actualCall("cbreak").returnIntValue();
}

// Mock ncurses.h noecho() call
int noecho()
{
	return mock().actualCall("noecho").returnIntValue();
}

// Mock ncurses.h nodelay() call
int nodelay(WINDOW* w, bool flag)
{
	return mock().
		actualCall("nodelay").
		withParameter("w", stdscr).
		withBoolParameter("flag", flag).
		returnIntValue();
}

// Mock ncurses.h curs_set() call
int curs_set(int viz)
{
	return mock().actualCall("curs_set").withParameter("viz", viz).returnIntValue();
}

// Mock ncurses.h endwin() call
int endwin()
{
	return mock().actualCall("endwin").returnIntValueOrDefault(0);
}

// Mock ncurses.h getch() call
int wgetch(WINDOW* w)
{
	return mock().
		actualCall("wgetch").
		withPointerParameter("w", w).
		returnIntValueOrDefault(0);
}

// Mock mvprintw declaration
int mvprintw(int y, int x, const char* fmt, ...) __attribute__((format(printf, 3, 4)));

// Mock mvprintw definition
int mvprintw(int y, int x, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	mock().actualCall("mvprintw").
		withIntParameter("y", y).
		withIntParameter("x", x).
		withStringParameter("fmt", fmt).
		withStringParameter("s", va_arg(args, char*));
	va_end(args);
	return mock().returnIntValueOrDefault(0);
}

// mock to check init_console calls ncurses initscr and getmaxyx
TEST(ConsoleMock, test_init_console_calls_initscr)
{
	mock().enable();
	mock().expectOneCall("initscr");
	mock().expectOneCall("cbreak").andReturnValue(0);
	mock().expectOneCall("noecho").andReturnValue(0);
	mock().expectOneCall("nodelay").withParameter("w", stdscr).withBoolParameter("flag", true).andReturnValue(0);
	mock().expectOneCall("curs_set").withParameter("viz", 0).andReturnValue(0);
	init_console();
	mock().checkExpectations();
	mock().disable();
}


TEST(ConsoleMock, test_end_console_calls_endwin)
{
	init_console();
	mock().enable();
	mock().expectOneCall("endwin");
	end_console();
	mock().checkExpectations();
	mock().disable();
}

TEST(ConsoleMock, test_get_dev_width_return_mock_value)
{
	init_console();
	int w = get_dev_width();

	CHECK_EQUAL(_MOCK_COLS, w);
}

TEST(ConsoleMock, test_get_dev_height_return_mock_value)
{
	init_console();
	int h = get_dev_height();

	CHECK_EQUAL(_MOCK_LINES, h);
}

TEST(ConsoleMock, test_get_dev_aspect_from_mock_values)
{
	init_console();
	float a = get_dev_aspect();

	DOUBLES_EQUAL(CONSOLE_FONT_ASPECT * _MOCK_COLS/_MOCK_LINES, a, 0.01);
}

TEST(ConsoleMock, test_key_pressed_call_wgetch)
{
	init_console();
	mock().enable();
	mock().expectOneCall("wgetch").withParameter("w", stdscr).andReturnValue('?');
	int k = key_pressed();
	mock().checkExpectations();
	mock().disable();

	CHECK_EQUAL('?', k);
}

TEST(ConsoleMock, test_to_dev_x_multi_cases)
{
	init_console();

	int mid_dev_x = to_dev_x(0.0);
	CHECK_EQUAL(_MOCK_COLS/2, mid_dev_x);

	int left_dev_x = to_dev_x(-0.5);
	CHECK_EQUAL(0, left_dev_x);

	int right_dev_x = to_dev_x(0.5);
	CHECK_EQUAL(_MOCK_COLS, right_dev_x);

	int bound_left_dev_x = to_dev_x(-100);
	CHECK_EQUAL(0, bound_left_dev_x);
	
	int bound_right_dev_x = to_dev_x(100);
	CHECK_EQUAL(_MOCK_COLS, bound_right_dev_x);
}

TEST(ConsoleMock, test_to_dev_y_multi_cases)
{
	init_console();

	int mid_dev_y = to_dev_y(0.0);
	CHECK_EQUAL(_MOCK_LINES/2, mid_dev_y);

	int top_dev_y = to_dev_y(-0.5);
	CHECK_EQUAL(0, top_dev_y);

	int bot_dev_y = to_dev_y(0.5);
	CHECK_EQUAL(_MOCK_LINES, bot_dev_y);

	int bound_top_dev_y = to_dev_y(-100);
	CHECK_EQUAL(0, bound_top_dev_y);
	
	int bound_bot_dev_y = to_dev_y(100);
	CHECK_EQUAL(_MOCK_LINES, bound_bot_dev_y);
}


TEST(ConsoleMock, test_to_dev_width_multi_cases)
{
	init_console();

	int mid_dev_wth = to_dev_width(0.5);
	CHECK_EQUAL(_MOCK_COLS/2, mid_dev_wth);

	int lo_dev_wth = to_dev_width(0);
	CHECK_EQUAL(0, lo_dev_wth);

	int hi_dev_wth = to_dev_width(1);
	CHECK_EQUAL(_MOCK_COLS, hi_dev_wth);

	int bound_lo_dev_wth = to_dev_width(-100);
	CHECK_EQUAL(0, bound_lo_dev_wth);
	
	int bound_hi_dev_wth = to_dev_width(100);
	CHECK_EQUAL(_MOCK_COLS, bound_hi_dev_wth);
}

TEST(ConsoleMock, test_to_dev_height_multi_cases)
{
	init_console();

	int mid_dev_ht = to_dev_height(0.5);
	CHECK_EQUAL(_MOCK_LINES/2, mid_dev_ht);

	int lo_dev_ht = to_dev_height(0);
	CHECK_EQUAL(0, lo_dev_ht);

	int hi_dev_ht = to_dev_height(1);
	CHECK_EQUAL(_MOCK_LINES, hi_dev_ht);

	int bound_lo_dev_ht = to_dev_height(-100);
	CHECK_EQUAL(0, bound_lo_dev_ht);
	
	int bound_hi_dev_ht = to_dev_height(100);
	CHECK_EQUAL(_MOCK_LINES, bound_hi_dev_ht);
}

TEST(ConsoleMock, test_text_at_call_mvprintw)
{
	init_console();
	mock().enable();

	mock().expectOneCall("mvprintw").
		withIntParameter("y", 25).
		withIntParameter("x", 50).
		withStringParameter("fmt", "%s").
		withStringParameter("s", "hello");
	text_at((char*)"hello", 0., 0., BLUE_ON_BLACK, LEFT);
	mock().checkExpectations();
	mock().clear();

	mock().expectOneCall("mvprintw").
		withIntParameter("y", 25).
		withIntParameter("x", 48).
		withStringParameter("fmt", "%s").
		withStringParameter("s", "hello");
	text_at((char*)"hello", 0., 0., BLUE_ON_BLACK, CENTER);
	mock().checkExpectations();
	mock().clear();

	mock().expectOneCall("mvprintw").
		withIntParameter("y", 25).
		withIntParameter("x", 45).
		withStringParameter("fmt", "%s").
		withStringParameter("s", "hello");
	text_at((char*)"hello", 0., 0., BLUE_ON_BLACK, RIGHT);
	mock().checkExpectations();
	mock().clear();

	mock().disable();
}

TEST(ConsoleMock, test_rect_at_call_mvprintw)
{
	init_console();
	mock().enable();
	mock().expectOneCall("mvprintw").
		withIntParameter("y", 25).
		withIntParameter("x", 50).
		withStringParameter("fmt", "%s").
		withStringParameter("s", " ");
	rect_at(0.01, 0.02, 0., 0., RED_ON_BLACK);
	mock().checkExpectations();
	mock().disable();
}