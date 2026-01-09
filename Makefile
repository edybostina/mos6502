CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TARGET = my_program

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

format:
	clang-format -i $(SRC_DIR)/*.c $(INC_DIR)/*.h

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
