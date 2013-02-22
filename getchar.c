/*
 * getchar implmentation for Tmote Sky 
 * 
 * @author jonl
 */

#include "getchar.h"
#include <stdio.h>
#include <iomacros.h>
#include <msp430x16x.h>


//char _low_level_get(void){
//	printf("low_level_get: wait for char\n");
//	while (!(IFG2 & URXIFG1));
//	printf("low_level_get: got char\n");
//	return (U1RXBUF);
//	//	while (!(UCA0IFG & UCRXIFG));
//	//	return (UCA0RXBUF);
//}
//


// circular queue buffer for incoming data
static char qbuff[QSIZE];           // +2 for \n & \0
static volatile int qfront;         // index for writing to buffer 
static volatile int qback;          // index for reading buffer 


// macros for q impl
#define QLEN              ( (qback >= qfront) ? (qback - qfront) : (qback + QSIZE - qfront))
#define QUEUE_END         ( ( qfront + QLEN ) % QSIZE )
#define CONTAINS_DATA     ( QLEN > 0 )
#define IS_EMPTY          ( QLEN == 0 )
#define IS_FULL           ( QLEN == QSIZE )
#define HAS_SPACE         ( QLEN  < QSIZE )



static void enq(char c){
	qbuff[qback] = c ; 
	qback = (qback + 1) % QSIZE;
}

static char deq(){
	char c = qbuff[qfront];
	qfront = (qfront + 1) % QSIZE;
	return c;
}


int uart_isr_input_handler(unsigned char c){
	if(HAS_SPACE){
		if (c == In_EOL) {
			enq(EOF);
		} 
		else {
			enq(c);
		}
		return 1; // all went well if we go there, return
	} 
	return 1; // couldn't process incoming data, return
}


/*
 * blocking getchar routine
 * returns 
 * 
 */
int __getchar(void) {
	while(!CONTAINS_DATA); // wait for data
	return deq();
}
