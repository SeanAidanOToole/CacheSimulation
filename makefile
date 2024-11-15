CC = gcc
FLAGS = -Wall -std=c99

project2: milestone1 generate.o
	$(CC) $(FLAGS) -o project2 milestone1.o cacheStore.o
milestone1.o:
	$(CC) $(FLAGS) -c -o milestone1.o milestone1.c
cacheStore.o:
	$(CC) $(FLAGS) -c -o cacheStore.o cacheStore.c
clean:
	rm *.o project1