
#ifndef IARRAY_H_
#define IARRAY_H_

#include "Bool.h"
//#include "inceos.h"
#include "unix.h"

/* ****************************************************
 *
 *
 *  Simple general purpose array code
 *
 *
 * ****************************************************/


typedef struct IArray_data *IArrayPNTR, IArrayStruct;
struct IArray_data {
  void (*decRef)(IArrayPNTR pntr);
  unsigned short type_size : 6;
  unsigned short length : 10;
  void *data;
};

extern IArrayPNTR Construct_Array(unsigned capacity, unsigned type_size, 
				  void *init, bool contains_pointers);
extern void *array_loc(IArrayPNTR a, unsigned index, bool *success);



#endif /*IARRAY_H_*/

