/*
 * String "class" (C-module)
 *
 * original author ???
 *
 * Modified by jonl 2008-05-02.
 * Decided to disallow concat on Strings in Insense, so 
 * have removed String functions and altered String objects to contain
 * a char* reference to string literals (that are in .text section of ELF) 
 * and a length field, set at creation using strlen.
 *
 */

#ifndef STRING_H_
#define STRING_H_

#include "Bool.h"

typedef struct String_data *StringPNTR, StringStruct;
struct String_data {
  void (*decRef)(StringPNTR p);
  char* data;
  unsigned length;
} ;

// Constructor functions
/*
 * Create String object from char * data parameter
 * The string data is assumed to be in .text section of ELF
 * so no dynamic memory is allocated for string data and
 * the data element of the struct is not garbage collected
 */
extern StringPNTR Construct_String0( char* data );

/*
 * Create String object from char * data parameter
 * Memory for the string data is dynamically allocated 
 * and garbage collected.
 */
extern StringPNTR Construct_String1( char* data );

extern int stringCompare(StringPNTR s1, StringPNTR s2);

#endif /*STRING_H_*/
