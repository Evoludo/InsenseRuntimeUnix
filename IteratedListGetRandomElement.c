
#include "IteratedList.h"      // for linked list
#include "DAL_error.h"


void *getRandomElement(List_PNTR l){
  unsigned short rand, max;
  if(l==NULL){
    DAL_error(NULL_POINTER_ERROR);
    return(NULL);
  }
  if(isEmpty(l)){
#if DEBUG
    DAL_error(EMPTY_CONTAINER_ERROR);
#endif
    return(NULL);
  }
  max = getListLength(l) - 1; // elements numbered from 0, so max=length-1
  rand = DAL_random(max);
  return(getElementN(l, rand));
}
