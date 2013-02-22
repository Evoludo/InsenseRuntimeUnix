/*
 * Bool impl, constructor and free when dealing with bool pointers
 * 
 * A bit of a waste of space this really, but we decided to
 * share the ready flag (a bool) among multiple channels for
 * the select operation implementation, so we need bool* created
 * on heap dynamically.
 *
 * @author jonl ... ;-(
 *
 */


#include "Bool.h"
#include "DAL_mem.h"
#include "DAL_error.h"

#ifndef DALSMALL
static char *file_name = "Bool"; // used by DAL_error macro
#endif

bool *Construct_bool( bool value ){
  bool *this = (bool *) DAL_alloc(sizeof(int), false);
  if(this == ((bool *) 0)) DAL_error(OUT_OF_MEMORY_ERROR);
  *this = value;
  return this;
}

// no need for freeBool or decRef function
