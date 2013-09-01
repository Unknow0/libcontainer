PROG=libcontainer.so
CC=gcc
CFLAGS=-I./include -g
LDFLAGS=-shared

SRC=$(wildcard *.c)
OBJECTS=$(SRC:.c=.o)

all:build
build: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(PROG) $^

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: clean

clean:
	rm $(OBJECTS)
