CC=gcc
CFLAGS=-g -Wall

OBJECTS=alist.o types.o print_form.o read_form.o main.o

all: $(OBJECTS)
	$(CC) -o uberlisp $(OBJECTS)

clean:
	rm -f *.o uberlisp
