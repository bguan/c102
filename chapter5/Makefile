CC = cc
CPP = c++
CFLAGS = -g -Wall -Iinclude
CPPFLAGS = -g -Wall -Iinclude
LDFLAGS = -pthreads
TEST_LDFLAGS = -lCppUTest -lCppUTestExt -pthreads

all:	build/main build/test_all

build/main: src/main.c build/common.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

build/common.o: src/common.c include/common.h
	$(CC) $(CFLAGS) -c $< -o $@

test:	build/test_all run_tests

build/test_all: test_src/test_all.cpp build/common.o build/test_common.o
	$(CPP) $(CPPFLAGS) $(TEST_LDFLAGS) $^ -o $@

build/test_common.o: test_src/test_common.cpp build/common.o
	$(CPP) $(CPPFLAGS) -c $< -o $@

run_tests:	build/test_all
	build/test_all

clean:
	rm -fr build/*
