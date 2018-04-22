CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 

shrink: shrink.o
	${CC} ${CFLAGS} -o $@ $^

shrink.o: shrink.c shrink.h


