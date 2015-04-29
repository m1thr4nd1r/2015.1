
CFLAGS = -lm `pkg-config --cflags --libs opencv`

LIBS = `pkg-config --libs opencv`

CC = gcc
CPP = g++
SRC = stereo.cpp
OUT = a.out

all: $(OBJ)
	$(CPP) $(SRC) $(CFLAGS) -o $(OUT) -g

clen:
	rm -f *.o *- core $(PROG)
