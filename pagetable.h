#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <time.h>
typedef struct PtEntry {
   int logicalAddress;
   int physicalAddress;
   time_t timeLastUsed;
   PtEntry *next;
} PtEntry;

/*this is a liked list containing all page table entries*/
typedef struct pagetable {
   PtEntry *head;
   PtEntry *tail;
} pagetable;

pagetable *initPtTable();                        /*create the pagetable structure*/
PtEntry *createEntry(int address);               /*create an pagetable entry*/
int insertToPt(pagetable pt, PtEntry currEntry); /*insert entry to point return 0 if suceesful, return 1 if failed*/
int removeFromPt(pagetable pt);                  /*find an element to remove from the page table return the index of the element removed, return -1 if failed*/

#endif