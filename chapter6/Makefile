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
TEST_MAIN = test_main

# use wildcard to find all .c files under source directory
ALL_SRC = $(filter-out $(SRC_DIR)/$(MAIN).c, $(wildcard $(SRC_DIR)/*.c))

# use wildcard to find all .cpp files under test source directory
ALL_TEST_SRC = $(filter-out $(TEST_SRC_DIR)/$(TEST_MAIN).cpp, $(wildcard $(TEST_SRC_DIR)/*.cpp))

# to get all runtime objects, substitude build for src in path, while changing suffix from .c to .o
ALL_OBJ = $(subst $(SRC_DIR), $(BLD_DIR), $(ALL_SRC:.c=.o))

# to get all test time objects, substitude test_build for test_src in path, while changing suffix from .cpp to .o
ALL_TEST_OBJ = $(ALL_OBJ) $(subst $(TEST_SRC_DIR), $(TEST_BLD_DIR), $(ALL_TEST_SRC:.cpp=.o))

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
	$(CC) $(CFLAGS) $(ALL_OBJ) $(SRC_DIR)/$(MAIN).c -o $@ $(LDFLAGS) 

run: $(BLD_DIR)/$(MAIN)
	$(BLD_DIR)/$(MAIN) 

$(TEST_BLD_DIR)/$(TEST_MAIN): $(BLD_DIR) $(TEST_BLD_DIR) $(ALL_TEST_OBJ)
	$(CPP) $(CPPFLAGS) $(ALL_TEST_OBJ) $(TEST_SRC_DIR)/$(TEST_MAIN).cpp -o $@ $(TEST_LDFLAGS) 

test: $(TEST_BLD_DIR)/$(TEST_MAIN)
	$(TEST_BLD_DIR)/$(TEST_MAIN) -v 

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) -c $^ -o $@

$(TEST_BLD_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $^ -o $@

clean:
	rm -fr $(BLD_DIR) $(TEST_BLD_DIR)
