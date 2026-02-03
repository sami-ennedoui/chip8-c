# CHIP-8 Makefile for MSYS2 UCRT64
CC=gcc
CFLAGS=-I/c/libs/SDL2/include   -L/c/libs/SDL2/lib   -lSDL3  
LDFLAGS=-L/ucrt64/lib -lSDL3

SOURCES= ./test/main.c ./src/chip8.c   # List your .c files
TARGET=chip8

OBJECTS=$(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean

