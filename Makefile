CC=gcc
CFLAGS=`sdl2-config --cflags` -Wall -Wextra -O2 -Iinclude -I/opt/homebrew/include/SDL2
LDFLAGS=`sdl2-config --libs` -L/opt/homebrew/lib -lm

SRC=$(wildcard src/*.c) main.c
OBJ=$(SRC:.c=.o)
TARGET=3d_engine

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
