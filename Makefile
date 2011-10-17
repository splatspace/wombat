CC=gcc
CFLAGS=-g -Wall

OBJECTS=types.o print.o read.o main.o

all: $(OBJECTS)
	$(CC) -o uberlisp $(OBJECTS)

clean:
	rm -f *.o uberlisp
