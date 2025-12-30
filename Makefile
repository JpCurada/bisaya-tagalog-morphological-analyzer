CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC -O2 -g
LDFLAGS = -shared

SRC_DIR = src/c
BUILD_DIR = build
LIB_DIR = lib

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET_LIB = $(LIB_DIR)/libanalyzer.so

.PHONY: all clean test directories

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

test_build: $(OBJS) tests/test_c/test_main.c | directories
	$(CC) $(CFLAGS) -o build/test_main tests/test_c/test_main.c $(OBJS) -I$(SRC_DIR)

test: test_build
	./build/test_main
