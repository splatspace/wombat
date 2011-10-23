CC=gcc
CFLAGS=-g -Wall

OBJECTS=alist.o types.o print_form.o read_form.o main.o
SOURCES=$(patsubst %.o,%.c,$(OBJECTS))

all: $(OBJECTS)
	$(CC) -o uberlisp $(OBJECTS)

debug:
	$(CC) -ggdb -c $(SOURCES)
	$(CC) -ggdb -o uberlisp $(OBJECTS)

clean:
	rm -f *.o uberlisp
