CC = gcc
CFLAGS= -Wall -g

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
all : punteros3.o
	gcc $(CFLAGS) -o exe punteros3.o
.PHONY: clean

clean:
	-rm *o exe