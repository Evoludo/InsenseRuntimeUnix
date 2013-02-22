
#include "IteratedList.h"      // for linked list
#include "DAL_error.h"         // for DAL_error macro


bool containsElement(List_PNTR l, void *element){
  if(l==NULL){
#if DEBUG
    DAL_error(NULL_POINTER_ERROR);
#endif
    return(false);
  }
  if(isEmpty(l)){
#if DEBUG
    DAL_error(EMPTY_CONTAINER_ERROR);
#endif
    return(false);
  }
  ListNode_PNTR current = l->first;
  while (current != NULL){
    if(current->payload == element)
      return(true);
    current = current->tail;
  }
  return(false);
}
