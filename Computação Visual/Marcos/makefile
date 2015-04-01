
CFLAGS = -lm `pkg-config --cflags --libs opencv`

LIBS = `pkg-config --libs opencv`

CC = gcc
CPP = g++

all: $(OBJ)
	$(CPP) aula_12_03.cpp $(CFLAGS) -o aula_12_03 -g
	$(CPP) aula_19_03.cpp $(CFLAGS) -o aula_19_03 -g
	$(CPP) aula_26_03.cpp $(CFLAGS) -o aula_26_03 -g

clen:
	rm -f *.o *- core $(PROG)
