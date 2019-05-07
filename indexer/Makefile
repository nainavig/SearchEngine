# Naina Vig
# Makefile for Indexer

L = ../libcs50

#PROG = indexer
OBJS = ../common/pagedir.o ../common/index.o ../common/word.o $L/hashtable.o $L/jhash.o $L/set.o $L/counters.o $L/file.o $L/webpage.o $L/memory.o 
LIB = -lm
#LLIBS = $L/libcs50-given.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make




indexer: indexer.o $(OBJS) 
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


indextest: indextest.o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indextest.o: ../common/index.h $L/file.h $L/counters.h $L/hashtable.h $L/set.h
indexer.o: $L/counters.h $L/set.h ../common/word.h ../common/pagedir.h $L/hashtable.h $L/jhash.h $L/webpage.h
counters.o: $L/bag.h $L/memory.h
set.o: $L/set.h $L/memory.h
hashtable.o: $L/hashtable.h $L/set.h $L/jhash.h $L/memory.h
jhash.o: $L/jhash.h $L/memory.h
webpage.o: $L/webpage.h $L/memory.h $L/file.h
memory.o: $L/memory.h
file.o: $L/file.h
word.o: ../common/word.h
index.o: ../common/index.h $L/counters.h $L/set.h $L/hashtable.h $L/jhash.h 

.PHONY: test clean valgrind

valgrind: $(PROG)
	valgrind ./(PROG)


test: $(PROG)
	./$(PROG)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

