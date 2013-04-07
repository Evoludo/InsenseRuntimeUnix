// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printHeaderStart
#ifndef MARSHALLER_H_
#define MARSHALLER_H_

#include "String.h"

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializerPrototype
extern void* serialize_b( bool *p,int* size ) ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializerPrototype
extern void* serialize_i( int *p,int* size ) ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializerPrototype
extern void* serialize_u( unsigned *p,int* size ) ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializerPrototype
extern void* serialize_r( float *p,int* size ) ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializerPrototype
extern void* serialize_8( uint8_t *p,int* size ) ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printSerializerPrototype
extern void* serialize_s( StringPNTR p,int* size ) ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializerPrototype
extern void* deserialize_b( void* p )  ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializerPrototype
extern void* deserialize_i( void* p )  ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializerPrototype
extern void* deserialize_u( void* p )  ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializerPrototype
extern void* deserialize_r( void* p )  ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializerPrototype
extern void* deserialize_8( void* p )  ;
// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printDeSerializerPrototype
extern void* deserialize_s( void* p )  ;

extern void initializeSerializerFunctions();

// Generated from: uk.ac.stand.cs.insense.compiler.unixCCgen.TypeMarshaller::printHeaderEnd
#endif	// MARSHALLER_H_

