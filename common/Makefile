# Naina Vig
# makefile for common dir

L = ../libcs50

OBJS = word.o pagedir.o index.o $L/jhash.o $L/memory.o $L/hashtable.o $L/set.o $L/counters.o $L/file.o
LIB = common.a


CFLAGS = -Wall -pedantic -std=c11 -ggdb 
CC = gcc
MAKE = make


# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)


pagedir.o: pagedir.h
word.o: word.h
index.o: index.h $L/hashtable.h $L/set.h $L/file.h $L/counters.h 
counters.o: $L/bag.h $L/memory.h
set.o: $L/set.h $L/memory.h
hashtable.o: $L/hashtable.h $L/set.h $L/jhash.h $L/memory.h
jhash.o: $L/jhash.h $L/memory.h
memory.o: $L/memory.h
file.o: $L/file.h


.PHONY: clean

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM

