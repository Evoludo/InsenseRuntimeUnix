/*
 * cstring.c 
 *
 * to provide some common ops on strings in c
 *
 */

#include "cstring.h"


/*
 * Convert a base 10 string representation of an address into a pointer
 * @param addr_as_decimal_string The string to convert into a pointer
 * @return A pointer created from the address
 */
void* string_to_address( char *addr_as_decimal_string ) {
	unsigned addr_as_int = atoi( addr_as_decimal_string );	
	return (void*) addr_as_int;
}
