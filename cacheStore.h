#ifndef CACHESTORE_H
#define CACHESTORE_H

#include <stdbool.h>

typedef struct cacheRow{
   cacheNode** nodes
   /*quene*/
} cacheRow;


typedef struct cacheNode{
   int tag;
   bool validBit;
}cacheNode;

int insertElement(int length, char* address); /*returns the index of the inserted element if the array is full then returns -1*/
int initializeCache(int associativity, int blockNum); /*initializes the cache data structure so that its memory is ready to go*/

#endif