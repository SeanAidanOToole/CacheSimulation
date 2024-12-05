#include "pagetable.h"
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
PtEntry *createEntry(pagetable *pt, int address) {
   PtEntry *currEntry;
   currEntry = malloc(sizeof(PtEntry));
   currEntry->next = NULL;
   currEntry->timeLastUsed = time(NULL);
   currEntry->logicalAddress = address;
   currEntry->physicalAddress = logicalToPhysicalConvert(address, pt->curLen);

   return currEntry;
}
int logicalToPhysicalConvert(int address, int offset) {
   int ptNum;

   ptNum = 1;
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
   int index = 1;
   int curInd = 2;
   PtEntry *curr;

   if (pt->curLen == pt->maxLen) {
      return 1;
   }

   index = findNextEmpty(pt);
   curr = pt->head;
   printf("\nCurrent index: %d\n", index);

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
int FindLRU(PtEntry *head) {
   int currInd = 1;
   int LRU = 0;
   time_t LRUtime = 0;

   while (head->next != NULL) {
      if (LRUtime < head->timeLastUsed) {
         LRU = currInd;
         LRUtime = head->timeLastUsed;
      }

      currInd++;
      head = head->next;
   }

   return LRU;
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
      printf("Time: %ld\n", curr->timeLastUsed);

      curr = curr->next;
      index++;
   }

   return 0;
}