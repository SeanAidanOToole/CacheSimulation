PROGRAMS = project
CC = gcc
CFLAGS = -Wall -g -std=c99 -lm
all: $(PROGRAMS)
clean:
	rm -f *.o project
# C compilations
pagetable.o: pagetable.c pagetable.h
	$(CC) $(CFLAGS) -c pagetable.c
project.o: milestone3.c pagetable.h
	$(CC) $(CFLAGS) -o project.o -c milestone3.c
# Executable programs
project: project.o pagetable.o
	$(CC) $(CFLAGS) -o project project.o pagetable.o