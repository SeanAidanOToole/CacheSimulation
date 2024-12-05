#include "pagetable.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

pagetable *initPtTable(int maxLength) {
   pagetable *currPageTable;
   currPageTable = malloc(sizeof(pagetable));
   currPageTable->head = NULL;
   currPageTable->tail = NULL;
   currPageTable->curLen = 0;
   currPageTable->maxLen = maxLength;

   return currPageTable;
}
PtEntry *createEntry(pagetable *pt, int address, int cycle) {
   PtEntry *currEntry;
   currEntry = malloc(sizeof(PtEntry));
   currEntry->next = NULL;
   currEntry->cycleLastUsed = cycle;
   currEntry->logicalAddress = address;
   currEntry->physicalAddress = logicalToPhysicalConvert(address, pt->curLen);

   return currEntry;
}
int logicalToPhysicalConvert(int address, int ptNum) {
   int offset;

   offset = address << 5;
   offset = offset >> 5;

   return offset + (ptNum << 7);
}
int findNextEmpty(pagetable *pt) {
   PtEntry *curr;
   int currInd = 1;

   if (pt->head == NULL) {
      return 1;
   }

   curr = pt->head;

   while (curr->next != NULL) {
      curr = curr->next;
      currInd++;
   }

   return currInd + 1;
}
int insertToPt(pagetable *pt, PtEntry *currEntry) {
   int index;
   int curInd = 2;
   PtEntry *curr;

   if (pt->curLen == pt->maxLen) {
      return 1;
   }

   index = findNextEmpty(pt);
   curr = pt->head;

   /*case if next avaible index is head*/
   if (index == 1) {
      /*check if the page table is empty*/
      if (pt->curLen == 0) {
         pt->head = currEntry;
         pt->tail = currEntry;
      } else {
         currEntry->next = pt->head;
      }
      pt->curLen++;
      return 0;
   }

   /*case if next avaible index is tail*/
   if (index == pt->curLen + 1) {
      pt->tail->next = currEntry;
      pt->tail = currEntry;
      pt->curLen++;
      return 0;
   }

   /*case if the next avaible index is between the head and tail of the pagetable*/
   while (curr->next != NULL) {
      /*increment list*/
      curr = curr->next;
      curInd++;
      /*reached index to add*/
      if (curInd == index + 1) {
         currEntry->next = curr->next;
         curr->next = currEntry;
         return 0;
      }
   }

   /*failed to add to list*/
   return 1;
}
int FindLRU(pagetable *pt) {
   PtEntry *curr;
   int currInd = 1;
   int LRUindex = 0;
   int LRUCycle = INT_MAX;

   curr = pt->head;

   while (curr != NULL) {
      if (LRUCycle > curr->cycleLastUsed) {
         LRUindex = currInd;
         LRUCycle = curr->cycleLastUsed;
      }

      currInd++;
      curr = curr->next;
   }

   return LRUindex;
}
int removeElement(pagetable *pt, int index) {
   int currIndex = 1;
   PtEntry *prev;
   PtEntry *curr;

   if (pt->head == NULL) {
      return 1;
   }

   curr = pt->head;
   prev = NULL;

   /*case if the element deleted is the first one*/
   if (index == 1) {
      pt->head = curr->next;
      free(curr);
      curr = NULL;
      pt->curLen--;
      return 0;
   }

   /*case if the element is the last one to be removed*/
   if (index == pt->curLen) {
      while (curr != NULL) {
         /*reached end of list*/
         if (currIndex == pt->curLen) {
            free(pt->tail);
            pt->tail = prev;
            pt->tail->next = NULL;
         }

         prev = curr;
         curr = curr->next;
         currIndex++;
      }

      return 0;
   }

   while (curr->next != NULL) {
      /*reached index to delete*/
      if (currIndex == index) {
         prev->next = curr->next;
         free(curr);
         curr = NULL;
         pt->curLen--;
         return 0;
      }

      /*increment list*/
      prev = curr;
      curr = curr->next;
      currIndex++;
   }

   /*reached end of list and failed to remove*/
   return 1;
}

int printPt(pagetable *pt) {
   PtEntry *curr;
   int index = 1;

   printf("\n***** Contents of page table *****\n\n");
   printf("Max length: %d\n", pt->maxLen);
   printf("Current length: %d\n", pt->curLen);

   curr = pt->head;

   while (curr != NULL) {
      printf("\nElement: %d \n", index);
      printf("Logical Address %x\n", curr->logicalAddress);
      printf("Physical Address %x\n", curr->physicalAddress);
      printf("Cycle last Used: %d\n", curr->cycleLastUsed);

      curr = curr->next;
      index++;
   }

   return 0;
}