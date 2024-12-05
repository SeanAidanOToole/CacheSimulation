#include "pagetable.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define EMPTY -1

// global variables
int compMiss = 0;
int confMiss = 0;
int cacheHits = 0;
int cacheAccess = 0;
int exists; // if its 1 then it is in the cache already or put into the cache if its 0 then need to implement replacement policy
int instBytes = 0;
int srcdstBytes = 0; // instruction bytes instBytes += length then on affective address then you instBytes += 4
int blockCount = 0;
int numAddress = 0; //
// variables for cycle
int cycleH = 0;
int cycleM = 0;
int addrCycle = 0;
int blockSize;
int instructionCount = 0; // The number of lines that contain "SRC DST" that aren't 0
// function Replacement policy is random
void insertCache(int row, int assc, int cache[row][assc], unsigned int tag, unsigned int index, unsigned int offset, int length, int max, int *Bytes) {
   exists = 1;
   cacheAccess += 1;
   int i;
   // int count;
   for (i = 0; i < assc && exists == 1; i++) {
      // code for hit
      if (cache[index][i] == tag) { // The index matches the tag and gets a hit
                                    // add the max offset - what alr has data
                                    // bytes goes over the block limit, triggers new index, add comp miss and new bytes
                                    /* if(length + offset > max){
                                    //	cacheHits += (int)ceil((length - (max - offset + 1))/offset) + 1; //redo
                                    //	cacheHits += 1;
                                    */
         cacheHits += 1;
         cycleH += 1;
         // first access to cache, [i] = -1 -> [i] = tag
      } else if (cache[index][i] == -1) { // the index is empty
         cache[index][i] = tag;           // place the tag into cache
         compMiss += 1;
         cycleM += (4 * ceil(blockSize / 4));
         blockCount += 1;
         if (length + offset > max) {
            cacheHits += max - offset;
         } else {
            cacheHits += length - 1;
         }
      } else {
         continue;
      }
      *Bytes += 1;
      exists = 0;
   }

   if (exists == 1) { // conflict miss, implement replacement algorithim

      confMiss++;

      if (length + offset > max) {
         cacheHits += max - offset;
      } else {
         cacheHits += length - 1;
      }
      *Bytes += max;
      // implement rnd
      int randAssc = rand() % assc;
      cache[index][randAssc] = tag;
   }

   if (length + offset > max) { // there is overreach into another
      length = length - (max - offset);
      insertCache(row, assc, cache, tag, index + 1, 0, length, max, Bytes); // recursive call
   }
}

