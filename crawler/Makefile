# Naina Vig
# Makefile for Crawler

PROG = crawler
OBJS = crawler.o ../common/pagedir.o ../libcs50/hashtable.o ../libcs50/jhash.o ../libcs50/set.o ../libcs50/bag.o ../libcs50/file.o ../libcs50/webpage.o ../libcs50/memory.o 
LIB = libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make



$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


crawler.o: ../libcs50/bag.h ../libcs50/set.h ../common/pagedir.h ../libcs50/hashtable.h ../libcs50/jhash.h ../libcs50/webpage.h 
bag.o: ../libcs50/bag.h ../libcs50/memory.h 
set.o: ../libcs50/set.h  ../libcs50/memory.h
hashtable.o: ../libcs50/hashtable.h ../libcs50/set.h ../libcs50/jhash.h ../libcs50/memory.h
jhash.o: ../libcs50/jhash.h ../libcs50/memory.h
webpage.o: ../libcs50/webpage.h ../libcs50/memory.h ../libcs50/file.h
memory.o: ../libcs50/memory.h
file.o: ../libcs50/file.h



.PHONY: test clean

test: $(PROG)
	./$(PROG)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)


