/**
 * NetworkSendComponent.h
 * Header for component to serialise and send data to another InSense program using sockets.
 *
 * @author Andrew Bell
 *
 */


#include "unix.h"
#include "String.h"
#include "NetPacket.h"
#include "UnixNetwork.h"
#include "MarshallerFull.h"

#ifndef NETSEND_COMP_H
#define NETSEND_COMP_H

typedef struct NetworkSend *NetworkSendPNTR , NetworkSendStruct;
struct NetworkSend
{
	void (*decRef)(NetworkSendPNTR pntr);
	bool stopped;
	Channel_PNTR input_comp;
};

extern void Construct_NetworkSend0( NetworkSendPNTR this, int _argc, void* _argv[], pthread_mutex_t* init ) ;
extern void behaviour_NetworkSend( NetworkSendPNTR this );
#endif

