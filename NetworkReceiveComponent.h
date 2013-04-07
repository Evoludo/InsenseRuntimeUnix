#include "unix.h"
#include "String.h"
#include "NetPacket.h"
#include "UnixNetwork.h"
#include "MarshallerFull.h"

#ifndef NETRECV_COMP_H
#define NETRECV_COMP_H

typedef struct NetworkReceive *NetworkReceivePNTR , NetworkReceiveStruct;
struct NetworkReceive
{
	void (*decRef)(NetworkReceivePNTR pntr);
	bool stopped;
	Channel_PNTR output_comp;
};

extern void Construct_NetworkReceive0( NetworkReceivePNTR this, int _argc, void* _argv[], pthread_mutex_t* init ) ;
extern void behaviour_NetworkReceive( NetworkReceivePNTR this );
#endif

