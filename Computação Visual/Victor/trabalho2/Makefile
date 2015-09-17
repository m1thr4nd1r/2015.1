#MAKEFILE

CC = gcc
CPP = g++

CFLAGS = -lm `pkg-config --cflags --libs opencv`

PROG = a.out

all: $(OBJ)
		$(CPP) ${SRC} $(CFLAGS) $(LDFLAGS) -o $(PROG) -g

clean:
		rm -f *.o *~ core $(PROG)