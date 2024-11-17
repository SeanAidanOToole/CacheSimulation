#include <stdlib.h>
#include "queue.h"
#include "cacheStore.h"
#include "data.h"

cacheRow** initializeCache(int maxIndex, int associativity, int blockNum){
   cacheRow** cache;
   int i;
   int j;

   cache = (cacheRow**)malloc(sizeof(cacheRow*) * maxIndex);

   for(i = 0; i < maxIndex; i++){
      cache[i] = malloc(sizeof(cacheRow));
      
      cache[i]->nodeQueue = malloc(sizeof(RRqueue));
      initializeRRQueue(cache[i]->nodeQueue, associativity);
      cache[i]->nodes = (cacheNode**)malloc(sizeof(cacheNode) * associativity);
      for(j = 0; j < associativity; j++){
         cache[i]->nodes[j] = malloc(sizeof(cacheRow));
         cache[i]->nodes[j]->validBit = 0;
      }
   }

   return cache;
}

int insertElement(cacheRow** cache, int MaxIndex, int Associativity, int index, int tag, int offset, int blockSize){
   int i;
   /*Capacity Miss*/
   if(index > MaxIndex){
      return -1;
   }

   for(i = 0; i < Associativity; i++){
      /*hit*/
      if(cache[index]->nodes[i]->validBit == 0){
         cache[index]->nodes[i]->tag = tag;
         cache[index]->nodes[i]->validBit = 1;
         return i;
      }
   }

   /*miss?*/
   return -1;
}