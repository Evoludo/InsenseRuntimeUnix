/* 
 * Memory management function declarations
 *
 *
 *
 */

#ifndef DAL_MEM_H_
#define DAL_MEM_H_

#include<stdlib.h>
#include "Bool.h"

//#ifdef CONTIKI_SKY
//#include"malloc.h"
//#endif

#define BASE_mem_alloc malloc
#define BASE_mem_free free

typedef void (*decRefFunc_t)(void *pntr);

typedef struct MemHeader {
  unsigned long ref_count : 63;
  bool mem_contains_pointers : 1;
} *MemHeader, MemHeaderStruct;


// Every object that contains pointers has a pntr to a function decRef that 
// decrements the references for memory refernced by its pointer fields
typedef struct PointerContainer {
  decRefFunc_t decRef;
} PointerContainerStruct, *PointerContainerPNTR;

// CopyableObject logically extends PointerContainer
// Every object that can be copied has a reference to a copy function
typedef struct CopyableObject { 
  decRefFunc_t decRef;
  void *(*copyObject)(void *this, bool);
} CopyableObjectStruct, *CopyableObjectPNTR;


extern void DAL_assign(void *generic_var_pntr, void *new_mem);
extern void* DAL_alloc(size_t size, bool mem_contains_pointers);
extern unsigned DAL_getRef(void *pntr);
extern bool DAL_mem_contains_pointers(void *pntr);
extern void DAL_mem_set_contains_pointers(void *pntr, 
					  bool mem_contains_pointers);
extern void DAL_decRef(void *pntr);
extern void DAL_incRef(void *pntr);
extern void DAL_modRef_by_n(void *pntr, int n);
extern void DAL_free(void *pntr);


#endif /*DAL_MEM_H_*/
