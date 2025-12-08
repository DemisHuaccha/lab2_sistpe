CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGET=lab2
SRCS=lab2.c funciones.c
OBJS=$(SRCS:.c=.o)
HEADERS=funciones.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