int main(int argc, char *argv[]) {

   int i;
   int cSize;
   int bSize;
   int assc;
   char repPolicy[12];
   int pMem;
   int pMemUsed;
   int timeSlice;
   int fileCount = 0;
   char *trace1 = NULL;
   char *trace2 = NULL;
   char *trace3 = NULL;

   printf("\nCache Simulation CS 3853 Fall 2024 - Group #07\n");
   printf("\nTrace File(s):\n");

   for (i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-f") == 0) {
         printf("\t%s\n", argv[i + 1]);
         if (fileCount == 0) {
            trace1 = argv[i + 1];
         } else if (fileCount == 1) {
            trace2 = argv[i + 1];
         } else {
            trace3 = argv[i + 1];
         }
         fileCount++;
      }
      if (strcmp(argv[i], "-s") == 0) {
         cSize = atoi(argv[i + 1]);
      }
      if (strcmp(argv[i], "-b") == 0) {
         bSize = atoi(argv[i + 1]);
      }
      if (strcmp(argv[i], "-a") == 0) {
         assc = atoi(argv[i + 1]);
      }
      if (strcmp(argv[i], "-r") == 0) {
         if (strcmp(argv[i + 1], "rr") == 0) {
            strcpy(repPolicy, "Round Robin");
         } else if (strcmp(argv[i + 1], "rnd") == 0) {
            strcpy(repPolicy, "Random");
         }
      } else if (strcmp(argv[i], "-p") == 0) {
         pMem = atoi(argv[i + 1]);
      } else if (strcmp(argv[i], "-u") == 0) {
         pMemUsed = atoi(argv[i + 1]);
      } else if (strcmp(argv[i], "-n") == 0) {
         timeSlice = atoi(argv[i + 1]);
      }
   }

   printf("\n***** Cache Input Parameters *****\n\n");

   printf("Cache Size:\t\t\t\t%d KB\n", cSize);
   printf("Block Size: \t\t\t\t%d bytes\n", bSize);
   printf("Associativity: \t\t\t\t%d\n", assc);
   printf("Replacement Policy: \t\t\t%s\n", repPolicy);
   printf("Physical Memory: \t\t\t%d\n", pMem);
   printf("Percent Memory Used by System: \t\t%d.0%%\n", pMemUsed);
   printf("Instructions / Time Slice: \t\t%d\n", timeSlice);

   printf("\n***** Cache Calculated Values *****\n\n");

   double blockNum = (cSize * pow(2, 10)) / bSize; // number of blocks
   int rowNum = blockNum / assc;                   // number of rows
   double indexSize = log2(rowNum);
   double tagSize = (32 - (indexSize + log2(bSize)));
   double overHead = (blockNum * (tagSize + 1)) / 8;
   double implementation = (cSize * pow(2, 10)) + overHead;
   double rows = (blockNum / assc);

   blockSize = bSize;

   printf("Total # Blocks: \t\t\t%.0f\n", blockNum);
   printf("Tag Size: \t\t\t\t%0.f bits\n", tagSize);
   printf("Index Size: \t\t\t\t%0.f bits\n", indexSize);
   printf("Total # Rows: \t\t\t\t%0.f\n", rows);
   printf("Overhead Size: \t\t\t\t%0.f bytes\n", overHead);
   printf("Implementation Memory Size:\t\t%0.2f ( %0.f byte)s\n", implementation / 1024, implementation);
   printf("Cost: \t\t\t\t\t$ %.2f @ $0.15 per KB\n", (implementation / 1024.0) * 0.15);

   printf("\n***** Physical Memory Calculated Values *****\n\n");

   double pPages = (pMem * pow(2, 20)) / (pow(2, 12)); // physical pages
   double pSystem = pPages * (pMemUsed / (double)(100));
   int pageTableSize = log2(cSize) + 10;
   double totalRam = ((cSize * 1024) * fileCount * pageTableSize) / 8;

   printf("Number of Physical Pages: \t\t%0.f\n", pPages);
   printf("Number of Pages for System: \t\t%0.f\n", pSystem);
   printf("Size of Page Table Entry: \t\t%d\n", pageTableSize);
   printf("Total RAM for Page Table(s): \t\t%0.f\n", totalRam);

   //********************milestone #2*********************

   printf("\n***** CACHE SIMULATION RESULTS *****\n");
   printf("***** CACHE SIMULATION RESULTS *****\n\n");

   char buffer[100];
   FILE *filein = NULL;

   int length;
   int intAddr;
   int dstM;
   int srcM;
   char blank[100];

   // variables for cache miss and hit calculations
   int addrAccess = 0;
   unsigned int tag;
   unsigned int index;
   unsigned int offset;
   int maxOffset = bSize;

   // variables for instruction bytes and arcdst bytes
   //	int *instBytes = 0;
   //	int *srcdstBytes = 0;

   int row = (int)rows + 1;
   int cache[(row)][assc];
   int indexBits = (int)log2(rows);
   int offsetbits = (int)log2(bSize);
   int tagbits = (32 - indexBits - offsetbits);
   int j;
   int tBlocks = row * assc;
   //	int addrCycle = 0;

   for (i = 1; i <= row; ++i) {
      for (j = 0; j < assc; ++j) {
         cache[i][j] = -1;
      }
   }

   if ((filein = fopen(trace1, "r"))) {
      while (!feof(filein)) {
         fgets(buffer, 100, filein);
         sscanf(buffer, "EIP (%d): %x", &length, &intAddr);

         offset = intAddr << (32 - offsetbits);   // shift left to find the offset
         offset = offset >> (32 - offsetbits);    // shift right to bring the offset back
         tag = intAddr >> (32 - tagbits);         // shift right to cut everything but the tag
         index = intAddr << tagbits;              // shift left to cut off the tag
         index = index >> (offsetbits + tagbits); // shift right by the rest of the bits to bring it back
         numAddress += 1;
         addrCycle += 2;
         instructionCount += 1;
         insertCache(row, assc, cache, tag, index, offset, length, maxOffset, &instBytes);

         // caching for srcD and srcM
         fgets(buffer, 100, filein);
         sscanf(buffer, "dstM: %x %s    srcM: %x", &dstM, blank, &srcM);

         if (dstM != 0) { // cut out all of the dst with 0 for the addresses

            offset = intAddr << (32 - offsetbits);
            offset = offset >> (32 - offsetbits);
            tag = intAddr >> (32 - tagbits);
            index = intAddr << tagbits;
            index = index >> (offsetbits + tagbits);
            numAddress += 1;
            // caching for dstM
            insertCache(row, assc, cache, tag, index, offset, 4, maxOffset, &srcdstBytes);
         }

         if (srcM != 0) {
            offset = intAddr << (32 - offsetbits);
            offset = offset >> (32 - offsetbits);
            tag = intAddr >> (32 - tagbits);
            index = intAddr << tagbits;
            index = index >> (offsetbits + tagbits);
            numAddress += 1;
            // caching for srcM
            insertCache(row, assc, cache, tag, index, offset, 4, maxOffset, &srcdstBytes);
         }
         if (srcM != 0 || dstM != 0) {
            addrCycle += 1;
            instructionCount += 1;
         }

         fgets(buffer, 100, filein);
      }
   } else {
      printf("file did not open\n");
   }

   //	fclose(filein);

   // second trace file
   if ((filein = fopen(trace2, "r"))) {
      while (!feof(filein)) {
         fgets(buffer, 100, filein);
         sscanf(buffer, "EIP (%d): %x", &length, &intAddr);

         offset = intAddr << (32 - offsetbits);
         offset = offset >> (32 - offsetbits);
         tag = intAddr >> (32 - tagbits);
         index = intAddr << tagbits;
         index = index >> (offsetbits + tagbits);
         numAddress += 1;
         addrCycle += 2;
         instructionCount += 1;
         insertCache(row, assc, cache, tag, index, offset, length, maxOffset, &instBytes);

         // caching for srcD and srcM
         fgets(buffer, 100, filein);
         sscanf(buffer, "dstM: %x %s    srcM: %x", &dstM, blank, &srcM);

         if (dstM != 0) {

            offset = intAddr << (32 - offsetbits);
            offset = offset >> (32 - offsetbits);
            tag = intAddr >> (32 - tagbits);
            index = intAddr << tagbits;
            index = index >> (offsetbits + tagbits);
            numAddress += 1;
            // caching for dstM
            insertCache(row, assc, cache, tag, index, offset, 4, maxOffset, &srcdstBytes);
         }

         if (srcM != 0) {
            offset = intAddr << (32 - offsetbits);
            offset = offset >> (32 - offsetbits);
            tag = intAddr >> (32 - tagbits);
            index = intAddr << tagbits;
            index = index >> (offsetbits + tagbits);
            numAddress += 1;
            // caching for srcM
            insertCache(row, assc, cache, tag, index, offset, 4, maxOffset, &srcdstBytes);
         }

         if (dstM != 0 || srcM != 0) {
            instructionCount += 1;
            addrCycle += 1;
         }

         fgets(buffer, 100, filein);
      }
   } else {
      printf("File did not open.\n");
   }

   //	fclose(filein);

   // third trace file
   if ((filein = fopen(trace3, "r"))) {
      while (!feof(filein)) {
         fgets(buffer, 100, filein);
         sscanf(buffer, "EIP (%d): %x", &length, &intAddr);

         offset = intAddr << (32 - offsetbits);
         offset = offset >> (32 - offsetbits);
         tag = intAddr >> (32 - tagbits);
         index = intAddr << tagbits;
         index = index >> (offsetbits + tagbits);
         numAddress += 1;
         addrCycle += 2;
         instructionCount += 1;
         insertCache(row, assc, cache, tag, index, offset, length, maxOffset, &instBytes);

         // caching for srcD and srcM
         fgets(buffer, 100, filein);
         sscanf(buffer, "dstM: %x %s    srcM: %x", &dstM, blank, &srcM);

         if (dstM != 0) {

            offset = intAddr << (32 - offsetbits);
            offset = offset >> (32 - offsetbits);
            tag = intAddr >> (32 - tagbits);
            index = intAddr << tagbits;
            index = index >> (offsetbits + tagbits);
            numAddress += 1;
            // caching for dstM
            insertCache(row, assc, cache, tag, index, offset, 4, maxOffset, &srcdstBytes);
         }

         if (srcM != 0) {
            offset = intAddr << (32 - offsetbits);
            offset = offset >> (32 - offsetbits);
            tag = intAddr >> (32 - tagbits);
            index = intAddr << tagbits;
            index = index >> (offsetbits + tagbits);
            numAddress += 1;
            // caching for srcM
            insertCache(row, assc, cache, tag, index, offset, 4, maxOffset, &srcdstBytes);
         }

         if (srcM != 0 || dstM != 0) {
            addrCycle += 1;
            instructionCount += 1;
         }

         fgets(buffer, 100, filein);
      }
      fclose(filein);
   } else {
      printf("File did not open.\n");
   }

   //	fclose(filein);

   int cacheMiss = confMiss + compMiss;
   double missRate = (cacheMiss * 100) / (double)cacheAccess;
   double hitRate = 100 - missRate;
   int totalCycles = (addrCycle + cycleH + cycleM);
   int overheadSize = ((int)blockNum * (tagSize + 1)) / 1024;
   double unusedC = ((blockNum - compMiss) * (bSize + (tagSize + 1) / (double)8)) / (double)1024;
   double cacheSize = ((blockNum) * (16 + overHead)) / (double)1024;
   double unusedP = unusedC / (implementation / (double)1024);
   double waste = unusedC * 0.15;

   printf("Total Cache Accesses:\t\t%d\t(%d addresses)\n", cacheAccess, numAddress);
   printf("Instruction Bytes:\t\t%d\tSrcDst Bytes: %d\n", (instBytes * 2), srcdstBytes);
   printf("Cache Hits:\t\t\t%d\n", cacheHits);
   printf("Cache Misses:\t\t\t%d\n", confMiss + compMiss);
   printf("--- Compulsory Misses:\t%d\n", compMiss);
   printf("--- Conflict Misses: \t%d\n\n", confMiss);
   printf("***** ***** CACHE HIT & MISS RATE: ***** *****\n\n");
   printf("Hit Rate:\t\t\t%0.4f\n", hitRate);
   printf("Miss Rate:\t\t\t%0.4f\n", missRate);
   printf("CPI:\t\t\t\t%0.2f Cycles/Instruction\t(%d)\n", totalCycles / (double)instructionCount, instructionCount);
   printf("Unused Cache Space:\t\t%0.2f KB / %0.2f KB = %0.2f Waste: $%0.2f\n", unusedC, implementation / (double)1024, unusedP * 100, waste);
   printf("Unused Cache Blocks:\t\t%d / %d\n", ((int)blockNum - blockCount), (int)blockNum);

   //********************pagetable test*********************
   pagetable *pt;
   int testSize = 7;

   pt = initPtTable(testSize);

   insertToPt(pt, createEntry(pt, 212121111415));
   insertToPt(pt, createEntry(pt, 545648165175));
   usleep(5000);
   insertToPt(pt, createEntry(pt, 5456175));

   printPt(pt);

   return 0;
}
