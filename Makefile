CC=gcc
CFLAGS=-g -Wall -I./include

OBJECTS=src/alist.o src/types.o src/print_form.o src/read_form.o src/main.o
SOURCES=$(patsubst %.o,%.c,$(OBJECTS))

all: $(OBJECTS)
	$(CC) -o uberlisp $(OBJECTS)

debug:
	$(CC) -ggdb -c $(SOURCES)
	$(CC) -ggdb -o uberlisp $(OBJECTS)

clean:
	rm -f src/*.o uberlisp
