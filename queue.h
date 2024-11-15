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

void initializeRRQueue(RRqueue* q, int associativity);
bool isFull(RRqueue* q);
bool isEmpty(RRqueue* q);
void enqueue(RRqueue* q, cacheNode* node);
cacheNode* dequeue(RRqueue* q);

#endif