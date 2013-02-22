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
