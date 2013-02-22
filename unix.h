/*
 * unix.h
 *
 *  Created on: 5 Nov 2012
 *      Author: Andrew Bell
 *
 *      Includes and definitions to replace those lost from InceOS, for compatibility. Most of these will probably be removed.
 */

#ifndef UNIX_H_
#define UNIX_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "Bool.h"
#include "setjmp.h"
#include "DAL_mem.h"
#include "DAL_error.h"
#include "IteratedList.h"

// channel struct stuff
typedef enum { IN, OUT } chan_dir;

typedef struct Channel Channel, *Channel_PNTR;
struct Channel
{
	void (*decRef)(Channel_PNTR pntr); // GC decRef
	chan_dir direction;	// for error checking in bind, etc.
	size_t typesize;	// how large the buffer is
	void* buffer;		// data to send/receive
	bool ready;		// ready flag
	List_PNTR connections; 	// list of type Channel_PNTR, channels we're connected to
	sem_t conns_sem;	// how many connections available
	pthread_mutex_t mutex;	// for locking the channel
	pthread_mutex_t blocked;	// block component if waiting for other channel
};

//typedef Channel_PNTR chan_id; // channel pointer is unique, let's use it as id (remove when refactoring)

// select struct
struct select_struct
{
    int nchans;                 // number of channels
    Channel_PNTR *chans;             // array of channels for which guard conditions are satisfied
    void *buffer;               // ptr to the receiving buffer of appropriate size
    bool have_default;  // do we have default clause
};

typedef enum {
  BAD_CHAN_EVENT = 1,                        // channel index is out of range or its usage flags are not set
  CHAN_RECV_NODATA_EVENT,                    // blocked receiver process woken without data to receive (interrupt woke up rcvr)
  CHAN_RECV_BUFF_EMPTY_EVENT,                // returned by dequeue and send when q is empty
  CHAN_RECV_EVENT,                           // successful receive from channel
  CHAN_SEND_EVENT,                           // successful send over channel
  RADIO_SEND_EVENT,                          // radio has sent data
  RADIO_RECV_EVENT,                          // radio has received data
  CHAN_CONNECT_EVENT,                        // channel has been connected
  PUBLIC_CHAN_QUERY_EVENT,                   // query remote node about its pub lic channels
  NEIGHBOUR_QUERY_EVENT,                     // query remote node about its neighbours
  GENERAL_SUCCESS_EVENT,                     // general success inidicator
  GENERAL_EXCEPTION_EVENT                    // general exception inidcator
} inceos_event_t;


// Exception events which can be caught in Insense programs
typedef enum {
  // exceptions start at GENERAL_EXCEPTION_EVENT == 1+GENERAL_SUCCESS_EVENT
  OperationFailedException = GENERAL_EXCEPTION_EVENT,
  // for ASYNCH send code
  ChannelBufferFullException,                 // thrown by send when local (and connected channel) queue buffers are full
  DivisionByZeroException,
  // For inter-node channels
  SendStatusUnknownException,                 // status of of SYNCH send over inter-node channel is unknown, no ACK received
  DuplicateLCNException,                      // two channels published with the same LCN on a single node
  NodesUnreachableException,                  // there is no route to the remote node
  ChannelsUnknownException,                   // the specified remote channel has not been published on the remote node
  IncompatibleChannelsException,              // two channels are incomaptible for binding to take place
  BindStatusUnknownException                  // status of binding two inter-node channels together is unknown, no ACK received
} inceos_exception_t;


extern unsigned current_stack_bottom;

// component functions

typedef struct Ithis_s
{
	void (*decRef)(void* this);
	bool stopped;
} Ithis_s;

typedef struct args_s
{
	void (*behaviour)(void*);
	Ithis_s* this_ptr;
	int argc;
	void** argv;
	pthread_mutex_t* init;
} args_s;

void *component_create(void(*behaviour)(), int struct_size, int stack_size, int argc, void *argv[]);

void component_stop(void *this_ptr);

void component_exit();

void component_yield(void);

// channel struct typedefs
/* let's get rid of this garbage and start from scratch.
// Channel payload tag def
typedef enum datatag { SCALAR, POINTER } datatag_t;

// Union for scalar channel payload 
typedef union ScalarUnion ScalarUnion, *ScalarUnion_PNTR;
union ScalarUnion {
  int int_value;
  float real_value;
  bool bool_value;
  uint8_t byte_value;
};

// The channel payload definition
typedef struct ChannelPayload ChannelPayloadStruct, *ChannelPayload_PNTR;
struct ChannelPayload {
  datatag_t tag;
  ScalarUnion scalar;
  void* pointer_value;
};


typedef struct Channel ChannelStruct, *Channel_PNTR;
struct Channel {
  void (*decRef)(Channel_PNTR pntr); // decRef for garbage collection
  unsigned ID;                 // unique identity of a channel
  enum direction dir;                 // records insense type e.g. "in integer"
  int typesize;
  ChannelPayloadStruct buffer; // has tag, pointer, and union of scalars
  List_PNTR connections;       // list of connections to other half channels
  bool *ready;                 // ready flag (pntr) can be shared during select
};
*/

// channel functions

Channel_PNTR channel_create(int typesize);

static void Channel_decRef(Channel_PNTR pntr);

bool channel_bind(Channel_PNTR id1, Channel_PNTR id2);

void channel_unbind(Channel_PNTR id);

int channel_select(struct select_struct *s);

int channel_send(Channel_PNTR id, void *buffer, jmp_buf *ex_handler);

int channel_receive(Channel_PNTR id, void *buffer, bool in_ack_after);

int channel_multicast_send(Channel_PNTR id, void *buffer);

void remoteAnonymousUnbind_proc(Channel_PNTR id, void* var);

#endif /* UNIX_H_ */
