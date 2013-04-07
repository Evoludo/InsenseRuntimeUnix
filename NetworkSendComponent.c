#include "NetworkSendComponent.h"

static void decRef_NetworkSend( NetworkSendPNTR this )
{ 
	channel_unbind( this->input_comp ) ;
} 

void behaviour_NetworkSend( NetworkSendPNTR this )
{ 
	inceos_event_t op_status;// for exception handling

	NetPacket_PNTR packet;
	char* send_data;
	int send_size;
	char* address;

	while( ! this->stopped )
	{ 
		channel_receive(this->input_comp, &packet);
		address = packet->addr->data;
		send_data = serialiseAnyType(packet->payload, &send_size);
		unicast_send(address, send_data, send_size);
	} 
	component_exit(  ) ;

} 

void NetworkSend_init_globals( NetworkSendPNTR this ) 
{ 
	this->decRef = decRef_NetworkSend;
	this->input_comp=channel_create( sizeof( NetPacket_PNTR  ) ,CHAN_IN);
} 

void Construct_NetworkSend0( NetworkSendPNTR this, int _argc, void* _argv[], pthread_mutex_t* init )
{ 
	NetworkSend_init_globals( this ) ;
	// this may look unsafe, but the mutex is, in fact, locked by the new thread
	pthread_mutex_unlock( init ) ;
	behaviour_NetworkSend( this ) ;
} 


