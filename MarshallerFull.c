/**
 * MarshallerFull.c
 * All the serialisation and deserialisation functions. Grabbed 
 * from marshaller.c generated from insense_progs/unix/marshaller_types.isf
 * and RadioPacket.c
 *
 * @author Andrew Bell
 *
 */

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printIncludes
#include "InsenseRuntime.h"
#include "FunctionPair.h"
#include "BSTMap.h"
#include "GlobalObjects.h"
#include "cstring.h"
#include "MarshallerFull.h"


#ifndef DALSMALL
static char *file_name = "marshaller"; // used by DAL_error macro
#endif
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializer
void* serialize_b( bool *p,int* size )  { 
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.Code::genMallocAssign
	void*  pntr=( void* ) DAL_alloc( 128,false ) ;
	if( pntr== NULL ) { 
		DAL_error(OUT_OF_MEMORY_ERROR);
		return NULL;
	} 
	void* result=pntr;
	int used= 0;
	used += 2;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( pntr,"b",2 ) ;
	pntr =((char *)pntr)+2;	
	used += sizeof( int ) ;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( (char *)pntr,(char *)p,sizeof( int )  ) ;
	*size=used;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializer
void* serialize_i( int *p,int* size )  { 
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.Code::genMallocAssign
	void*  pntr=( void* ) DAL_alloc( 128,false ) ;
	if( pntr== NULL ) { 
		DAL_error(OUT_OF_MEMORY_ERROR);
		return NULL;
	} 
	void* result=pntr;
	int used= 0;
	used += 2;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( pntr,"i",2 ) ;
	pntr =((char *)pntr)+2;	
	used += sizeof( int ) ;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( (char *)pntr,(char *)p,sizeof( int )  ) ;
	*size=used;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializer
void* serialize_u( unsigned *p,int* size )  { 
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.Code::genMallocAssign
	void*  pntr=( void* ) DAL_alloc( 128,false ) ;
	if( pntr== NULL ) { 
		DAL_error(OUT_OF_MEMORY_ERROR);
		return NULL;
	} 
	void* result=pntr;
	int used= 0;
	used += 2;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( pntr,"u",2 ) ;
	pntr =((char *)pntr)+2;	
	used += sizeof( int ) ;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( (char *)pntr,(char *)p,sizeof( int )  ) ;
	*size=used;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializer
void* serialize_r( float *p,int* size )  { 
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.Code::genMallocAssign
	void*  pntr=( void* ) DAL_alloc( 128,false ) ;
	if( pntr== NULL ) { 
		DAL_error(OUT_OF_MEMORY_ERROR);
		return NULL;
	} 
	void* result=pntr;
	int used= 0;
	used += 2;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( pntr,"r",2 ) ;
	pntr =((char *)pntr)+2;	
	used += sizeof( float ) ;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( (char *)pntr,(char *)p,sizeof( float )  ) ;
	*size=used;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializer
void* serialize_8( uint8_t *p,int* size )  { 
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.Code::genMallocAssign
	void*  pntr=( void* ) DAL_alloc( 128,false ) ;
	if( pntr== NULL ) { 
		DAL_error(OUT_OF_MEMORY_ERROR);
		return NULL;
	} 
	void* result=pntr;
	int used= 0;
	used += 2;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( pntr,"8",2 ) ;
	pntr =((char *)pntr)+2;	
	used += sizeof( char ) ;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( (char *)pntr,(char *)p,sizeof( char )  ) ;
	*size=used;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializer
void* serialize_s( StringPNTR p,int* size )  { 
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.Code::genMallocAssign
	void*  pntr=( void* ) DAL_alloc( 128,false ) ;
	if( pntr== NULL ) { 
		DAL_error(OUT_OF_MEMORY_ERROR);
		return NULL;
	} 
	void* result=pntr;
	int used= 0;
	used += 2;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( pntr,"s",2 ) ;
	pntr =((char *)pntr)+2;	
	used += p->length + 1;
	if(used > 128){ 
		DAL_error( SERIALISATION_ERROR ) ;
		return NULL;
	} 
	memncpy( (char *)pntr,p->data,p->length + 1 ) ;
	*size=used;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializer
void* deserialize_b( void* p )  { 
	void *result;
	bool value;
	memncpy( (char *)&value,(char *)p,sizeof( int )  ) ;
	result = Construct_BoolAnyType0( value,"b" ) ;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializer
void* deserialize_i( void* p )  { 
	void *result;
	int value;
	memncpy( (char *)&value,(char *)p,sizeof( int )  ) ;
	result = Construct_IntAnyType0( value,"i" ) ;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializer
void* deserialize_u( void* p )  { 
	void *result;
	unsigned value;
	memncpy( (char *)&value,(char *)p,sizeof( int )  ) ;
	result = Construct_UnsignedIntAnyType0( value,"u" ) ;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializer
void* deserialize_r( void* p )  { 
	void *result;
	float value;
	memncpy( (char *)&value,(char *)p,sizeof( float )  ) ;
	result = Construct_RealAnyType0( value,"r" ) ;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializer
void* deserialize_8( void* p )  { 
	void *result;
	uint8_t value;
	memncpy( (char *)&value,(char *)p,sizeof( char )  ) ;
	result = Construct_ByteAnyType0( value,"8" ) ;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializer
void* deserialize_s( void* p )  { 
	void *result;
	StringPNTR value;
	value=Construct_String1( p ) ;
	result = Construct_PointerAnyType0( value,"s" ) ;
	return result;
} 

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printInitializeSerializerFunctions
void initializeSerializerFunctions() { 
	mapPut( serialiserMap,"b",Construct_FunctionPair( (serialf_t)serialize_b,(deserialf_t)deserialize_b )  ) ;
	mapPut( serialiserMap,"i",Construct_FunctionPair( (serialf_t)serialize_i,(deserialf_t)deserialize_i )  ) ;
	mapPut( serialiserMap,"u",Construct_FunctionPair( (serialf_t)serialize_u,(deserialf_t)deserialize_u )  ) ;
	mapPut( serialiserMap,"r",Construct_FunctionPair( (serialf_t)serialize_r,(deserialf_t)deserialize_r )  ) ;
	mapPut( serialiserMap,"8",Construct_FunctionPair( (serialf_t)serialize_8,(deserialf_t)deserialize_8 )  ) ;
	mapPut( serialiserMap,"s",Construct_FunctionPair( (serialf_t)serialize_s,(deserialf_t)deserialize_s )  ) ;
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


