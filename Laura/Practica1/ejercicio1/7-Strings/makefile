CC = gcc
CFLAGS= -Wall -g

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
all : strings2.o
	gcc $(CFLAGS) -o exe strings2.o
.PHONY: clean

clean:
	-rm *o exe