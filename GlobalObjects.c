/*
 * File containing definitions of globals that are assumed to exist
 * by the compiler generated code
 *
 */

#include "GlobalObjects.h"
#include "FunctionPair.h"
#include "DAL_mem.h"


bool success;

// Map for (de)serialisation
BSTMap_PNTR serialiserMap;

// Unix globs
KeyboardPNTR Keyboard_glob;
NetworkReceivePNTR NetworkReceive_glob;
NetworkSendPNTR NetworkSend_glob;

void initDALGlobalObjects(){
  success = false;
  
  //component_create(&insense_radio_constructor,sizeof(insense_radio_struct), 250, 0, NULL);
  
#ifdef DALINTERNODECHANNEL
  mapPut(serialiserMap, IPACKET_SERIALID,
	 Construct_FunctionPair((serialf_t)serialize_InterNodePacket,
				(deserialf_t)deserialize_InterNodePacket));
  mapPut(serialiserMap, LPACKET_SERIALID,
	 Construct_FunctionPair((serialf_t)serialize_LinkStatePacket,
				(deserialf_t)deserialize_LinkStatePacket));
  mapPut( serialiserMap, CONNECTION_CHANGE_SERIALID, 
	 Construct_FunctionPair( (serialf_t) serialize_ConnectionChange, 
			    (deserialf_t) deserialize_ConnectionChange )  ) ;
  mapPut( serialiserMap, PUBLIC_CHANNEL_QUERY_SERIALID, 
	 Construct_FunctionPair( (serialf_t) serialize_PublicChanQuery, 
			    (deserialf_t) deserialize_PublicChanQuery )  ) ;
  mapPut( serialiserMap, PUBLIC_CHANNEL_ARRAY_SERIALID, 
	 Construct_FunctionPair( (serialf_t) serialize_PublicChanArray, 
			    (deserialf_t) deserialize_PublicChanArray )  ) ;
  // Use connection struct when transferring channels over inter-node channels
  mapPut( serialiserMap, CONNECTION_SERIALID, 
	 Construct_FunctionPair( (serialf_t) serialize_Connection, 
			   (deserialf_t) deserialize_Connection )  ) ;
  mapPut( serialiserMap, ACK_DATA_SERIALID, 
	 Construct_FunctionPair( (serialf_t) serialize_AckData, 
			    (deserialf_t) deserialize_AckData )  ) ;
#endif /* DALINTERNODECHANNEL */

}

void initUnixGlobalObjects()
{
	// init semaphore for thread counting
	sem_init(&can_exit, 0, 1);
	num_threads = 0;

	// init channel connect/disconnect mutex
	pthread_mutex_init(&conn_op_mutex, NULL);

	// init keyboard component
	DAL_assign(&Keyboard_glob, component_create(Construct_Keyboard0, sizeof(KeyboardStruct) , 52, 0, NULL));

	// init network components
	DAL_assign(&NetworkReceive_glob, component_create(Construct_NetworkReceive0, sizeof(NetworkReceiveStruct) , 52, 0, NULL));
	DAL_assign(&NetworkSend_glob, component_create(Construct_NetworkSend0, sizeof(NetworkSendStruct) , 52, 0, NULL));

	// don't wait for any background components to exit after all user components have exited
	sem_post(&can_exit);
	num_threads = 0;
}
