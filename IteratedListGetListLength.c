
#include "IteratedList.h"      // for linked list
#include "DAL_error.h"


unsigned getListLength(List_PNTR l){
  unsigned count = 0;
  if(l==NULL){
    DAL_error(NULL_POINTER_ERROR);
    return((unsigned)NULL);
  }
  ListNode_PNTR current = l->first;
  while (current != NULL){
    current = current->tail;
    count++;
  }
  return(count);
}