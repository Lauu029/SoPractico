CC = gcc
CFLAGS= -Wall -g

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
all : array2.o
	gcc $(CFLAGS) -o exe array2.o
.PHONY: clean

clean:
	-rm *o exe