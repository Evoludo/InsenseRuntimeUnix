/*
 * Space tracking in terms of stack usage
 * 
 * @author jonl
 *
 */

#include "StackSpaceTrack.h"
#include <stdio.h>

// function to get hold of current stack pointer
unsigned getStackPointer() {
	unsigned res;
	asm volatile ("\t"
			"movl %%esp, %0\n\t " // put ESP (sp) into res
			: "=m" (res) 
	);
	return res;
}

void printStackUsage_proc(void *this, void *handler) {
	unsigned sp = getStackPointer();
	sp = sp + STACK_MEASURE_OFFSET; // take stack usage of these routines into account
	int stack_size = current_stack_bottom - sp;
	printf("stack_bottom = %u, sp = %u, usage = %u\n", current_stack_bottom, sp, stack_size);
} 
