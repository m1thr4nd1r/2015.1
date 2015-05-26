#MAKEFILE

CC = gcc
CPP = g++

CFLAGS = -lm `pkg-config --cflags --libs opencv`

SRC=stereo.cpp
PROG = a.out

all: $(OBJ)
		$(CPP) ${SRC} $(CFLAGS) $(LDFLAGS) -o $(PROG) -g

test: $(OBJ) $(OBJ)
		$(CPP) ${CODE} $(CFLAGS) $(LDFLAGS) -o $(PROG) -g
		$(CPP) ${SRC} $(CFLAGS) $(LDFLAGS) -o $(OUT) -g

clean:
		rm -f *.o *~ core $(PROG)
