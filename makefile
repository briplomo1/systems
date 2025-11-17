
CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = build/assign4

SRC = assign4.c

OBJ = $(patsubst %.c,build/%.o,$(SRC))

BUILD_DIR = build
$(shell mkdir -p $(BUILD_DIR))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

.PHONY: all clean rebuild