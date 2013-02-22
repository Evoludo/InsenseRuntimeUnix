
#include "IteratedList.h"      // for linked list
#include "DAL_error.h"
#include <stdio.h>

// Following only used for testing
void displayList(List_PNTR l, void (*f)(void *val)){
  if(l==NULL){
    DAL_error(NULL_POINTER_ERROR);
    return;
  }
  ListNode_PNTR current = l->first;
  printf("\nList: [ ");
  while(current!=NULL){
    (*f)(current->payload);
    current = current->tail;
  }
  printf("]\n");
}
