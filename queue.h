#ifndef QUENE_H
#define QUENE_H

#include <stdbool.h>
#include "cacheStore.h"

typedef struct RRqueue{
   cacheNode** items;
   int maxSize;
   int front;
   int rear;
}RRqueue;

int initializeRRQueue(RRqueue* q, int associativity);
bool isFull(RRqueue* q);
bool isEmpty(RRqueue* q);
cacheNode* enqueue(RRqueue* q, cacheNode* node);
cacheNode* dequeue(RRqueue* q)

#endif