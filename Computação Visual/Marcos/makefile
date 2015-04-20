
CFLAGS = -lm `pkg-config --cflags --libs opencv`

LIBS = `pkg-config --libs opencv`

CC = gcc
CPP = g++

all: $(OBJ)
	$(CPP) $(FILE) $(CFLAGS) -o a.out -g

clen:
	rm -f *.o *- core $(PROG)
