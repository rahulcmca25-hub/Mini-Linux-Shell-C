CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = rahul-shell

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SRCS = $(SRC_DIR)/main.c
OBJS = $(BUILD_DIR)/main.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean