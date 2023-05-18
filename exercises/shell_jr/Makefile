CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow \
-pedantic-errors -fstack-protector-all 

.PHONY: all clean

shell_jr: shell_jr.o

all: shell_jr

shell_jr.o:
	$(CC) $(CFLAGS) -c shell_jr.c

clean:
	rm -f *.o shell_jr a.out