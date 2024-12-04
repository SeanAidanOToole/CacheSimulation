PROGRAMS = project
CC = gcc
CFLAGS = -Wall -g -std=c99 -lm
all: $(PROGRAMS)
clean:
	rm -f *.o project3
# C compilations
project.o: milestone3.c
	$(CC) $(CFLAGS) -o project.o -c milestone3.c
# Executable programs
project: project.o 
	$(CC) $(CFLAGS) -o project project.o 