# Naina Vig
# makefile for common dir

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I
CC = gcc
MAKE = make

# executable depends on object files
$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)


pagedir.o: pagedir.h

clean:
	rm -f $(PROG)
	rm -f *~ *.o
	rm -rf *.dSYM

