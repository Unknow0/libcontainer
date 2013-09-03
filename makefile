PROG=libcontainer.so
CC=gcc
CFLAGS=-I./ -g
LDFLAGS=-shared
PREFIX=/usr/local

SRC=$(wildcard *.c)
OBJECTS=$(SRC:.c=.o)

all:build
build: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(PROG) $^

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $^

.PHONY: clean install uninstall

clean:
	rm $(OBJECTS)

install: build
	cp $(PROG) $(PREFIX)/lib/
	mkdir -p $(PREFIX)/include/container/
	cp include/* $(PREFIX)/include/container/

uninstall:
	rm $(PREFIX)/lib/$(PROG)
	rm -r $(PREFIX)/include/container
