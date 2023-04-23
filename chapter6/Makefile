# Makefile for the PONG project

# pointing C and C++ compilers to the relevant tools on your machine
# allowing quick change say from clang to gcc or whatever you
CC = cc
CPP = c++

# directory structures, separating C header files in include/, C source code src/, C++ test code in test_src/
# runtime executable and objects in build/ and test time executable and objects in test_build/
INCL_DIR = include
SRC_DIR = src
BLD_DIR = build
TEST_SRC_DIR = test_src
TEST_BLD_DIR = test_build

# the executables for run & test, as top level build targets
MAIN = main

# use wildcard to find all .c files under source directory
ALL_SRC = $(wildcard $(SRC_DIR)/*.c)

# use wildcard to find all .cpp files under test source directory
ALL_TEST_SRC = $(wildcard $(TEST_SRC_DIR)/*.cpp)

# to get all runtime objects, substitude build for src in path, while changing suffix from .c to .o
ALL_OBJ = $(subst $(SRC_DIR), $(BLD_DIR), $(ALL_SRC:.c=.o))
NON_MAIN_OBJ = $(filter-out $(BLD_DIR)/$(MAIN).o, $(ALL_OBJ))

# to get all test objects, substitude test_build for test_src in path, while changing suffix from .cpp to .o
ALL_TESTS = $(subst $(TEST_SRC_DIR), $(TEST_BLD_DIR), $(ALL_TEST_SRC:.cpp=))

# flags for C, C++ compiler and linker for runtime and test time
CFLAGS = -g -Wall -I$(INCL_DIR)
CPPFLAGS = -g -Wall -I$(INCL_DIR)
LDFLAGS = -lm -lncurses
TEST_LDFLAGS = $(LDFLAGS) -lCppUTest -lCppUTestExt 

# Rules for Make targets
all: $(BLD_DIR)/$(MAIN) 

$(BLD_DIR):
	mkdir -p $(BLD_DIR)

$(TEST_BLD_DIR): 
	mkdir -p $(TEST_BLD_DIR)
 
$(BLD_DIR)/$(MAIN): $(BLD_DIR) $(ALL_OBJ)
	$(CC) $(CFLAGS) $(ALL_OBJ) -o $@ $(LDFLAGS) 

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) -c $^ -o $@

run: $(BLD_DIR)/$(MAIN)
	$(BLD_DIR)/$(MAIN) 

test: $(BLD_DIR) $(TEST_BLD_DIR) $(ALL_TESTS)

$(TEST_BLD_DIR)/%: $(TEST_SRC_DIR)/%.cpp $(NON_MAIN_OBJ)
	$(CPP) $(CPPFLAGS) $< $(NON_MAIN_OBJ) -o $@ $(TEST_LDFLAGS)
	$@ -v


clean:
	rm -fr $(BLD_DIR) $(TEST_BLD_DIR)
