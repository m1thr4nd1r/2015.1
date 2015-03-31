# MAKEFILE

CC = gcc

CFLAGS =  -O2 -lm `pkg-config --cflags --libs opencv`

PROG = PI

all: $(OBJ)
	$(CC) pi.c $(CFLAGS) $(LDFLAGS) -o $(PROG) -g

clean:
	rm -f *.o *~ core $(PROG)



