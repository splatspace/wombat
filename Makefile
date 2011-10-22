CC=gcc
CFLAGS=-g -Wall

OBJECTS=alist.o types.o print.o read.o main.o

all: $(OBJECTS)
	$(CC) -o uberlisp $(OBJECTS)

clean:
	rm -f *.o uberlisp
