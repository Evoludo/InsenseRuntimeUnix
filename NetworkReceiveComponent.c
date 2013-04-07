/**
 * NetworkReceiveComponent.c
 * Component to receive send data from another InSense program using sockets and deserialise it.
 *
 * @author Andrew Bell
 *
 */


#include "NetworkReceiveComponent.h"

static void decRef_NetworkReceive( NetworkReceivePNTR this )
{ 
	channel_unbind( this->output_comp ) ;
} 

void behaviour_NetworkReceive( NetworkReceivePNTR this )
{ 
	inceos_event_t op_status;// for exception handling

	int nbytes;
	void* buffer = NULL;
	NetPacket_PNTR recv_data = NULL;
	char* address; 

	int listen_socket;
	while((listen_socket = unicast_listen()) < 0)
	{
		sleep(30);
	}
	
	while( ! this->stopped )
	{ 
		nbytes = unicast_receive(listen_socket, &buffer, &address);
		if(nbytes < 1)
		{
			continue;
		}
		DAL_assign(&recv_data , construct_NetPacket(Construct_String1(address), deserialiseToAnyType(buffer)));

		DAL_incRef(recv_data); // someone else will also be using this data after channel_send
		channel_send(this->output_comp, &recv_data);
	} 
	component_exit(  ) ;

} 

void NetworkReceive_init_globals( NetworkReceivePNTR this ) 
{ 
	this->decRef = decRef_NetworkReceive;
	this->output_comp=channel_create( sizeof( NetPacket_PNTR  ) ,CHAN_OUT ) ;
} 

void Construct_NetworkReceive0( NetworkReceivePNTR this, int _argc, void* _argv[], pthread_mutex_t* init )
{ 
	NetworkReceive_init_globals( this ) ;
	// this may look unsafe, but the mutex is, in fact, locked by the new thread
	pthread_mutex_unlock( init ) ;
	behaviour_NetworkReceive( this ) ;
} 


