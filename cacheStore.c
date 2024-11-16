#include "cacheStore.h"
#include "queue.h"

int initializeCache(cacheRow** cache, int associativity, int blockNum){
   int maxIndex;
   int i;
   int j;

   maxIndex = blockNum / associativity;

   cache = (cacheRow**)malloc(sizeof(cacheRow*) * maxIndex);

   for(i = 0; i < maxIndex; i++){
      cache[i] = malloc(sizeof(cacheRow));
      
      initializeRRQueue(cache[i]->nodeQueue, associativity);
      cache[i]->nodes = (cacheNode**)malloc(sizeof(cacheNode) * associativity);
      for(j = 0; j < associativity; j++){
         cache[i]->nodes[j] = malloc(sizeof(cacheRow));
         cache[i]->nodes[j]->validBit = 0;
      }
   }

   return maxIndex;
}

int insertElement(int MaxIndex, int Associativity, int index, int tag, int offset, int blockSize){
   
}