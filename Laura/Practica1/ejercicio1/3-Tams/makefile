CC = gcc
CFLAGS= -Wall -g

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
all : main2.o
	gcc $(CFLAGS) -o main2 main2.o
.PHONY: clean

clean:
	-rm *o main2