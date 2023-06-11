CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all:  Surtidores

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco:  Surtidores.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm Surtidores *.o