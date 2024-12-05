#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <time.h>
typedef struct PtEntry {
   int logicalAddress;
   int physicalAddress;
   int cycleLastUsed;
   struct PtEntry *next;
} PtEntry;

/*this is a liked list containing all page table entries*/
typedef struct pagetable {
   PtEntry *head;
   PtEntry *tail;
   int maxLen;
   int curLen;
} pagetable;

pagetable *initPtTable(int maxLength);                       /*create the pagetable structure*/
PtEntry *createEntry(pagetable *pt, int address, int cycle); /*create an pagetable entry*/
int logicalToPhysicalConvert(int address, int offset);       /*convert logical address to physical address*/
int findNextEmpty(pagetable *pt);                            /*return the next empty index of the page table if there isnt one return -1*/
int insertToPt(pagetable *pt, PtEntry *currEntry);           /*insert entry to point return 0 if suceesful, return 1 if failed*/
int FindLRU(PtEntry *head);                                  /*find LRU from the page table return the index of it, return -1 if failed*/
int removeElement(pagetable *pt, int index);                 /*remove specified element*/
int printPt(pagetable *pt);                                  /*prints out the contents of the page table, used for debuging*/

#endif