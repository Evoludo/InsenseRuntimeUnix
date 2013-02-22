
#ifndef RADIOPACKET_H_
#define RADIOPACKET_H_
#include "String.h"
#include "DAL_mem.h"
#include "AnyType.h"

// Radio packet with AnyType payload
// Packets of this type are sent to and received from Insense programs
typedef struct RadioPacket RadioPacket_struct, *RadioPacket_PNTR ;
struct RadioPacket { 
	void (*decRef)(RadioPacket_PNTR  pntr);
	int addr;
	AnyTypePNTR payload;
};

// Radio Packet with serialised payload
// Packets of this type are sent to and received from the xmac module in InceOS 
// TODO and the INCH module
// The packet type definition is mirrored in xmac.h
typedef struct RadioPacketSP RadioPacketSP_struct, *RadioPacketSP_PNTR;
struct RadioPacketSP { 
	int addr;
	void  *payload;
	unsigned char len;
};

// Radio packet constructors
extern RadioPacket_PNTR construct_RadioPacket( int addr, AnyTypePNTR payload);
extern RadioPacketSP_PNTR construct_RadioPacketSP( int addr, void *payload, unsigned char len);

// Marshalling functions
extern void *serialiseAnyType(AnyTypePNTR data, int *size);
//extern void *useRTSerialiseAnyType(AnyTypePNTR data, int *used, void *dest);
extern AnyTypePNTR deserialiseToAnyType(void *buffer);

#endif /* RADIOPACKET_H_ */ 

