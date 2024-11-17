#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include "data.h"

void initializeRRQueue(RRqueue* q, int associativity);
bool isFull(RRqueue* q);
bool isEmpty(RRqueue* q);
void enqueue(RRqueue* q, cacheNode* node);
cacheNode* dequeue(RRqueue* q);

#endif