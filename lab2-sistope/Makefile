CC = gcc
CFLAGS = -Wall -Wextra -g

all: lab2

lab2: lab2.o funciones.o
	$(CC) $(CFLAGS) -o lab2 lab2.o funciones.o

lab2.o: lab2.c funciones.h
	$(CC) $(CFLAGS) -c lab2.c

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c

clean:
	rm -f lab2 *.o
