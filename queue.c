#include <stdio.h>
#include "queue.h"

int initializeRRQueue(RRqueue* q, int associativity){
   q->maxSize = associativity;
}
bool isFull(RRqueue* q){
   if(q->rear == q->maxSize){
      return false;
   }

   return true;
}
bool isEmpty(RRqueue* q){
   if(q->front == - 1){
      return false;
   }

   return true;
}
void enqueue(RRqueue* q, cacheNode* node){
   if(isFull(q)){
      return NULL;
   }

   q->items[q->rear] = node;
   q->rear++;
}
cacheNode* dequeue(RRqueue* q){
   cacheNode* currNode;

   if(isEmpty){
      return NULL;
   }

   q->front++;
   currNode = q->items[q->front];

   return currNode;
}