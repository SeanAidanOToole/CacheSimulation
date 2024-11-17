CC = gcc
FLAGS = -Wall -std=c99 -lm

all: project
clean:
	rm -f *.o project

project.o: project.c queue.h cacheStore.h
	$(CC) $(FLAGS) -c project.c
cacheStore.o: cacheStore.c cacheStore.h queue.h data.h
	$(CC) $(FLAGS) -c cacheStore.c
queue.o: queue.c queue.h data.h cacheStore.h
	$(CC) $(FLAGS) -c queue.c

project: project.o cacheStore.o queue.o
	$(CC) $(FLAGS) -o project project.o cacheStore.o queue.o