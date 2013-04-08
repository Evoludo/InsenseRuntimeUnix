#include "KeyboardComponent.h"

static void decRef_Keyboard( KeyboardPNTR this )
{ 
	channel_unbind( this->output_comp ) ;
} 

void behaviour_Keyboard( KeyboardPNTR this )
{ 
	inceos_event_t op_status;// for exception handling

	int buffsize = 1024;
	char* buffer = (char*)malloc(buffsize); // if buff filled, will be replaced with malloc'd space anyway, no DAL_alloc'd

	while( ! this->stopped )
	{ 

		StringPNTR send_string = NULL;
		getline(&buffer, &buffsize, stdin);
		DAL_assign(&send_string , Construct_String0(buffer));

		// Make call to send op 
		DAL_incRef( buffer ) ;
		channel_send( this->output_comp,&buffer,NULL  ) ;
		// end of send op 

		//printString_proc(this, NULL, Construct_String0("Sent string on output:\n") ) ;
		//printString_proc(this, NULL, send_string ) ;

		DAL_decRef( buffer ) ;

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


