PROG=libcontainer
CC=gcc
CFLAGS=-I. -g
LDFLAGS=-shared
PREFIX=/usr/local

TEST=test
TEST_LDFLAGS=
TEST_OBJ=test.o

SRC=$(wildcard *.c)
OBJECTS=$(filter-out $(TEST_OBJ),$(SRC:.c=.o))

all: build

build: $(PROG).so $(PROG).a

$(PROG).so: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
$(PROG).a: $(OBJECTS)
	rm -f $@
	ar rcs $@ $^

$(TEST): $(TEST_OBJ) $(OBJECTS)
	$(CC) $(TEST_LDFLAGS) -o $@ $^

%.o:%.c %.h
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean install uninstall

clean:
	rm -f $(OBJECTS)
	rm -f $(TEST_OBJ)

install: build
	cp $(PROG).* $(PREFIX)/lib/
	mkdir -p $(PREFIX)/include/container/
	cp container/* $(PREFIX)/include/container/

uninstall:
	rm -f $(PREFIX)/lib/$(PROG).*
	rm -rf $(PREFIX)/include/container
