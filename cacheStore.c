#include "cacheStore.h"
#include "queue.h"

cacheRow* initializeCache(int associativity, int blockNum){
   cacheRow* cache;
   int maxIndex;
   int i;
   int j;

   maxIndex = blockNum / associativity;

   cache = (cacheRow*)malloc(sizeof(cacheRow) * maxIndex);

   for(i = 0; i < maxIndex; i++){
      cache->nodeQueue = (RRqueue*)malloc(sizeof(RRqueue));
      initializeRRQueue(cache->nodeQueue, associativity);

      for(j = 0; j < associativity; j++){
         cache->nodes[i] = (cacheNode*)malloc(sizeof(cacheNode));
      }
   }

   return cache;
}