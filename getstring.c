/*
 * getstring implmentation for Tmote Sky 
 * 
 * @author jonl
 */

#include "getstring.h"
#include "getchar.h"
#include <stdio.h>
#include <iomacros.h>
#include <msp430x16x.h>


char *__getstring(void){
	static char buf[LINE_LENGTH];
	int c, i=0;
	while(i < LINE_LENGTH && (c = getchar()) != EOF){
		buf[i++] = (char) c;
	}
	buf[i++] = '\0'; // string terminator
	return buf;
}
