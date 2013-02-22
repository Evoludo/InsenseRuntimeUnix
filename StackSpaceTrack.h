/*
 * Space tracking in terms of stack usage
 * 
 * @author jonl
 *
 */

#ifndef STACK_SPACE_TRACK_H_
#define STACK_SPACE_TRACK_H_

//#include "sys/sched.h"
#include "unix.h"

#define STACK_MEASURE_OFFSET 6 // for calling printStackUsage which calls getStackPointer

unsigned getStackPointer();
void printStackUsage_proc(void *this, void *op_status);

#endif /* STACK_SPACE_TRACK_H_ */
