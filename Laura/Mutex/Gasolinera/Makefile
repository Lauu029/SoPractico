CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all:  muelle2

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco:  muelle2.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm muelle2 muelle *.o