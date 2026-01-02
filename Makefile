CC = gcc
CFLAGS = -Wall -Wextra -Werror -fPIC -O2 -g
LDFLAGS = -shared

SRC_DIR = src/c
BUILD_DIR = build
LIB_DIR = lib

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET_LIB = $(LIB_DIR)/libanalyzer.so

# Cross-platform commands
MKDIR_P = python -c "import os, sys; [os.makedirs(d, exist_ok=True) for d in sys.argv[1:]]"
RM_RF = python -c "import shutil, sys; [shutil.rmtree(d, ignore_errors=True) for d in sys.argv[1:]]"

# OS Detection for Executable Name
ifeq ($(OS),Windows_NT)
	TARGET_EXEC = build\test_main.exe
	RUN_PREFIX = 
else
	TARGET_EXEC = build/test_main
	RUN_PREFIX = ./
endif

.PHONY: all clean test directories

all: directories $(TARGET_LIB)

directories:
	$(MKDIR_P) $(BUILD_DIR) $(LIB_DIR)

$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | directories
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM_RF) $(BUILD_DIR) $(LIB_DIR)

test_build: $(OBJS) tests/test_c/test_main.c | directories
	$(CC) $(CFLAGS) -o $(TARGET_EXEC) tests/test_c/test_main.c $(OBJS) -I$(SRC_DIR)

test: test_build
	$(RUN_PREFIX)$(TARGET_EXEC)


