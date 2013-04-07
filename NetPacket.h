/**
 * NetPacket.h
 * Header file for netpacket struct used by NetworkSendComponent and NetworkReceiveComponent
 *
 * @author Andrew Bell
 *
 */

#ifndef NETPACKET_H
#define NETPACKET_H
#include "InsenseRuntime.h"
#include "String.h"
#include "AnyType.h"

typedef struct NetPacket *NetPacket_PNTR , NetPacket_struct;
struct NetPacket
{
	void (*_decRef)(NetPacket_PNTR  pntr);
	StringPNTR addr;
	AnyTypePNTR payload;
};


extern NetPacket_PNTR construct_NetPacket(StringPNTR addr, AnyTypePNTR payload);

extern NetPacket_PNTR copy_NetPacket(NetPacket_PNTR p);

#endif

