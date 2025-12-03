
CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = build/assign5

SRC = assign5.c

OBJ = $(patsubst %.c,build/%.o,$(SRC))

BUILD_DIR = build

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

.PHONY: all clean rebuild