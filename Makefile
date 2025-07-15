SHELL := /bin/sh
CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: life

life: life.c universe.c
	$(CC) -o life life.c universe.c -lncurses

life.o: *.c
	$(CC) $(CFLAGS) -c *.c

clean:
	rm -f life *.o

format:
	clang-format -i -style=file *.[ch]
