#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

typedef struct cacheNode{
   int tag;
   bool validBit;
}cacheNode;

typedef struct RRqueue{
   cacheNode** items;
   int maxSize;
   int front;
   int rear;
}RRqueue;

typedef struct cacheRow{
   cacheNode** nodes;
   RRqueue* nodeQueue;
}cacheRow;

#endif