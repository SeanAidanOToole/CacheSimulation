#include "cacheStore.h"

int initializeCache(int associativity, int blockNum){
   cacheRow* cache;
   int maxIndex;
   int i;

   maxIndex = blockNum / associativity;

   cache = (cacheRow*)malloc(sizeof(cacheRow) * maxIndex);

   for(i = 0; i < maxIndex; i++){
      
   }
}