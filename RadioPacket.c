
#include "RadioPacket.h"
#include "DAL_error.h"
#include "FunctionPair.h"
#include "GlobalObjects.h"



#ifndef DALSMALL
static char *file_name = "RadioPacket"; // used by DAL_error macro
#endif

static void decRef_RadioPacket( RadioPacket_PNTR pntr ) { 
	DAL_decRef( pntr->payload ) ;
} 

RadioPacket_PNTR construct_RadioPacket(int addr, AnyTypePNTR payload){
	RadioPacket_PNTR pntr = DAL_alloc(sizeof(RadioPacket_struct), true);
	pntr->decRef = decRef_RadioPacket; // will only be called if GC is used
	pntr->addr = addr;
	DAL_assign(&pntr->payload, payload);
	return pntr;
} 


RadioPacketSP_PNTR construct_RadioPacketSP(int addr, void *payload, unsigned char len){
	RadioPacketSP_PNTR pntr = malloc(sizeof(RadioPacketSP_struct));
	if( pntr== NULL ) {
		PRINTF("construct_RadioPacketSP ");
		DAL_error(OUT_OF_MEMORY_ERROR);
		return NULL;
	} 
	pntr->addr = addr;
	pntr->payload = payload;
	pntr->len = len;
	return pntr;
}




// function to serialise AnyType and return pointer to buffer 
// containing data in serial form
void *serialiseAnyType(AnyTypePNTR data, int *size){
	void *serial_buffer;
	FunctionPairPNTR funcs = mapGet(serialiserMap, data->type);
	if (funcs == NULL || funcs->ser == NULL){
		DAL_error(SERIALISATION_ERROR);
		PRINTF("serAny:%s \n", (char *) data->type);
		return NULL;
	}
	//PRINTF(" serany: %s \n", data->type);
	if(stringStartsWith(data->type, "i") || stringStartsWith(data->type, "e"))
		serial_buffer = (*(funcs->ser))(&(data->value.int_value), size);
	else if(stringStartsWith(data->type, "u"))
		serial_buffer = (*(funcs->ser))(&(data->value.unsigned_value), size);
	else if(stringStartsWith(data->type, "r"))
		serial_buffer = (*(funcs->ser))(&(data->value.real_value), size);
	else if(stringStartsWith(data->type, "b"))
		serial_buffer = (*(funcs->ser))(&(data->value.bool_value), size);
	else if(stringStartsWith(data->type, "8"))
		serial_buffer = (*(funcs->ser))(&(data->value.byte_value), size);
	else
		serial_buffer = (*(funcs->ser))(data->value.pointer_value, size);
	return serial_buffer;
}


// utility function to serialise an AnyType into an existing serialisation buffer dest
//void *useRTSerialiseAnyType(AnyTypePNTR data, int *used, void *dest){
//	void *serial_buffer = NULL;
//	int size = 0;
//	DAL_assign(&serial_buffer, serialiseAnyType(data, &size));
//	*used = *used + size;
//	if(*used > 128){ 
//		DAL_error( SERIALISATION_ERROR ) ;
//		DAL_assign(&serial_buffer, NULL);
//		return NULL;
//	} 
//	memncpy(dest, serial_buffer, size);
//	DAL_assign(&serial_buffer, NULL);
//	dest = ((char*) dest) + size;
//	return dest;
//}



// function to de-serialise data received from the radio to a new AnyType
AnyTypePNTR deserialiseToAnyType(void *buffer){
	AnyTypePNTR newAny; void *data;
	FunctionPairPNTR funcs = mapGet(serialiserMap, (char *) buffer);
	if (funcs == NULL || funcs->des == NULL){
		DAL_error(SERIALISATION_ERROR);
		PRINTF("des2any:%s \n", (char *) buffer);
		return NULL;
	}
	//PRINTF(" des2any:%s ", (char *) buffer);
	data = ((char *) buffer) + strlen((char *)buffer) + 1; // 1 for \0
	newAny = (*(funcs->des))(data);
	//PRINTF("%p\n", newAny->value);
	return newAny;
}


