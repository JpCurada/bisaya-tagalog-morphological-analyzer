CC = gcc
CFLAGS = -Wall -Wextra -fPIC -O2 -g
LDFLAGS = -shared

SRC_DIR = src/c
BUILD_DIR = build
LIB_DIR = lib
TEST_DIR = tests/test_c

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET_LIB = $(LIB_DIR)/libanalyzer.so

.PHONY: all clean test directories test_fsa

all: directories $(TARGET_LIB)

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(LIB_DIR)

$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | directories
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

# Legacy test target
test_build: $(OBJS) tests/test_c/test_main.c | directories
	$(CC) $(CFLAGS) -o build/test_main tests/test_c/test_main.c $(OBJS) -I$(SRC_DIR)

test: test_build
	./build/test_main

# New FSA test target
test_fsa_build: $(OBJS) $(TEST_DIR)/test_fsa.c | directories
	$(CC) $(CFLAGS) -o build/test_fsa $(TEST_DIR)/test_fsa.c $(OBJS) -I$(SRC_DIR)

test_fsa: test_fsa_build
	./build/test_fsa

# Run all tests
test_all: test test_fsa
