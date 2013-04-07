/**
 * KeyboardComponent.c
 * A component that reads lines in from the keyboard and sends them on an output channel.
 * Blocks unless there is input on stdin, then as normal on sending to a channel.
 *
 * @author Andrew Bell
 *
 */


#include "KeyboardComponent.h"

static void decRef_Keyboard( KeyboardPNTR this )
{ 
	channel_unbind( this->output_comp ) ;
} 

void behaviour_Keyboard( KeyboardPNTR this )
{ 
	inceos_event_t op_status;// for exception handling

	int nbytes;
	size_t buffsize = 0;
	char* buffer;
	StringPNTR send_string = NULL;

	while( ! this->stopped )
	{ 
		nbytes = getline(&buffer, &buffsize, stdin);
		buffer[nbytes - 1] = '\0';
		DAL_assign(&send_string , Construct_String1(buffer));

		DAL_incRef(send_string); // someone else will also be using this data after channel_send
		channel_send(this->output_comp,&send_string);
	} 
	component_exit(  ) ;

} 

void Keyboard_init_globals( KeyboardPNTR this ) 
{ 
	this->decRef = decRef_Keyboard;
	this->output_comp=channel_create( sizeof( StringPNTR  ) ,CHAN_OUT ) ;
} 

void Construct_Keyboard0( KeyboardPNTR this, int _argc, void* _argv[], pthread_mutex_t* init )
{ 
	Keyboard_init_globals( this ) ;
	pthread_mutex_unlock( init ) ;
	behaviour_Keyboard( this ) ;
} 


