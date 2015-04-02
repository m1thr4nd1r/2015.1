#MAKEFILE

CC = gcc

CFLAGS = -lm `pkg-config --cflags --libs opencv`

PROG = PI

all: $(OBJ)
		$(CC) ${SRC} $(CFLAGS) $(LDFLAGS) -o $(PROG) -g

clean:
		rm -f *.o *~ core $(PROG)