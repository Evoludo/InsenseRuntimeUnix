/**
 * NetPacket.h
 * Type operations for NetPacket struct used by NetworkSendComponent and NetworkReceiveComponent.
 *
 * @author Andrew Bell
 *
 */

#include "NetPacket.h"

NetPacket_PNTR copy_NetPacket(NetPacket_PNTR p)
{
	DAL_incRef(p);
	NetPacket_PNTR copy = construct_NetPacket(p->addr, p->payload);
	DAL_decRef(p);
	return copy;
}


void decRef_NetPacket(NetPacket_PNTR pntr)
{
	DAL_decRef(pntr->addr);
	DAL_decRef(pntr->payload);
}


NetPacket_PNTR construct_NetPacket(StringPNTR addr, AnyTypePNTR payload)
{
	NetPacket_PNTR pntr = (NetPacket_PNTR) DAL_alloc(sizeof(NetPacket_struct), true);
	if(pntr == NULL)
	{
		DAL_error(OUT_OF_MEMORY_ERROR);
		return NULL;
	}
	pntr->_decRef = decRef_NetPacket;
	DAL_assign(&pntr->addr, addr);
	DAL_assign(&pntr->payload,payload);
	return pntr;
}

