#ifndef CACHESTORE_H
#define CACHESTORE_H

#include "data.h"

int insertElement(cacheRow** cache, int MaxIndex, int Associativity, int index, int tag, int offset, int blockSize); /*returns the index of the inserted element if the array is full then returns -1*/
cacheRow** initializeCache(int maxIndex, int associativity, int blockNum); /*initializes the cache data structure so that its memory is ready to go returns the maxIndex of the array*/

#endif