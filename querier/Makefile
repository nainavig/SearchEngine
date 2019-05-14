
# Naina Vig
# Makefile for querier


# pathname variables
L = ../libcs50
C = ../common

LIB = -lm
LLIBS = $L/libcs50.a $C/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L -I$C
CC = gcc
MAKE = make


querier: querier.o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LLIBS) $(LIBS) -o $@


querier.o: $L/file.h $L/hashtable.h $L/counters.h $L/set.h $C/index.h $C/word.h
file.o: $L/file.h
counters.o: $L/bag.h $L/memory.h
set.o: $L/set.h $L/memory.h
hashtable.o: $L/hashtable.h $L/set.h $L/jhash.h $L/memory.h
jhash.o: $L/jhash.h $L/memory.h
memory.o: $L/memory.h
index.o: $C/index.h $L/counters.h $L/set.h $L/hashtable.h $L/jhash.h
word.o: $C/word.h

.PHONY: clean valgrind test

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

valgrind: valgrindtest.sh
	./valgrindtest.sh

test: testing.sh
	./testing.sh
