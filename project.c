#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "queue.h"
#include "cacheStore.h"
#define EMPTY -1

int main (int argc, char *argv[]){

	int i;
	int cSize;
	int bSize;
	int assc;
	char repPolicy[12];
	int pMem;
	int pMemUsed;
	int timeSlice;
	int fileCount = 0;
	char * trace1 = NULL;
	char * trace2 = NULL;
	char * trace3 = NULL;

	printf("\nCache Simulation CS 3853 Fall 2024 - Group #07\n");
	printf("\nTrace File(s):\n");

	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-f") == 0){
			printf("\t%s\n", argv[i + 1]);
			if (fileCount == 0){
				trace1 = argv[i + 1];
			}
			else if(fileCount == 1){
				trace2 = argv[i + 1];
			}
			else{
				trace3 = argv[i + 1];
			}
			fileCount++;
		}
		if(strcmp(argv[i], "-s") == 0){
			cSize = atoi(argv[i + 1]);
		}
		if(strcmp(argv[i], "-b") == 0){
			bSize = atoi(argv[i + 1]);
		}
		if(strcmp(argv[i], "-a") == 0){
			assc = atoi(argv[i + 1]);
		}
		if(strcmp(argv[i], "-r") == 0){
			if(strcmp(argv[i + 1], "rr") == 0){
				strcpy(repPolicy, "Round Robin");
			}
			else if(strcmp(argv[i + 1], "rnd") == 0){
				strcpy(repPolicy, "Random");
			}
		}
		else if(strcmp(argv[i], "-p") == 0){
			pMem = atoi(argv[i + 1]);
		}
		else if(strcmp(argv[i], "-u") == 0){
			pMemUsed = atoi(argv[i + 1]);
		}
		else if(strcmp(argv[i], "-n") == 0){
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
	
	double blockNum = (cSize * pow(2,10)) / bSize;		//number of blocks
	int rowNum = blockNum / assc;				// number of rows
	double indexSize = log2(rowNum);
	double tagSize = (32 - (indexSize + log2(bSize)));
	double overHead = (blockNum * (tagSize + 1)) / 8;
	double implementation = (cSize * pow(2,10)) + overHead;
	double rows = (blockNum / assc);

	printf("Total # Blocks: \t\t\t%.0f\n", blockNum);
	printf("Tag Size: \t\t\t\t%0.f bits\n", tagSize);
	printf("Index Size: \t\t\t\t%0.f bits\n", indexSize);
	printf("Total # Rows: \t\t\t\t%0.f\n", rows);
	printf("Overhead Size: \t\t\t\t%0.f bytes\n", overHead);
	printf("Implementation Memory Size:\t\t%0.f bytes\n", implementation);
	printf("Cost: \t\t\t\t\t$ %.2f @ $0.15 per KB\n", (implementation/1024.0) * 0.15);


	printf("\n***** Physical Memory Calculated Values *****\n\n");

	double pPages = (pMem * pow(2,20)) / (pow(2,12));	//physical pages
	double pSystem = pPages * (pMemUsed / (double)(100));
	int pageTableSize = log2(cSize) + 10;
	double totalRam = ((cSize * 1024) * fileCount * pageTableSize) / 8;


	printf("Number of Physical Pages: \t\t%0.f\n", pPages);
	printf("Number of Pages for System: \t\t%0.f\n", pSystem);
	printf("Size of Page Table Entry: \t\t%d\n", pageTableSize);
	printf("Total RAM for Page Table(s): \t\t%0.f\n", totalRam);

	//********************milestone #2*********************

	printf("***** CACHE SIMULATION RESULTS *****\n");
	printf("***** CACHE SIMULATION RESULTS *****\n");

	char buffer[100];
	FILE * filein = NULL;

	int length;
	int intAddr;
	int dstM;
	int srcM;
	char blank[100];

	// variables for cache miss and hit calculations
	int cMiss;
	int cHit;
	int numReads;
	unsigned int tag;
	unsigned int index;
	unsigned int offset;

//	int* cache = malloc((rows * assc) * sizeof(int));

	int cache[(int)rows + 1][assc];
	int tagbits = (32 - (int)log2(rows) - (int)log2(bSize)); /*this line is wrong*/
	int offsetbits = (int)log2(bSize);

	int j;

	for(i = 1; i <= rows; i++){
		for(j = 1; j <= assc; j++){
			cache[i][j] = EMPTY;
		}
	}

	printf("ERROR\n");

	cacheRow** cacheStore;
	int maxIndex;


	maxIndex = blockNum / assc;
	cacheStore = initializeCache(maxIndex, assc, blockNum);

	if((filein = fopen(trace1, "r"))){
		while(!feof(filein)){
			fgets(buffer, 100, filein);
			sscanf(buffer, "EIP (%d): %x", &length, &intAddr);

			offset = intAddr << (32 - offsetbits);
			offset = offset >> (32 - offsetbits);
			tag = intAddr >> (32 - tagbits);
			index = intAddr << tagbits;
			index = index >> (offsetbits + tagbits);

			printf("offset: %x\n", offset);
			printf("index: %x\n", index);
			printf("tag: %x\n", tag);
			printf("intAddr: %x\n\n", intAddr);

			fgets(buffer, 100, filein);
			sscanf(buffer, "dstM: %x %s    srcM: %x", &dstM, blank, &srcM);

			fgets(buffer, 100, filein);
		}

		printf("OffsetBits: %d\n", offsetbits);
		printf("TagBits: %d", tagbits);
	}
	else{
		printf("file did not open\n");
	}

//	free(cache);

	return 0;

}
