CC      = gcc
CFLAGS  = $(shell pkg-config --cflags sdl3) -Wall -Wextra
LDFLAGS = $(shell pkg-config --libs sdl3)

SOURCES = ./test/main.c ./src/chip8.c
TARGET  = chip8
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
