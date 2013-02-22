/*
 * Memory management functions
 *
 * @author jonl
 *
 */

#include "DAL_mem.h"
#include "DAL_error.h"

/*
 * returns the reference count for a memory segment allocated by DAL_alloc
 */
unsigned DAL_getRef(void *pntr){
  if(pntr==NULL){
    DAL_error(NULL_POINTER_ERROR);
    return 0;
  }
  MemHeader header = (MemHeader) (pntr - sizeof(MemHeaderStruct));
  return(header->ref_count);
}
