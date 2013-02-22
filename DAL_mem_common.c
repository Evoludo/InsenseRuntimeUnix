/*
 * Memory management functions
 *
 * @author jonl
 *
 */

#include <stdio.h>
#include "DAL_mem.h"
#include "DAL_error.h"

#define DEBUG 0

#if DEBUG 
static void *last_max_malloc;
#endif

#if DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#define CHAN_PRINT(...) channel_print(__VA_ARGS__)
#else
#define PRINTF(...)
#define CHAN_PRINT(...)
#endif


#ifndef DALSMALL
static char *file_name = "DAL_mem"; // used by DAL_error macro
#endif

/*
 * Assigns a memory address pntr to a variable of "pointer type" 
 * generic_var_pntr which is passed by reference to the function.
 */

void DAL_assign(void *generic_var_pntr, void *new_mem){
    PRINTF("DAL_assign\n");
  void **var_pntr = (void **) generic_var_pntr;
  void *old_mem = *var_pntr; // can deref void** but not void*
  //printf("assign: old_mem=%p, new_mem=%p\n", old_mem, new_mem);
  // if old memory address is same as the new one don't do anything
  if(old_mem == new_mem) return;
  // Otherwise if old memory is not null update ref_count and free if 0 
  if(old_mem != NULL) 
    DAL_decRef(old_mem);
  // if new memory is not null
  if(new_mem != NULL) 
    DAL_incRef(new_mem);
  // complete the assignment of memory address to caller's variable
  *var_pntr = new_mem; // as var_pntr has type void** can dereference it here
}


/* 
 * Private alloc function used by other forms of alloc
 * Allocates size number of bytes + memory for header containing reference
 * count & type and sets reference count to 0
 */
void *DAL_alloc(size_t size, bool mem_contains_pointers){
    PRINTF("DAL_alloc\n");
  void* pntr = (void *) BASE_mem_alloc( size + sizeof(MemHeaderStruct) );
#if DEBUG == 1
  if(pntr > last_max_malloc){
	  last_max_malloc = pntr;
	  printf("\nm(%u): %p", size, pntr);
  }
#endif
  if(pntr == NULL){
      printf("DAL_alloc ");
    DAL_error(OUT_OF_MEMORY_ERROR);
    //printf(" for %u ", size );
    return NULL;
  }
#if DEBUG == 2
  printf("\nm(%u): %p", size, pntr);
#endif
  // zero memory area to avoid having to set all pointer types to NULL
  // prior to DAL_assign
  memset(pntr, 0, (size + sizeof(MemHeaderStruct)) );
  ((MemHeader) pntr)->ref_count = 0;
  ((MemHeader) pntr)->mem_contains_pointers = mem_contains_pointers;
  size_t temp = sizeof(MemHeaderStruct);
  return ((pntr + sizeof(MemHeaderStruct)));
}

/*
 * decrements reference count for memory referenced by pntr
 */
void DAL_decRef(void *pntr) {
    PRINTF("DAL_decRef\n");

	if(pntr==NULL){
		//DAL_error(NULL_POINTER_ERROR);
		return;
	}
	MemHeader header = (MemHeader) (pntr - sizeof(MemHeaderStruct));
        PRINTF("DAL_decRef: ref count = %d\n", header->ref_count);
	header->ref_count -= 1;
        
	if(header->ref_count <= 0)
        {
		if(header->mem_contains_pointers){    // memory contains pointers
			PointerContainerPNTR thisMem = (PointerContainerPNTR) pntr;
			thisMem->decRef(pntr);  // call decRef function
			DAL_free(pntr);         // free resources

                        PRINTF("Decref: garbage collect with pointers\n");
		} else { // memory is neither component nor contains pointers
			DAL_free(pntr);           // free resources
                        PRINTF("Decref: garbage collect with pointers\n");
		}
	} // ref count is greater 0 after decrement, do nothing
        else
            PRINTF("DAL_decRef: do nothing\n");

}

/*
 * modify reference count for memory referenced by pntr
 */
void DAL_modRef_by_n(void *pntr, int n) {
  if(pntr==NULL){
    DAL_error(NULL_POINTER_ERROR);
    return;
  }
  ((MemHeader)(pntr - sizeof(MemHeaderStruct)))->ref_count += n;
}

/*
 * increments reference count for memory referenced by pntr
 */
void DAL_incRef(void *pntr) {
    PRINTF("DAL_incRef\n");
  DAL_modRef_by_n(pntr, 1);
}


/*
 * frees memory assigned by DAL_alloc and referenced by pntr
 */
void DAL_free(void *pntr){
  PRINTF("DAL_free\n");
  if(pntr==NULL){
    DAL_error(NULL_POINTER_ERROR);
    return;
  }
  MemHeader header = (MemHeader) (pntr - sizeof(MemHeaderStruct));
  if(header->ref_count > 0){
    DAL_error(STILL_REFERENCED_ERROR);
    return;
  }
#if  DEBUG == 2
  printf("\nf(%p)\n", header);
#endif
  BASE_mem_free(header);
  PRINTF("\nf(%p)\n", header);

}


