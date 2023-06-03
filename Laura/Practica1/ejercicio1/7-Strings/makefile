CC = gcc
CFLAGS= -Wall -g

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
all : arg2.o
	gcc $(CFLAGS) -o exe arg2.o
.PHONY: clean

clean:
	-rm *o exe